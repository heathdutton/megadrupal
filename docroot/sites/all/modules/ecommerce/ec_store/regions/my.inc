<?php

/**
 * @file
 * MY regional Settings
 */

/**
 * Implements hook_ec_region_COUNTRYCODE_info().
 */
function ec_store_ec_region_my_info() {
  return array(
    'states' => array(
      'JH' => t('Johor'),
      'KD' => t('Kedah'),
      'KN' => t('Kelantan'),
      'KL' => t('Kuala Lumpur'),
      'ML' => t('Malacca'),
      'NS' => t('Negeri Sembilan'),
      'PH' => t('Pahang'),
      'PK' => t('Perak'),
      'PS' => t('Perlis'),
      'PG' => t('Penang'),
      'WP' => t('Wilayah Persekutuan'),
      'SL' => t('Selangor'),
      'TR' => t('Terengganu'),
      'LB' => t('Labuan'),
      'SB' => t('Sabah'),
      'SR' => t('Sarawak'),
    ),
  );
}
