<?php

/**
 * @file
 * Ireland Regional Settings
 */

/**
 * Implements hook_ec_region_COUNTRYCODE_info().
 */
function ec_store_ec_region_ie_info() {
  return array(
    // Measures
    'weight' => 'KG',
    'length' => 'M',
    'area' => 'M2',
    'volume' => 'M3',
    // Geo
    'state_name' => t('County'),
    'use_state_names' => TRUE,
    'zip' => FALSE, // No zip codes in Ireland (yet!)
    'states' => array(
      'AM' => t('Antrim'),
      'AH' => t('Armagh'),
      'C' => t('Cork'),
      'CE' => t('Clare'),
      'CN' => t('Cavan'),
      'CW' => t('Carlow'),
      'D' => t('Dublin'),
      'DL' => t('Donegal'),
      'DN' => t('Down'),
      'DY' => t('Derry'),
      'FH' => t('Fermanagh'),
      'GW' => t('Galway'),
      'KE' => t('Kildare'),
      'KK' => t('Kilkenny'),
      'KY' => t('Kerry'),
      'LD' => t('Longford'),
      'LH' => t('Louth'),
      'LK' => t('Limerick'),
      'LM' => t('Leitrim'),
      'LS' => t('Laois'),
      'MH' => t('Meath'),
      'MN' => t('Monaghan'),
      'MO' => t('Mayo'),
      'OY' => t('Offaly'),
      'RN' => t('Roscommon'),
      'SO' => t('Sligo'),
      'TN' => t('Tipperary North'),
      'TS' => t('Tipperary South'),
      'TE' => t('Tyrone'),
      'WD' => t('Waterford'),
      'WH' => t('Westmeath'),
      'WW' => t('Wicklow'),
      'WX' => t('Wexford'),
    ),
    // Price format
    'ec_payment_symbol' => '€',
    'ec_payment_symbol_position' => 1, // Left
    'ec_payment_thousands' => ',',
    'ec_payment_decimal' => '.',
    'ec_payment_decimal_places' => 2,
  );
}
