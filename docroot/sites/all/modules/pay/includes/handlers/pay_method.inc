<?php

/**
 * @file
 * The base class for payment activities. All payment method classes should
 * extend this class.
 */

class pay_method extends pay {

  var $pmid;
  var $title;
  var $description;
  var $min_amount;
  var $max_amount;
  var $pay_form_action = 'complete';
  var $total;
  var $status = 1;
  var $first_name;
  var $last_name;
  var $mail;
  var $billto = array();
  var $table = 'pay_method';
  var $key = 'pmid';

  /**
   * Your payment method should return an array of valid currencies, using
   * 3-digit currency codes.  Example:
   *
   * return array('USD', 'EUR');
   */
  function available_currencies() {
    return array();
  }

  /**
   * Modify the list of payment actions that are valid for a given pay_form.
   *
   * @param $pay_form
   *  A payment form that may use this payment method.
   * @param $actions
   *  An array of default actions, passed by reference. Modify or augment this
   *  as needed.
   */
  function set_valid_actions($pay_form, &$actions) {}

  /**
   * Determine whether an action is valid and appropraite for a transaction.
   *
   * @param $name
   *   The name of the action, e.g. 'complete' , 'cancel', etc.
   * @param $transaction
   *   The transaction that the suggested action will apply to.
   * @param $history
   *   A history of all pay_activity entries that have already been handled by
   *   this payment method on this transaction.  For example, an 'authorize'
   *   activity would allow us to initiate a 'capture' action.
   */
  function valid_action($action, $transaction, $history) {
    // Subclasses can implement this method in order to qualify its result.
    return TRUE;
  }

  /**
   * Cancel a transaction by marking it as 'canceled'.
   *
   * Override this if your payment_method handler needs to do more.
   */
  function cancel_action() {
    $this->activity->result = 1;
    return 'canceled';
  }

  /**
   * Set a default min_amount if none is specified.
   */
  function set_min_amount($val = NULL) {
    $this->min_amount = isset($val) ? (float) $val : 0;
  }

  /**
   * Set a default max_amount if none is specified.
   */
  function set_max_amount($val = NULL) {
    $this->max_amount = isset($val) ? (float) $val : 1000;
  }

  /**
   * Set a default title if none is specified.
   */
  function set_title($val = NULL) {
    if (isset($val)) {
      $this->title = check_plain($val);
    }
    else {
      $info = pay_handlers('pay_method', $this->handler());
      if (!empty($info['title'])) {
        $this->title = $info['title'];
      }
    }
  }

  /**
   * Set a default description if none is specified.
   */
  function set_description($val = NULL) {
    if (isset($val)) {
      $this->description = check_plain($val);
    }
    else {
      $info = pay_handlers('pay_method', $this->handler());
      if (!empty($info['description'])) {
        $this->description = $info['description'];
      }
    }
  }

  function settings_form(&$form, &$form_state) {
    parent::settings_form($form, $form_state);
    $group = $this->handler();

    $form[$group]['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#description' => t('This is what users see when they submit payments. It is a good idea to keep it generic, such as "Credit card".'),
      '#default_value' => $this->title,
    );
    $form[$group]['description'] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#description' => t('This is how the payment method will be listed in admin interfaces. Use something that will set this apart from other payment methods.'),
      '#default_value' => $this->description,
    );
    $form[$group]['min_amount'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#title' => t('Minimum amount'),
      '#default_value' => $this->min_amount,
    );
    $form[$group]['max_amount'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#title' => t('Maximum amount'),
      '#default_value' => $this->max_amount,
    );
    $form[$group]['pay_form_action'] = array(
      '#type' => 'radios',
      '#title' => t('When a payment form is submitted'),
      '#options' => array(
        'complete' => t('Reflect the payment immediately.'),
        'authorize' => t('Collect payment information and authorize the card (if applicable), but do not process payment. Set the transaction to "Authorized".'),
        '' => t('Do not collect payment information or process payment, leave the transaction as "Pending".'),
      ),
      '#description' => t('In some cases, (for example, when delivering a product), you may be legally required to defer payment for review or further action. If the payment method supports it, the transaction will be authorized but not processed.'),
      '#default_value' => $this->pay_form_action,
    );

    $form[$group]['permissions']['default'] = $this->permissions_settings('default');
  }

  // This is called from the form_validate function in a pay_form class.
  /**
   * @todo Please document this function.
   * @see http://drupal.org/node/1354
   */
  function pay_method_validate($form, &$form_state, $element) {

    // Confirm that the amount falls within our min/max settings.
    if (($this->total < $this->min_amount) || ($this->total > $this->max_amount) ) {
      $error = t('The %method payment method requires an amount between %min and %max', array('%method' => $this->title(), '%min' => $this->min_amount, '%max' => $this->max_amount));
      form_error($element['total'], $error);
    }
  }

  /**
   * Return the total number of transactions that have used this payment method.
   */
  function transactionCount() {
    return db_query('SELECT COUNT(1) FROM {pay_activity} a WHERE a.pmid = :pmid
      GROUP BY pxid', array(':pmid' => $this->pmid))->fetchColumn(1);
  }

  /**
   * Disable this payment method.
   */
  function disable() {
    // Call any actions that might want to know about this.
    $this->drupal_invoke('pay_method_delete');

    // Set the status to 0.
    $this->status = 0;
    $this->save();
  }

  /**
   * Delete this payment method.
   */
  function delete() {
    // Disable this instead, if there are any transactions.
    if ($this->transactionCount() > 0) {
      return $this->disable();
    }

    // Call any actions that might want to know about this.
    $this->drupal_invoke('pay_method_delete');

    // Delete this payment method from pay_method.
    db_delete('pay_method')
      ->condition('pmid', $this->pmid)
      ->execute();
    return TRUE;
  }
}
