<?php

/**
 * @file
 * Australian Regional Settings
 */

/**
 * Implements hook_ec_region_COUNTRYCODE_info().
 */
function ec_store_ec_region_au_info() {
  return array(
    // Measures
    'weight' => 'KG',
    'length' => 'M',
    'area' => 'M2',
    'volume' => 'M3',
    // Geo
    'state_name' => t('State'),
    'use_state_names' => FALSE,
    'zip' => TRUE,
    'zip_name' => t('Postcode'),
    'states' => array(
      'ACT' => t('Australian Capital Territory'),
      'NSW' => t('New South Wales'),
      'NT' => t('Northern Territory'),
      'QLD' => t('Queensland'),
      'SA' => t('South Australia'),
      'TAS' => t('Tasmania'),
      'VIC' => t('Victoria'),
      'WA' => t('Western Australia'),
    ),
    // Price format
    'payment_symbol' => '$',
    'payment_symbol_position' => 1, // Left
    'payment_thousands' => ',',
    'payment_decimal' => '.',
    'payment_decimal_places' => 2,
  );
}
