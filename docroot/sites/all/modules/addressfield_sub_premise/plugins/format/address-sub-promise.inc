<?php

/**
 * @file
 * Extends the default format for adresses.
 */

$plugin = array(
  'title'           => t('Address form extension (sub premise field support)'),
  'format callback' => 'addressfield_format_address_sub_premise',
  'type'            => 'textfield',
  'weight'          => -99,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_format_address_sub_premise(&$format, $address, $context = array()) {
  $address_format = addressfield_get_address_format($address['country']);

  $label = isset($address_format['sub_premise_label']) ? $address_format['sub_premise_label'] : t('Address 3');

  $format['street_block']['sub_premise'] = array(
    '#title'      => $label,
    '#required'   => in_array('sub_premise', $address_format['required_fields']),
    '#access'     => in_array('sub_premise', $address_format['used_fields']),
    '#tag'        => 'div',
    '#size'       => 30,
    '#attributes' => array(
      'class'              => array('sub-premise'),
      'x-autocompletetype' => 'address-line3',
      'autocomplete'       => 'address-line3',
    ),
  );
}
