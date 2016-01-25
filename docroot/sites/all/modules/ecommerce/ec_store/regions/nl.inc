<?php

/**
 * @file
 * NL regional Settings
 */

/**
 * Implements hook_ec_region_COUNTRYCODE_info().
 */
function ec_store_ec_region_nl_info() {
  return array(
    'states' => array(
      'DR' => t('Drenthe'),
      'FL' => t('Flevoland'),
      'FR' => t('Friesland'),
      'GE' => t('Gelderland'),
      'GR' => t('Groningen'),
      'LI' => t('Limburg'),
      'NB' => t('Noord Brabant'),
      'NH' => t('Noord Holland'),
      'OV' => t('Overijssel'),
      'UT' => t('Utrecht'),
      'ZE' => t('Zeeland'),
      'ZH' => t('Zuid Holland'),
    ),
  );
}
