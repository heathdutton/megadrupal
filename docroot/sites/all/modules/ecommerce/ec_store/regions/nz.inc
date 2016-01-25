<?php

/**
 * @file
 * New Zealand regional Settings
 */

/**
 * Implements hook_ec_region_COUNTRYCODE_info().
 */
function ec_store_ec_region_nz_info() {
  return array(
    // Geo
    'state_name' => t('Province'),
    'use_state_names' => TRUE,
    'zip' => TRUE,
    'zip_name' => t('Postcode'),
  );
}