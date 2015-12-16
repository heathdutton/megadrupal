<?php

/**
 * @file
 * Format plugin for Brazilian address lookup.
 */

$plugin = array(
  'title' => t('Via CEP Brazilian address lookup'),
  'format callback' => 'addressfield_viacep_format_address',
  'type' => 'address',
  'weight' => -80,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_viacep_format_address(&$format, $address, $context = array()) {
  if ($context['mode'] == 'form' && $address['country'] == 'BR') {
    $format['#attributes']['class'][] = 'addressfield-viacep-wrapper';

    // Move Postal Code out of 'locality_block' and move it to the top.
    $format['postal_code'] = $format['locality_block']['postal_code'];
    $format['postal_code']['#weight'] = -5;
    $format['postal_code']['#attributes']['class'][] = 'addressfield-viacep-postal-code';
    unset($format['locality_block']['postal_code']);

    $format['#attached']['js'] = array(
      drupal_get_path('module', 'addressfield_viacep') . '/addressfield_viacep.js',
    );

  }
}
