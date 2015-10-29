<?php

/**
 * @file
 * French regional Settings
 */

/**
 * Implements hook_ec_region_COUNTRYCODE_info().
 */
function ec_store_ec_region_fr_info() {
  return array(
    // Measures
    'weight' => 'KG',
    'length' => 'M',
    'area' => 'M2',
    'volume' => 'M3',
    // Geo
    'state_name' => t('State'),
    'use_state_names' => TRUE,
    'zip' => TRUE,
    'states' => array(
      'A67' => t('Bas-Rhin - Alsace'),
      'A68' => t('Haut-Rhin - Alsace'),
      'B24' => t('Dordogne - Aquitaine'),
      'B33' => t('Gironde - Aquitaine'),
      'B40' => t('Landes - Aquitaine'),
      'B47' => t('Lot-et-Garonne - Aquitaine'),
      'B64' => t('Pyrenees-Atlantiques - Aquitaine'),
      'B79' => t('Deux-Sevres - Aquitaine'),
      'C03' => t('Allier - Auvergne'),
      'C15' => t('Cantal - Auvergne'),
      'C43' => t('Haute-Loire - Auvergne'),
      'C63' => t('Pu-de-Dme - Auvergne'),
      'D21' => t("Cote-d'Or - Bourgogne"),
      'D58' => t('Nievre - Bourgogne'),
      'D71' => t('Saone-et-Loire - Bourgogne'),
      'D89' => t('Yonne - Bourgogne'),
      'E22' => t("Cotes-d'Armor - Bretagne"),
      'E29' => t('Finistere - Bretagne'),
      'E35' => t('Ille-et-Vilaine - Bretagne'),
      'E56' => t('Morbihan - Bretagne'),
      'F18' => t('Cher - Centre'),
      'F28' => t('Eure-et-Loir - Centre'),
      'F36' => t('Indre - Centre'),
      'F37' => t('Indre-et-Loire - Centre'),
      'F41' => t('Loir-et-Cher - Centre'),
      'F45' => t('Loiret - Centre'),
      'G08' => t('Ardennes - Champagne-Ardenne'),
      'G10' => t('Aube - Champagne-Ardenne'),
      'G51' => t('Marne - Champagne-Ardenne'),
      'G52' => t('Haute-Marne - Champagne-Ardenne'),
      'H2A' => t('Corse-du-Sud - Corse'),
      'H2B' => t('Haute-Corse - Corse'),
      'I25' => t('Doubs - Franche-Comte'),
      'I39' => t('Jura - Franche-Comte'),
      'I70' => t('Haute-Saone - Franche-Comte'),
      'I90' => t('Haute-Saone - Territoire de Belfort'),
      'J75' => t('Paris - Ile-de-France'),
      'J77' => t('Seine-et-Marne - Ile-de-France'),
      'J78' => t('Yvelines - Ile-de-France'),
      'J91' => t('Essonne - Ile-de-France'),
      'J92' => t('Hauts-de-Seine - Ile-de-France'),
      'J93' => t('Seine-Saint-Denis - Ile-de-France'),
      'J94' => t('Val-de-Marne - Ile-de-France'),
      'J95' => t("Val-d'Oise - Ile-de-France"),
      'U04' => t('Alpes-de-Haute-Provence'),
      'U05' => t('Hautes-Alpes'),
      'U06' => t('Alpes-Maritimes'),
      'U13' => t('Bouches-du-Rhne'),
      'U83' => t('Var'),
      'U84' => t('Vaucluse'),
    ),
    // Price format
    'payment_symbol' => '$',
    'payment_symbol_position' => 1, // Left
    'payment_thousands' => ',',
    'payment_decimal' => '.',
    'payment_decimal_places' => 2,
  );
}
