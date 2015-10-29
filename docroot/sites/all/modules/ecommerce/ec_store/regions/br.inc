<?php

/**
 * @file
 */

/**
 * Implements hook_ec_region_COUNTRYCODE_info().
 */
function ec_store_ec_region_br_info() {
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
      'AC' => t('Acre'),
      'AL' => t('Alagoas'),
      'AP' => t('Amapá'),
      'AM' => t('Amazonas'),
      'BA' => t('Bahia'),
      'CE' => t('Ceará'),
      'DF' => t('Distrito Federal'),
      'ES' => t('Espírito Santo'),
      'GO' => t('Goiás'),
      'MA' => t('Maranhão'),
      'MT' => t('Mato Grosso'),
      'MS' => t('Mato Grosso do Sul'),
      'MG' => t('Minas Gerais'),
      'PA' => t('Pará'),
      'PB' => t('Paraíba'),
      'PR' => t('Paraná'),
      'PE' => t('Pernambuco'),
      'PI' => t('Piauí'),
      'RJ' => t('Rio de Janeiro'),
      'RN' => t('Rio Grande do Norte'),
      'RS' => t('Rio Grande do Sul'),
      'RO' => t('Rondônia'),
      'RR' => t('Roraima'),
      'SC' => t('Santa Catarina'),
      'SP' => t('São Paulo'),
      'SE' => t('Sergipe'),
      'TO' => t('Tocantins'),
    ),
    // Price format
    'payment_symbol' => 'R$',
    'payment_symbol_position' => 1, // Left
    'payment_thousands' => '.',
    'payment_decimal' => ',',
    'payment_decimal_places' => 2,
  );
}
