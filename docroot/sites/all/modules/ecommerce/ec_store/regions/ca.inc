<?php

/**
 * @file
 */

/**
 * Implements hook_ec_region_COUNTRYCODE_info().
 */
function ec_store_ec_region_ca_info() {
  return array(
    // Measures
    'weight' => 'KG',
    'length' => 'M',
    'area' => 'M2',
    'volume' => 'M3',
    // Geo
    'state_name' => t('Province'),
    'use_state_names' => TRUE,
    'zip' => TRUE,
    // Price format
    'payment_symbol' => '$',
    'payment_symbol_position' => 1, // Left
    'payment_thousands' => ',',
    'payment_decimal' => '.',
    'payment_decimal_places' => 2,
    'states' => array(
        'AB' => t('Alberta'),
        'BC' => t('British Columbia'),
        'MB' => t('Manitoba'),
        'NS' => t('Nova Scotia'),
        'NB' => t('New Brunswick'),
        'NL' => t('Newfoundland and Labrador'),
        'ON' => t('Ontario'),
        'PE' => t('Prince Edward Island'),
        'QC' => t('Quebec'),
        'SK' => t('Saskatchewan'),
    ),
  );
}
