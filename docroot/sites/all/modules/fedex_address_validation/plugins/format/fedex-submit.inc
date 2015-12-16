<?php

/**
 * @file
 * FedEx address validation.
 */

$plugin = array(
  'title' => t('FedEx Address validation, user submit'),
  'format callback' => 'fedex_address_validation_format_submit_callback',
  'type' => 'address',
  'weight' => -100,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_validate_callback()
 */
function fedex_address_validation_format_submit_callback(&$format, $address, $context = array()) {
  if ($context['mode'] == 'form') {
    $format['#element_validate'][] = 'fedex_address_validation_element_submit_validate';
    // Add a hidden field that we'll attach data to.
    $format['proposed']['#type'] = 'hidden';
  }
}
