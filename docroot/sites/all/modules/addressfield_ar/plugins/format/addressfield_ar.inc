<?php

/**
 * @file
 * Address field plugin providing a detailed form for Argentina addresses.
 */

$plugin = array(
  'title' => t('Address form (specific for Argentina)'),
  'format callback' => 'addressfield_ar_format_address_generate',
  'type' => 'address',
  'weight' => 100,
);

/**
 * Format callback for Argentina address.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_ar_format_address_generate(&$format, $address, $context = array()) {
  if ($address['country'] == 'AR') {
    if ($context['mode'] == 'form') {
      $format['street_block']['thoroughfare']['#access'] = FALSE;
      $format['street_block']['premise']['#access'] = FALSE;

      $format['street_block']['thoroughfare_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Street'),
        '#required' => TRUE,
        '#attributes' => array('class' => array('street')),
        '#size' => 30,
        '#default_value' => isset($address['thoroughfare_name']) ? $address['thoroughfare_name'] : '',
      );

      $format['street_block']['thoroughfare_number'] = array(
        '#type' => 'textfield',
        '#title' => t('Number'),
        '#required' => TRUE,
        '#attributes' => array('class' => array('number')),
        '#size' => 4,
        '#default_value' => isset($address['thoroughfare_number']) ? $address['thoroughfare_number'] : '',
      );

      // We use premise_name for the name of the building, premise_number for
      // the floor and sub_premise_number for the office / apartment.
      $format['street_block']['premise_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Block/Building'),
        '#required' => FALSE,
        '#attributes' => array('class' => array('premise-name')),
        '#size' => 4,
        '#default_value' => isset($address['premise_name']) ? $address['premise_name'] : '',
      );

      $format['street_block']['premise_number'] = array(
        '#type' => 'textfield',
        '#title' => t('Floor'),
        '#required' => FALSE,
        '#attributes' => array('class' => array('premise-number')),
        '#size' => 4,
        '#default_value' => isset($address['premise_number']) ? $address['premise_number'] : '',
      );

      $format['street_block']['sub_premise_number'] = array(
        '#type' => 'textfield',
        '#title' => t('Apartment/Office'),
        '#required' => FALSE,
        '#attributes' => array('class' => array('sub-premise-number')),
        '#size' => 4,
        '#default_value' => isset($address['sub_premise_number']) ? $address['sub_premise_number'] : '',
      );

      $format['locality_block']['administrative_area']['#weight'] = 12;
      $format['locality_block']['sub_administrative_area'] = array(
        '#title' => t('County'),
        '#size' => 10,
        '#required' => FALSE,
        '#attributes' => array('class' => array('sub-administrative-area')),
        '#default_value' => isset($address['sub_administrative_area']) ? $address['sub_administrative_area'] : '',
        '#weight' => 10,
      );
    }
  }
}
