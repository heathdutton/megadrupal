<?php

/**
 * @file
 * Russian regional Settings
 */

/**
 * Implements hook_ec_region_COUNTRYCODE_info().
 */
function ec_store_ec_region_ru_info() {
  return array(
    // Measures
    'weight' => 'KG',
    'length' => 'M',
    'area' => 'M2',
    'volume' => 'M3',
    // Geo
    'state_name' => t('State'),
    'zip' => TRUE,
    // Price format
    'payment_symbol' => ' руб.', // I put a space before currency on purpose, since it must be padded with a space in the end, like "29.95 руб."
    'payment_symbol_position' => 2, // Right
    'payment_thousands' => ' ',
    'payment_decimal' => ',',
    'payment_decimal_places' => 2,
  );
}