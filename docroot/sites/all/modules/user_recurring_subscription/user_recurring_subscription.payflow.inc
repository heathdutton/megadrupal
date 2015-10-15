<?php
/**
 * @file
 * Credit card form subscription module.
 */

/**
 * Implements hook_form().
 */
function user_recurring_subscription_form($form, &$form_state) {
  $form['cc_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Card number'),
    '#default_value' => NULL,
    '#attributes' => array('autocomplete' => 'off'),
    '#size' => 30,
    '#maxlength' => 19,
  );

  $form['cc_exp_month'] = user_recurring_subscription_select_month(t('Expiration date'));
  $form['cc_exp_year'] = user_recurring_subscription_select_year(t('Expiration year'));

  $help_text = l(t('?'),
      'http://www.beaconwine.com/cvv2-example.html',
      array(
        'attributes' =>
        array(
          'onclick' => "window.open(this.href, 'CVV_Info', 'toolbar=0,scrollbars=1,location=0,statusbar=0,menubar=0,resizable=1,width=480,height=460'); return false;",
        ),
        'html' => TRUE)
      );

  $form['cc_cvv'] = array(
    '#type' => 'password',
    '#title' => t('CVV'),
    '#default_value' => NULL,
    '#size' => 10,
    '#attributes' => array('autocomplete' => 'off'),
    '#field_suffix' => $help_text,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('user_recurring_subscription_form_submit', 'user_recurring_subscription_info_submit'),
  );
  return $form;
}

/**
 * Implements hook_form_validate().
 */
function user_recurring_subscription_form_validate($form, &$form_state) {
  // Fetch the CC details from the $_POST directly.
  $cc_data = array();
  $cc_data['cc_number'] = $form_state['values']['cc_number'];
  $cc_data['cc_cvv'] = $form_state['values']['cc_cvv'];
  $cc_data['cc_exp_month'] = $form_state['values']['cc_exp_month'];
  $cc_data['cc_exp_year'] = $form_state['values']['cc_exp_year'];
  $cc_data['cc_number'] = str_replace(' ', '', $cc_data['cc_number']);

  // Validate the CC number if that's turned on/check for non-digits.
  if (!user_recurring_subscription_valid_card_number($cc_data['cc_number'])  || !ctype_digit($cc_data['cc_number'])) {
    form_set_error('cc_number', t('You have entered an invalid credit card number.'));
  }

  // Validate the card expiration date.
  if (!user_recurring_subscription_valid_card_expiration($cc_data['cc_exp_month'], $cc_data['cc_exp_year'])) {
    form_set_error('cc_exp_month', t('The credit card you entered has expired.'));
    form_set_error('cc_exp_year');
  }

  // Validate the CVV number if enabled.
  if (!user_recurring_subscription_valid_cvv($cc_data['cc_cvv'])) {
    form_set_error('cc_cvv', t('You have entered an invalid CVV number.'));
  }
}

/**
 * Implements hook_form_submit().
 */
function user_recurring_subscription_form_submit($form, &$form_state) {
  $subscription = user_recurring_subscription_load($form_state['values']['subscription']);
  if (!empty($subscription->initial_charge_unit)) {
    list($length, $unit) = explode(' ', $subscription->initial_charge_unit);
    watchdog('user_recurring_subscription_uint', 'Initial change unit is @unit',
            array('@unit' => $unit));
    if ($length == 0) {
      $start_date = date('mdY', strtotime('+1 days'));
    }
    else {
      $start_date = date('mdY', strtotime("+" . $subscription->initial_charge_unit));
    }
  }
  else {
    $start_date = date('mdY', strtotime('+1 days'));
  }

  if (!empty($subscription->fee)) {
    $nvp_request['OPTIONALTRX'] = 'S';
    $nvp_request['OPTIONALTRXAMT'] = round($subscription->fee, 2);
  }

  $nvp_request = user_recurring_subscription_request();
  $nvp_request['START'] = $start_date;
  $nvp_request['PAYPERIOD'] = $subscription->regular_interval_unit;
  $nvp_request['AMT'] = round($subscription->price, 2);
  $nvp_request['ACCT'] = $form_state['values']['cc_number'];
  $nvp_request['CVV2'] = $form_state['values']['cc_cvv'];
  $nvp_request['EXPDATE'] = date('my', mktime(0, 0, 0, $form_state['values']['cc_exp_month'], 1, $form_state['values']['cc_exp_year']));
  $nvp_request['EMAIL'] = $form_state['values']['mail'];

  $nvp_response = user_recurring_subscription_api_request($nvp_request, variable_get('user_recurring_subscription_server', 'https://pilot-payflowpro.paypal.com/'));

  if (isset($nvp_response['RESULT']) && $nvp_response['RESULT'] != 0) {
    if (!empty($nvp_response['RESPMSG'])) {
      $error = check_plain($nvp_response['RESPMSG']);
    }
    else {
      $error = t('Unknown transaction code');
    }
    drupal_set_message($error, 'error');
    drupal_goto('user/register');
  }
  else {
    $data['response'] = $nvp_response;
    $data['subscription'] = $subscription;
    $form_state['values']['profile_info']['data'] = $data;
    $form_state['values']['profile_info']['profile_id'] = $nvp_response['PROFILEID'];
    $form_state['values']['profile_info']['next_charge'] = strtotime("+" . $subscription->initial_charge_unit);
    $form_state['values']['profile_info']['price'] = round($subscription->price, 2);
    $form_state['values']['profile_info']['regular_interval'] = $subscription->regular_interval_unit;
    $form_state['values']['profile_info']['sid'] = $subscription->sid;
    $form_state['values']['roles'][$subscription->assigned_role] = TRUE;
    $form_state['values']['cc_number'] = substr($form_state['values']['cc_number'], 4, -1);
  }
}

/**
 * Implements hook_form_submit().
 */
function user_recurring_subscription_info_submit($form, &$form_state) {
  $payflow_profile = new stdClass();
  $payflow_profile = (object) $form_state['values']['profile_info'];
  $payflow_profile->uid = $form_state['values']['uid'];
  $payflow_profile->fee_handler = 'PayFlow Pro';
  $payflow_profile->attempts = 0;
  $payflow_profile->created = REQUEST_TIME;
  $payflow_profile->status = 1;
  drupal_write_record('recurring', $payflow_profile);
}

/**
 * Creates a month select box for a form.
 */
function user_recurring_subscription_select_month($title = NULL, $default = NULL, $allow_empty = FALSE) {
  $options = $allow_empty ? array('' => '') : array();

  $select = array(
    '#type' => 'select',
    '#title' => (is_null($title) ? t('Month') : $title),
    '#options' =>
    $options +
    array(
      1 => t('01 - January'),
      2 => t('02 - February'),
      3 => t('03 - March'),
      4 => t('04 - April'),
      5 => t('05 - May'),
      6 => t('06 - June'),
      7 => t('07 - July'),
      8 => t('08 - August'),
      9 => t('09 - September'),
      10 => t('10 - October'),
      11 => t('11 - November'),
      12 => t('12 - December'),
    ),
    '#default_value' => (is_null($default) ? 0 : $default),
  );

  return $select;
}

/**
 * Creates a year select box for a form.
 */
function user_recurring_subscription_select_year($title = NULL, $default = NULL, $min = NULL, $max = NULL, $allow_empty = FALSE) {
  $min = is_null($min) ? intval(date('Y')) : $min;
  $max = is_null($max) ? intval(date('Y')) + 20 : $max;
  $options = $allow_empty ? array('' => '') : array();

  $select = array(
    '#type' => 'select',
    '#title' => (is_null($title) ? t('Year') : $title),
    '#options' => $options + drupal_map_assoc(range($min, $max)),
    '#default_value' => (is_null($default) ? 0 : $default),
  );

  return $select;
}

/**
 * Validates a credit card number during subscription.
 *
 * Luhn algorithm.  See: http://www.merriampark.com/anatomycc.htm
 */
function user_recurring_subscription_valid_card_number($number) {
  $total = 0;
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
    return FALSE;
  }

  return TRUE;
}

/**
 * Validates an expiration date on a card.
 *
 * @param string $month
 *   The 1 or 2-digit numeric representation of the month, i.e. 1, 6, 12.
 * @param string $year
 *   The 4-digit numeric representation of the year, i.e. 2008.
 *
 * @return TRUE
 *   TRUE for non-expired cards, FALSE for expired.
 */
function user_recurring_subscription_valid_card_expiration($month, $year) {
  if ($year < date('Y')) {
    return FALSE;
  }
  elseif ($year == date('Y')) {
    if ($month < date('n')) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Validates a CVV number during checkout.
 */
function user_recurring_subscription_valid_cvv($cvv) {
  $digits = array(3, 4);
  // Fail validation if it's non-numeric or an incorrect length.
  if (!is_numeric($cvv) || (count($digits) > 0 && !in_array(strlen($cvv), $digits))) {
    return FALSE;
  }

  return TRUE;
}
