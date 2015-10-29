<?php

/**
 * @file
 * GE regional Settings
 */

/**
 * Implements hook_ec_region_COUNTRYCODE_info().
 */
function ec_store_ec_region_ge_info() {
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
    'states' => array(
      'AB' => t('Abkhazia'),
      'AJ' => t('Ajaria'),
      'TB' => t('Tbilisi'),
      'GU' => t('Guria'),
      'IM' => t('Imereti'),
      'KA' => t('Kakheti'),
      'KK' => t('Kvemo Kartli'),
      'MM' => t('Mtskheta-Mtianeti'),
      'RL' => t('Racha Lechkhumi and Kvemo Svaneti'),
      'SZ' => t('Samegrelo-Zemo Svaneti'),
      'SJ' => t('Samtskhe-Javakheti'),
      'SK' => t('Shida Kartli'),
    ),
    // Price format
    'payment_symbol' => '$',
    'payment_symbol_position' => 1, // Left
    'payment_thousands' => ',',
    'payment_decimal' => '.',
    'payment_decimal_places' => 2,
  );
}
