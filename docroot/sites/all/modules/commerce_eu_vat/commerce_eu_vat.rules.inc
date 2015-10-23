<?php
/**
 * @file
 * commerce_eu_vat.rules.inc
 */

/**
 * Implements hook_rules_condition_info().
 */
function commerce_eu_vat_rules_condition_info() {

  $conditions = array();

  $conditions['commerce_eu_vat_rules_site_is_in_eu'] = array(
    'label' => t("The site's default country is in the EU"),
    'group' => t('Commerce EU VAT'),
    'callbacks' => array(
      'execute' => 'commerce_eu_vat_rules_site_is_in_eu',
    ),
  );

  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_eu_vat_rules_action_info() {
  $actions = array();

  $actions['commerce_eu_vat_calculate'] = array(
    'label' => t('Calculate EU VAT EC Supply for a line item'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
      'country' => array(
        'type' => 'text',
        'label' => t('ISO 2 Country Code'),
      ),
    ),
    'group' => t('Commerce VAT'),
    'callbacks' => array(
      'execute' => 'commerce_eu_vat_rules_calculate',
    ),
  );

  return $actions;
}

/**
 * Callback function to check if the sites default country is in the EU.
 */
function commerce_eu_vat_rules_site_is_in_eu() {

  $default_country = variable_get('site_default_country', NULL);
  $eu_countries = _commerce_eu_vat_eu_countries();

  if (array_key_exists($default_country, $eu_countries)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Rules action: checks for the application of each vat rate of a certain type.
 */
function commerce_eu_vat_rules_calculate($line_item, $iso2) {
  commerce_vat_calculate_rates($line_item, $iso2);
}
