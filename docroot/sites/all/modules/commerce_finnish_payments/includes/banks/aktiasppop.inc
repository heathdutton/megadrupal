<?php
/**
 * @file
 *   Contains Aktia specific callback implementation.
 */

/**
 * Payment method callback for settings form.
 */
function commerce_finnish_payments_settings_form_aktiasppop($settings = NULL) {

  // Get bank specific default settings and
  // merge them with $settings parameter.
  $bank = _commerce_finnish_payments_banks('aktiasppop');
  $settings = (array) $settings + $bank['default_settings'];

  // Find out does bank need receiver information
  $receiver = $bank['default_settings']['receiver'] === FALSE ? FALSE : TRUE;

  return commerce_finnish_payments_settings_form($settings, $receiver);

}