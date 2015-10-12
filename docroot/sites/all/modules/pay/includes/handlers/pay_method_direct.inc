<?php

/**
 * @file
 * The base class for 'direct' payment activities, where payments are collected
 * on third-party sites such as PayPal, and the responses are returned to Drupal
 * on a URL callback.
 */
class pay_method_direct extends pay_method {

  /**
   * This method will be called when the user needs to be sent to the gateway
   * provider for processing.  Override it to include valid request values.
   */
  function direct_request() {
  }

  /**
   * This method will be called on a return response (e.g. PayPal IPN)
   * including an unfiltered version of the $_REQUEST array.  This method must
   * be extended in order for your direct method to work!
   */
  function direct_response($response = array()) {
  }

  /**
   * Augment the available payment actions for transactions that include
   * direct payment methods.
   *
   * The 'response' action will allow us to respond to updates from the gateway.
   */
  function set_valid_actions($pay_form, &$actions) {
    $actions['response'] = array(
      'title' => 'Response',
      'callback' => 'response_action',
      'valid states' => array('pending'),
    );
  }

  /**
   * Create a stub method to ensure that do_action('authorize') is executed.
   * If your subclass wants to change, store, or fixup data before it's sent to
   * the gateway, do it by overriding this method.
   *
   * The 'real' work will be done in the form_submit() function.
   */
  function authorize_action($values = array()) {
    $this->set_direct_pending($values);
  }

  /**
   * Create a stub method to ensure that do_action('complete') is executed.
   * If your subclass wants to change, store, or fixup data before it's sent to
   * the gateway, do it by overriding this method.
   *
   * The 'real' work will be done in the form_submit() function.
   */
  function complete_action($values = array()) {
    $this->set_direct_pending($values);

    // Look for an 'authorize' action in this activity's history.
    foreach ($this->activity->history() as $previous) {
      // If there was a successful authorization, copy its details.
      if ($previous->action == 'authorize' && $previous->result) {
        $this->activity->identifier = $previous->identifier;
        $this->activity->data = $previous->data;
        $this->activity->total = $previous->total;
        $this->payment_type = $previous->payment_type;
        $this->activity->action = 'complete';
      }
    }
  }

  /**
   * Implement a 'response' action, which interprets the response from a
   * direct payment gateway.
   */
  function response_action($values = array()) {
    // Copy the submitted/stored values from the previous activity.
    foreach ($this->activity->history() as $previous) {
      if (in_array($previous->action, array('pending', 'authorize')) && $previous->result) {
        $this->__construct($previous->data);
        if ($previous->action == 'pending') {
          $this->activity->action = $previous->data['action'];
        }
      }
    }

    // Get a boolean response from the response parser.
    return $this->direct_response($values) ? 'complete' : 'canceled';
  }

  /**
   * Store values submitted by a form function, etc.  This will allow us to
   * construct a request to the gateway processor when necessary.
   */
  function set_direct_pending($values = array()) {
    $this->activity->data = $values;
    $this->activity->result = TRUE;

    // Store the current activity for response handling, but store 'pending'.
    $this->activity->data['action'] = $this->activity->action;
    $this->activity->action = 'pending';

    // The direct handler may hardcode its payment_type.
    if (isset($this->payment_type)) {
      $this->activity->payment_type = $this->payment_type;
    }
  }

  function form(&$form, &$form_state) {
    global $user;
    parent::form($form, $form_state);

    if (isset($this->gateway_testmode) && $this->gateway_testmode) {
      drupal_set_message(t('The @name payment method is in test mode. This transaction will not be fully processed.', array('@name' => $this->title())), 'warning');
    }
    $method_form = array();
    $method_form['first_name'] = array(
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#required' => TRUE,
    );
    $method_form['last_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Last name'),
      '#required' => TRUE,
    );
    if ($user->uid) {
      $method_form['mail'] = array(
        '#type' => 'value',
        '#value' => $user->mail,
      );
    }
    else {
      $method_form['mail'] = array(
        '#type' => 'textfield',
        '#title' => t('E-mail'),
        '#required' => TRUE,
      );
    }
    $method_form['billto'] = array(
      '#type' => 'postal',
      '#postal_user' => $user,
      '#title' => t('Billing address'),
      '#required' => TRUE,
    );

    // Add this method_form to the expected place on the parent form.
    $form[$this->pay_form->handler()]['pay_method'][$this->pmid] = $method_form;
  }

  function form_submit(&$form, &$form_state) {
    // Find our transaction object provided by pay_form::form_submit().
    if (isset($form_state['pay_activity']) && $form_state['pay_activity']) {
      foreach ($form_state['pay_activity'] as $key => $activity) {
        if ($activity->pmid == $this->pmid) {
          // Load the activity as the 'current' one.
          $this->activity = pay_activity_load($activity->paid);
          $this->__construct($this->activity->data);
          $this->activity->action = $this->pay_form_action;

          // Effect a redirect to the direct payment gateway.
          if ($this->activity->action && $this->activity->action != 'pending') {
            unset($form_state['storage']);
            $form_state['rebuild'] = FALSE;
            $form_state['redirect'] = $this->direct_request();
            break;
          }
        }
      }
    }
  }
}
