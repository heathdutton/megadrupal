<?php

/**
 * @file
 * FedEx address validation.
 */

$plugin = array(
  'title' => t('FedEx Address validation, auto correcting'),
  'format callback' => 'fedex_address_validation_format_auto_callback',
  'type' => 'address',
  'weight' => -100,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_validate_callback()
 */
function fedex_address_validation_format_auto_callback(&$format, $address, $context = array()) {
  if ($context['mode'] == 'form') {
    $format['#element_validate'][] = 'fedex_address_validation_element_auto_validate';
  }
}
