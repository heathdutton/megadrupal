<?php
/**
 * @file
 * Provides form handling helpers.
 */

/**
 * Form element validation handler for email elements.
 */
function clients_suitecrm_element_validate_url($element, &$form_state) {
  $value = $element['#value'];
  if ($value != '' && !valid_url($value)) {
    form_error($element, t('%name must be a valid url.', array('%name' => $element['#title'])));
  }
}

/**
 * Form element validation handler for email elements.
 */
function clients_suitecrm_element_validate_email($element, &$form_state) {
  $value = $element['#value'];
  if ($value != '' && !valid_email_address($value)) {
    form_error($element, t('%name must be a valid email.', array('%name' => $element['#title'])));
  }
}

/**
 * Form element validation handler for email elements.
 */
function clients_suitecrm_element_validate_phone($element, &$form_state) {
  $value = $element['#value'];
  if (module_exists('phone')) {
    module_load_include('inc', 'phone', 'include/phone.int');
    if ($value != '' && !valid_int_phone_number($value)) {
      form_error($element, t('%name must be a valid phone number.', array('%name' => $element['#title'])));
    }
  }
  else {
    // Very simple check - remove all allowed non numbers, if then a non numeric
    // value is left something is wrong.
    $value = str_replace(array('.', '(', ')', '[', ']', '-', '+', ' '), '', $value);
    if ($value != '' && !is_numeric($value)) {
      form_error($element, t('%name must be a valid email.', array('%name' => $element['#title'])));
    }
  }
}
