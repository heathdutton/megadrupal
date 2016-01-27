<?php

/**
 * @file
 * The base class for credit card payment activities.
 */

class pay_method_gateway extends pay_method {
  var $payment_types = array();
  var $gateway_testmode = TRUE;

  // Each subclass should define these according to capabilities.
  var $gateway_supports_ach = FALSE;
  var $gateway_supports_cc = TRUE;
  var $gateway_supports_recurring = FALSE;

  var $payment_type;
  var $cc_type;
  var $cc_number;
  var $cc_ccv2;
  var $cc_exp_month;
  var $cc_exp_year;
  var $cc_issue_number;

  /**
   * Effect an 'authorize' gateway transaction.  If successful, the transaction
   * state will be set to 'active' and the authorization history will be stored
   * in our records.
   *
   * @return string the recommended transaction state after this action.
   *  Possible states are listed in $pay_transaction->states().
   */
  function authorize_action() {
    // Call execute, which runs the gateway interactions.
    $result = $this->execute();

    // We can't proceed if there's an error, so return 'canceled'.
    return ($result) ? 'active' : 'canceled';
  }

  /**
   * Complete a transaction.  For new transactions, this means creating an
   * AUTH_CAPTURE request.  For existing ones, capture a preauthorization.
   *
   * @return string the recommended transaction state after this action.
   *  Possible states are listed in $pay_transaction->states().
   */
  function complete_action() {

    // Look for an 'authorize' action in this activity's history.
    foreach ($this->activity->history() as $previous) {
      // If there was a successful authorization, copy its details.
      if ($previous->action == 'authorize' && $previous->result) {
        $this->activity->identifier = $previous->identifier;
        $this->activity->data = $previous->data;
        $this->activity->total = $previous->total;
        $this->payment_type = $previous->payment_type;
      }
    }

    // TODO this should be deprecated: 'activity' has been renamed to 'action'.
    $this->activity->activity = $this->activity->action;

    // TODO passing in $this->activity should also be deprecated.
    $result = $this->execute($this->activity);

    // If successful, increase the 'captured' balance.
    if ($result) {
      $this->activity->set_transaction_total($this->activity->total);
    }

    // If the transaction's state is 'pending', this is an auth_capture
    // action. We don't have enough to complete it, so return a failed state.
    if ($this->activity->pay_transaction()->state() == 'pending') {
      return ($result) ? 'complete' : 'canceled';
    }

    // However, if we're going from 'active' (AKA authorized)
    // It may be possible to try again somehow. Return 'active'.
    elseif ($this->activity->pay_transaction()->state() == 'active') {
      return ($result) ? 'complete' : 'active';
    }
  }

  /**
   * Refund a transaction.
   */
  function refund_action() {
  }

  /**
   * Cancel a transaction.
   */
  function cancel_action() {
    // TODO - If there's anything to void, void it!
    return parent::cancel_action();
  }

  /**
   * The URL where your provider accepts requests.
   *
   * Returning NULL prevents anything from executing. Subclasses are responsible
   * for returning a responsible url.
   */
  function gateway_url() { }

  /**
   * The request data that will be sent to your gatway provider.
   *
   * Returning NULL prevents anything from executing. Subclasses are responsible
   * for returning responsible request data.
   */
  function gateway_request() { }

  /**
   * HTTP headers that should be sent to the gateway provider.
   *
   * For example, if your gateway provider requires XML:
   *
   *  function gateway_headers() {
   *     return array('Content-Type' => 'text/xml');
   *  }
   */
  function gateway_headers() {
    return array();
  }

  function gateway_response() { }

  function execute($activity) {
    if ($request = $this->gateway_request()) {
      $ret = drupal_http_request($this->gateway_url(), array('headers' => $this->gateway_headers(), 'method' => 'POST', 'data' => $request));
      if (isset($ret->error)) {
        watchdog('payment', "Gateway Error: @err Payment NOT processed.", array('@err' => $ret->error));
        drupal_set_message(t('We were unable to process your credit card payment. Please verify your card details and try again. If the problem persists, contact us to complete your order.'), 'error');
        $this->activity->data = (array) $ret;
        $this->activity->result = FALSE;
      }
      else {
        $this->activity->result = $this->gateway_response($ret->data);
      }

      // Return TRUE or FALSE on success/failure.
      return ($this->activity->result == 1);
    }
  }

  function cc_expiration() {
    if ($this->cc_exp_month && $this->cc_exp_year) {
      $exp = str_pad( (int) $this->cc_exp_month, 2, '0', STR_PAD_LEFT);
      $exp .= str_pad( (int) $this->cc_exp_year, 2, '0', STR_PAD_LEFT);
      return $exp;
    }
  }

  function cc_issue_number_required($val = NULL) {
    $issue_number_cc_types = array('switch');

    // For validation of form submission.
    if ($val && in_array($val, $issue_number_cc_types)) {
      return TRUE;
    }
    // For display of form field.
    if (!$val) {
      foreach ($issue_number_cc_types as $cc_type) {
        if ($this->payment_types[$cc_type]) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  function set_cc_issue_number($val) {
    $this->cc_issue_number = preg_replace('/[^\d]/', '', $val);
  }

  function set_cc_number($val) {
    $this->cc_number = preg_replace('/[^\d]/', '', $val);

    // Set the credit card type rather than relying on user input.
    $orig_cc_type = $this->cc_type;
    $this->cc_type = NULL;

    $prefix1 = substr($this->cc_number, 0, 1);
    $prefix2 = substr($this->cc_number, 0, 2);
    $prefix3 = substr($this->cc_number, 0, 3);
    $prefix4 = substr($this->cc_number, 0, 4);
    $prefix5 = substr($this->cc_number, 0, 5);
    $prefix6 = substr($this->cc_number, 0, 6);

    /**
     * References: http://www.beachnet.com/~hstiles/cardtype.html
     */
    switch ($prefix1) {
      case 1:
        // JCB: prefix 1800, length 15
        if ($prefix4 == 1800) {
          $this->cc_type = 'jcb';
        }
        break;

      case 2:
        // JCB: prefix 2131, length 15
        if ($prefix4 == 2131) {
          $this->cc_type = 'jcb';
        }

        // enRoute: prefix 2014, 2149 length 15
        elseif ($prefix4 == 2014) {
          $this->cc_type = 'enroute';
        }
        elseif ($prefix4 == 2149) {
          $this->cc_type = 'enroute';
        }
        break;

      case 3:
        // Diners Club: prefix 300-305, length 14
        if (in_array($prefix3, array(300, 301, 302, 303, 304, 305))) {
          $this->cc_type = 'diners';
        }

        // AmEX: prefix 34, 37, length 15
        elseif (in_array($prefix2, array(34, 37))) {
          $this->cc_type = 'amex';
        }

        // Diners Club: prefix 36, 38, length 14
        elseif (in_array($prefix2, array(36, 38))) {
          $this->cc_type = 'diners';
        }

        // JCB: prefix 3, length 16
        else {
          $this->cc_type = 'jcb';
        }
        break;

      case 4:
        // Visa: prefix 4, length 13, 16
        $this->cc_type = 'visa';
        // Switch: prefix 4903, 4905, 4911, 4936
        if (in_array($prefix4, array(4903, 4905, 4911, 4936))) {
          $this->cc_type = 'switch';
        }
        break;

      case 5:
        // Mastercard: prefix 51-55, length 16
        if (in_array($prefix2, array(51, 52, 53, 54, 55))) {
          $this->cc_type = 'mc';
        }
        // Switch: prefix 564182
        if ($prefix6 == 564182) {
          $this->cc_type = 'switch';
        }
        // Maestro: prefix 5018, 5020, 5038
        if (in_array($prefix4, array(5018, 5020, 5038))) {
          $this->cc_type = 'maestro';
        }
        break;

      case 6:
        // Discover: prefix 6011, length 16
        if ($prefix4 == 6011) {
          $this->cc_type = 'discover';
        }
        // Laser: prefix 6304 or 6706 or 6709 or 6771, length 19
        if ($prefix4 == 6304 || $prefix4 == 6706 || $prefix4 == 6709 || $prefix4 == 6771) {
          $this->cc_type = 'laser';
        }
        // Switch: prefix 633110, 6333, 6759,
        if ($prefix6 == 633110 || $prefix4 == 6333) {
          $this->cc_type = 'switch';
        }
        // Maestro: prefix 6759, 6761, 6763
        if (in_array($prefix4, array(6759, 6761, 6763))) {
          $this->cc_type = 'maestro';
        }
        // Solo: prefix 6334 or 6767
        if ($prefix4 == 6334 || $prefix4 == 6767) {
          $this->cc_type = 'solo';
        }
        break;
    }
    // Only if we still fail to determine card type, reset it to user supplied
    // value.
    if (empty($this->cc_type)) {
      $this->cc_type = $orig_cc_type;
    }
  }

  function set_cc_ccv2($val) {
    $this->cc_ccv2 = preg_replace('/[^\d]/', '', $val);
  }

  function cc_number_validate() {
    $total = 0;
    $number = $this->cc_number;

    if ((strlen($number) < 13) || (strlen($number) > 19)) {
      $this->error_message = t('Invalid credit card number.');
      return FALSE;
    }

    // Mod-10 credit card validation.
    for ($i = 0; $i < strlen($number); $i++) {
      $digit = substr($number, $i, 1);
      if ((strlen($number) - $i - 1) % 2) {
        $digit *= 2;
        if ($digit > 9) {
          $digit -= 9;
        }
      }
      $total += $digit;
    }

    if ($total % 10 != 0) {
      $this->error_message = t('Invalid credit card number.');
      return FALSE;
    }

    return TRUE;
  }

  function cc_expiration_validate() {
    if ($this->cc_exp_year == date('y')) {
      if ($this->cc_exp_month < date('m')) {
        $this->error_message = t('The credit card appears to be expired.');
        return FALSE;
      }
    }
    return TRUE;
  }

  function cc_issue_number_validate() {
    if ($this->cc_issue_number_required($this->cc_type) && (strlen($this->cc_issue_number) != 2 || !is_numeric($this->cc_issue_number))) {
      $this->error_message = t('Invalid issue number.');
      return FALSE;
    }
    return TRUE;
  }

  function cc_ccv2_validate() {
    if ($this->cc_ccv2) {
      if ($this->cc_type == 'amex') {
        if (strlen($this->cc_ccv2) != 4) {
          $this->error_message = t('The security code must be 4 numeric digits for American Express cards');
          return FALSE;
        }
      }
      elseif (strlen($this->cc_ccv2) != 3) {
        $this->error_message = t('The security code must be 3 numeric digits.');
        return FALSE;
      }
    }
    return TRUE;
  }

  function payment_types($filter = NULL) {
    $payment_types = array();

    if ($this->gateway_supports_cc) {
      $payment_types = array(
        'visa' => t('Visa'),
        'mc' => t('Mastercard'),
        'amex' => t('American Express'),
        'discover' => t('Discover'),
        'diners' => t("Diner's Club"),
        'laser' => t('Laser'),
        'maestro' => t('Maestro'),
        'switch' => t('Switch'),
        'solo' => t('Solo'),
      );
    }
    if ($this->gateway_supports_ach) {
      // TODO this is a stub. We need to build the echeck form and validation
      // functions for this to be relevant. Thus it's commented out for now.

      //$payment_types['ach'] = t('Online checking transfer.');
    }

    if ($filter) {
      foreach ($payment_types as $key => $label) {
        if (!$filter[$key]) {
          unset($payment_types[$key]);
        }
      }
    }
    return $payment_types;
  }

  function settings_form(&$form, &$form_state) {
    parent::settings_form($form, $form_state);
    $group = $this->handler();

    $form[$group]['gateway'] = array(
      '#type' => 'fieldset',
      '#title' => t('Gateway settings'),
      '#group' => $group,
    );
    $form[$group]['gateway']['payment_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Available payment types'),
      '#options' => $this->payment_types(),
      '#default_value' => $this->payment_types,
      '#required' => TRUE,
      '#parents' => array($group, 'payment_types'),
    );
    $form[$group]['gateway']['gateway_testmode'] = array(
      '#type' => 'checkbox',
      '#title' => t('Test mode'),
      '#default_value' => $this->gateway_testmode,
      '#parents' => array($group, 'gateway_testmode'),
    );
  }

  function form(&$form, &$form_state) {
    global $user;
    $group = $this->pay_form->handler();

    if (isset($this->gateway_testmode) && $this->gateway_testmode) {
      drupal_set_message(t('The @name payment method is in test mode. This transaction will not be fully processed.', array('@name' => $this->title())), 'warning');
    }

    if (isset($form[$group]['pay_method'][$this->pmid])) {
      $method_form = $form[$group]['pay_method'][$this->pmid];
    }

    $method_form['#theme'] = 'pay_cc_form';

    $options = array();
    if ($this->gateway_supports_cc) {
      $options['cc'] = t('Credit or debit card');
    }
    // TODO should reflect 'bank account' input here, but it's unsupported.

    $method_form['payment_type'] = array(
      '#type' => 'select',
      '#title' => t('Payment type'),
      '#options' => $options,
      '#required' => TRUE,
      '#default_value' => 'cc',
      '#access' => count($options) > 1,
    );

    $method_form['first_name'] = array(
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#pre_render' => array('pay_element_set_required'),
    );
    $method_form['last_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Last name'),
      '#pre_render' => array('pay_element_set_required'),
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
        '#pre_render' => array('pay_element_set_required'),
      );
    }
    $method_form['billto'] = array(
      '#type' => 'postal',
      '#postal_user' => $user,
      '#title' => t('Billing address'),
      '#pre_render' => array('pay_element_set_required'),
    );
    $method_form['cc_type'] = array(
      '#type' => 'radios',
      '#title' => t('Card type'),
      '#options' => $this->payment_types($this->payment_types),
    );
    $method_form['cc_number'] = array(
      '#type' => 'textfield',
      '#title' => t('Card number'),
      '#size' => 19,
      '#maxlength' => 19,
      '#pre_render' => array('pay_element_set_required'),
      '#attributes' => array('autocomplete' => 'off'),
    );
    $method_form['cc_ccv2'] = array(
      '#type' => 'textfield',
      '#title' => t('Security code'),
      '#size' => 4,
      '#maxlength' => 4,
      '#attributes' => array('autocomplete' => 'off'),
    );
    if ($this->cc_issue_number_required()) {
      $method_form['cc_issue_number'] = array(
        '#type' => 'textfield',
        '#title' => t('Issue number'),
        '#size' => 2,
        '#maxlength' => 2,
        '#attributes' => array('autocomplete' => 'off'),
      );
    }

    $months = array();
    foreach (range(1, 12) as $i) {
      $key = str_pad($i, 2, '0', STR_PAD_LEFT);
      $months[$key] = $key . '- ' . date('F', strtotime('1-' . $i . '-2000'));
    }
    $method_form['cc_exp_month'] = array(
      '#type' => 'select',
      '#options' => $months,
      '#pre_render' => array('pay_element_set_required'),
      '#attributes' => array('title' => t('Expiration month')),
    );

    $years = array();
    foreach (range(0, 9) as $i) {
      $year = date('Y') + $i;
      $years[substr($year, 2)] = $year;
    }
    $method_form['cc_exp_year'] = array(
      '#type' => 'select',
      '#options' => $years,
      '#pre_render' => array('pay_element_set_required'),
      '#attributes' => array('title' => t('Expiration year')),
    );

    // Add this method_form to the expected place on the parent form.
    $form[$group]['pay_method'][$this->pmid] = $method_form;
    $form[$group]['pay_method'][$this->pmid]['#prefix'] = '<div class="payment-method-form">';
    $form[$group]['pay_method'][$this->pmid]['#suffix'] = '</div>';
  }

  // This is called from the form_validate function in a pay_form class.
  /**
   * @todo Please document this function.
   * @see http://drupal.org/node/1354
   */
  function pay_method_validate($form, &$form_state, $element) {
    parent::pay_method_validate($form, $form_state, $element);

    if ($this->payment_type == 'cc') {

      // Manually handle #required on fields required by this payment_type.
      $required = array(
        'first_name',
        'last_name',
        'mail',
        'cc_number',
        'cc_exp_month',
        'cc_exp_year',
      );

      foreach ($required as $key) {
        if ((!count($this->$key) || (is_string($this->$key) && strlen(trim($this->$key)) == 0)) && isset($element[$key])) {
          form_error($element[$key], t('!name field is required.', array('!name' => $element[$key]['#title'])));
        }
      }

      // Validate the card number and set the form value to our clean version.
      if (!$this->cc_number_validate()) {
        form_error($element['cc_number'], $this->error_message);
      }
      form_set_value($element['cc_number'], $this->cc_number, $form_state);

      // Validate the CCV2 code and set the form value to our clean version.
      if (!$this->cc_ccv2_validate()) {
        form_error($element['cc_ccv2'], $this->error_message);
      }
      form_set_value($element['cc_ccv2'], $this->cc_ccv2, $form_state);

      // Validate the expiration date.
      if (!$this->cc_expiration_validate()) {
        form_error($element['cc_exp_month'], $this->error_message);
      }

      // Set the "payment_type" value to the specific card type.
      form_set_value($element['payment_type'], $this->cc_type, $form_state);

      // Validate the issue number field.
      if (!$this->cc_issue_number_validate()) {
        form_set_error('cc_issue_number', $this->error_message);
      }
    }
  }
}
