<?php
/**
 * @file
 * commerce_eu_vat.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_eu_vat_default_rules_configuration() {

  // French Profile Address Conditions.
  $rule = rules_and(array(
    'customer_profile' => array(
      'label' => t('Customer Profile'),
      'type' => 'commerce_customer_profile',
    ),
  ));

  $rule->tags = array('Commerce VAT', 'Profile Address');

  $rule->label = t('Profile Address is in FR or MC');

  $rule->condition('entity_has_field', array('entity:select' => 'customer-profile', 'field' => 'commerce_customer_address'));
  $rule->condition(rules_condition('data_is_empty', array('data:select' => 'customer-profile:commerce-customer-address'))->negate());

  $rule
  ->condition('data_is', array(
    'data:select' => 'customer-profile:commerce-customer-address:country',
    'op' => 'IN',
    'value' => array('FR', 'MC'),
  ));

  $rules['commerce_vat_profile_address_fr'] = $rule;


  // Great Britain Profile Address Conditions.
  $rule = rules_and(array(
    'customer_profile' => array(
      'label' => t('Customer Profile'),
      'type' => 'commerce_customer_profile',
    ),
  ));
  $rule->tags = array('Commerce VAT', 'Profile Address');

  $rule->label = t('Profile Address is in GB or IM');

  $rule->condition('entity_has_field', array('entity:select' => 'customer-profile', 'field' => 'commerce_customer_address'));
  $rule->condition(rules_condition('data_is_empty', array('data:select' => 'customer-profile:commerce-customer-address'))->negate());

  $rule
  ->condition('data_is', array(
    'data:select' => 'customer-profile:commerce-customer-address:country',
    'op' => 'IN',
    'value' => array('GB', 'IM'),
  ));

  $rules['commerce_vat_profile_address_gb'] = $rule;

  // EU Profile Address Conditions.
  $rule = rules_or(array(
    'customer_profile' => array(
      'label' => t('Customer Profile'),
      'type' => 'commerce_customer_profile',
    ),
  ));
  $rule->label = t('Profile Address is in EU');
  $rule->tags = array('EU VAT', 'Place of Supply');

  $rule
  ->condition('component_commerce_vat_profile_address_at', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_be', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_bg', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_cy', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_cz', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_de', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_dk', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_ee', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_es', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_fi', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_fr', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_gb', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_gr', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_hu', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_ie', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_it', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_lt', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_lu', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_lv', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_mt', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_nl', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_pl', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_pt', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_ro', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_se', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_si', array('customer_profile:select' => 'customer-profile'))
  ->condition('component_commerce_vat_profile_address_sk', array('customer_profile:select' => 'customer-profile'));

  $rules['commerce_eu_vat_profile_address_eu'] = $rule;

  // EU Place of Supply Rule.
  $rule = rules_or(array(
    'commerce_order' => array(
      'label' => t('Commerce Order'),
      'type' => 'commerce_order',
    ),
  ));
  $rule->label = t('Place of Supply is in EU');
  $rule->tags = array('Commerce VAT', 'Place of Supply');

  $billing_rule = rules_and()
  ->condition('entity_has_field', array('entity:select' => 'commerce-order', 'field' => 'commerce_customer_billing'))
  ->condition(rules_condition('data_is_empty', array('data:select' => 'commerce-order:commerce-customer-billing'))->negate())
  ->condition('component_commerce_eu_vat_profile_address_eu', array('customer_profile:select' => 'commerce-order:commerce-customer-billing'));

  if (module_exists('commerce_shipping')) {
    $shipping_rule = rules_and()
    ->condition('entity_has_field', array('entity:select' => 'commerce-order', 'field' => 'commerce_customer_shipping'))
    ->condition(rules_condition('data_is_empty', array('data:select' => 'commerce-order:commerce-customer-shipping'))->negate())
    ->condition('component_commerce_eu_vat_profile_address_eu', array('customer_profile:select' => 'commerce-order:commerce-customer-shipping'));

    $rule->condition($shipping_rule);

    $billing_rule
    ->condition('entity_has_field', array('entity:select' => 'commerce-order', 'field' => 'commerce_customer_shipping'))
    ->condition('data_is_empty', array('data:select' => 'commerce-order:commerce-customer-shipping'));
  }

  $rule->condition($billing_rule);

  $rules['commerce_eu_vat_place_of_supply_eu'] = $rule;

  // Create a new product pricing rule.
  $rule = rules_reaction_rule();

  $rule->label = t('Calculate EC Supply VAT');
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

  $default_country = variable_get('site_default_country', NULL);

  $rule->action('commerce_eu_vat_calculate', array(
    'commerce_line_item:select' => 'commerce-line-item',
    'country' => $default_country,
  ));

  $rules['commerce_eu_vat_calculate'] = $rule;

  return $rules;
}

/**
 * Implimentation of hook_default_rules_configuration_alter().
 */
function commerce_eu_vat_default_rules_configuration_alter(&$configs) {

  // Exclude Heligoland (27498) and B�singen (78266).
  if (isset($configs['component_commerce_vat_profile_address_de'])) {
    $configs['component_commerce_vat_profile_address_de']->condition(rules_condition('data_is', array(
      'data:select' => 'customer-profile:commerce-customer-address:postal-code',
      'op' => 'IN',
      'value' => array('27498','78266'),
    ))->negate());
  }

  // Place of Supply = ES.
  // Exclude Canary Islands.
  if (isset($configs['component_commerce_vat_profile_address_es'])) {
    $configs['component_commerce_vat_profile_address_es']->condition(rules_condition('text_matches', array(
      'text:select' => 'customer-profile:commerce-customer-address:postal-code',
      'operation' => 'regex',
      'match' => '(35|38)[0-9]{3}',
    ))->negate());
  }

  // Exclude Ceuta and Melilla.
  if (isset($configs['component_commerce_vat_profile_address_es'])) {
    $configs['component_commerce_vat_profile_address_es']->condition(rules_condition('text_matches', array(
      'text:select' => 'customer-profile:commerce-customer-address:postal-code',
      'operation' => 'regex',
      'match' => '(51|52)[0-9]{3}',
    ))->negate());
  }

  // Place of Supply = IT.
  // Exclude Livigno (23030), Campione d�Italia and
  // the territorial waters of Lake Lugano (22060).
  if (isset($configs['component_commerce_vat_profile_address_it'])) {
    $configs['component_commerce_vat_profile_address_it']->condition(rules_condition('text_matches', array(
      'text:select' => 'customer-profile:commerce-customer-address:postal-code',
      'operation' => 'regex',
      'match' => '(23030|22060)',
    ))->negate());
  }
}

