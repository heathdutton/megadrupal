<?php

/**
 * @file
 * Ukraine regional Settings
 */

/**
 * Implements hook_ec_region_COUNTRYCODE_info().
 */
function ec_store_ec_region_ua_info() {
  return array(
    // Measures
    'weight' => 'KG',
    'length' => 'M',
    'area' => 'M2',
    'volume' => 'M3',
    // Geo
    'state_name' => t('State'), // the same format is used all over Ukraine
    'zip' => TRUE,
    // Price format
    'payment_symbol' => ' грн.', // I put a space before currency on purpose, since it must be padded with a space in the end, like "29.95 грн."
    'payment_symbol_position' => 2, // Right
    'payment_thousands' => ' ',
    'payment_decimal' => ',',
    'payment_decimal_places' => 2,
  );
}
