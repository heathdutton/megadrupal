<?php
/**
 * @file
 * Admin callback functions.
 */

function paypal_ipn_settings($form, &$form_state) {
  $form['paypal_ipn_use_curl'] = array(
    '#title' => t('Use cURL'),
    '#type' => 'checkbox',
    '#description' => t(''),
    '#default_value' => variable_get('paypal_ipn_use_curl', PAYPAL_IPN_USE_CURL),
  );
  $form['paypal_ipn_follow_location'] = array(
    '#title' => t('Follow Location'),
    '#type' => 'checkbox',
    '#description' => t(''),
    '#default_value' => variable_get('paypal_ipn_follow_location', PAYPAL_IPN_FOLLOW_LOCATION),
  );
  $form['paypal_ipn_use_ssl'] = array(
    '#title' => t('Use SSL'),
    '#type' => 'checkbox',
    '#description' => t(''),
    '#default_value' => variable_get('paypal_ipn_use_ssl', PAYPAL_IPN_USE_SSL),
  );
  $form['paypal_ipn_use_sandbox'] = array(
    '#title' => t('Use Sandbox'),
    '#type' => 'checkbox',
    '#description' => t(''),
    '#default_value' => variable_get('paypal_ipn_use_sandbox', PAYPAL_IPN_USE_SANDBOX),
  );
  $form['paypal_ipn_validate_vars'] = array(
    '#title' => t('Validation Vars'),
    '#type' => 'textarea',
    '#description' => t('These will be validated on each request. On each line enter a variable name in the IPN callback with the expected value separated by a "|"'),
    '#default_value' => _paypal_ipn_nested_array_to_string(_paypal_ipn_get_confirm_vars()),
  );
  $form = system_settings_form($form);
  return $form;
}

function paypal_ipn_settings_validate($form, &$form_state) {
  $validate_vars = &$form_state['values']['paypal_ipn_validate_vars'];
  $list = explode("\n", $validate_vars);
  $list = array_map('trim', $list);
  $list = array_filter($list, 'strlen');

  $validate_vars = array();
  foreach ($list as $position => $text) {

    // Check for an explicit key.
    $matches = array();
    $valid_format = FALSE;
    if (preg_match('/(.*)\|(.*)/', $text, $matches)) {
      if ($matches[1] && $matches[2]) {
        $validate_vars[$matches[1]] = $matches[2];
        $valid_format = TRUE;
      }

    }
    if (!$valid_format) {
      form_error($form['paypal_ipn_validate_vars'], t('Incorrect format'));
      return;
    }

  }
}

/**
 * Generates a string representation of an array of 'allowed values'.
 *
 * This string format is suitable for edition in a textarea.
 *
 * @param $values
 *   An array of values, where array keys are values and array values are
 *   labels.
 *
 * @return
 *   The string representation of the $values array:
 *    - Values are separated by a carriage return.
 *    - Each value is in the format "value|label" or "value".
 */
function _paypal_ipn_nested_array_to_string($values) {
  $lines = array();
  foreach ($values as $key => $value) {
    $lines[] = "$key|$value";
  }
  return implode("\n", $lines);
}


