<?php
/**
 * @file
 * Commerce EU VAT Intra-Community Supply Rules
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_eu_vat_ic_default_rules_configuration() {
  // Create a new product pricing rule.
  $rule = rules_reaction_rule();

  $rule->label = t('Calculate EC Reverse Charge VAT');
  $rule->tags = array('Commerce VAT');
  $rule->active = TRUE;

  $rule->event('commerce_product_calculate_sell_price');

  foreach (commerce_vat_countries() as $name => $vat_country) {
    $country_rates = commerce_vat_country_rates($vat_country['iso2']);

    if (!empty($vat_country['rules_component']) && !empty($country_rates)) {
      $rule->condition(rules_condition('component_' . $vat_country['rules_component_place'], array('line_item:select' => 'commerce-line-item'))->negate());
    }
  }

  $rule->condition('component_commerce_eu_vat_place_of_supply_eu', array('commerce_order:select' => 'commerce-line-item:order'));
  $rule->condition('component_commerce_eu_vat_rc_order_has_vat_number', array('commerce_order:select' => 'commerce-line-item:order'));

  $default_country = variable_get('site_default_country', NULL);

  $rule->action('commerce_vat_rate_apply', array(
    'commerce_line_item:select' => 'commerce-line-item',
    'vat_rate_name' => 'ec_ic_rc',
  ));

  $rules['commerce_eu_vat_ic_calculate'] = $rule;

  return $rules;
}

/**
 * Impliments hook_default_rules_configuration_alter().
 */
function commerce_eu_vat_ic_default_rules_configuration_alter(&$configs) {
  if (isset($configs['commerce_eu_vat_calculate'])) {
    $configs['commerce_eu_vat_calculate']
      ->condition(rules_condition('component_commerce_eu_vat_rc_order_has_vat_number', array('commerce_order:select' => 'commerce-line-item:order'))->negate());
  }
}
