<?php

/**
 * @file
 * Defines default vat components and rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_vat_default_rules_configuration() {
  $rules = array();

  $vat_countries = commerce_vat_countries();
  // Loop over every vat rate looking for rates requiring a default component
  // that specify a a component name.
  foreach (commerce_vat_rates() as $name => $vat_rate) {
    if ($vat_rate['default_rules_component'] && !empty($vat_rate['rules_component'])) {
      // Define a default rules component for applying this vat rate to a line
      // item using the name specified by the rate.
      $rule = rule(array(
        'line_item' => array(
          'type' => 'commerce_line_item',
          'label' => t('Line item'),
        ),
      ));
      $rule->label = t('Calculate @title', array('@title' => $vat_rate['title']));
      $rule->tags = array('Commerce VAT');

      // Add the action to apply the current vat.
      $rule
      ->action('commerce_vat_rate_apply', array(
        'commerce_line_item:select' => 'line-item',
        'vat_rate_name' => $name,
      ));

      $rules[$vat_rate['rules_component']] = $rule;
    }
  }

  // Loop over every vat rate looking for rates requiring a default component
  // that specify a a component name.
  foreach ($vat_countries as $iso2 => $vat_country) {
    if ($vat_country['default_profile_rules_component'] && !empty($vat_country['rules_component_profile'])) {

      // Define a default rules component for checking the
      // profile address.
      $rule = rules_and(array(
        'customer_profile' => array(
          'label' => t('Customer Profile'),
          'type' => 'commerce_customer_profile',
        ),
      ));
      $rule->tags = array('Commerce VAT', 'Profile Address');

      $rule->label = t('Profile Address is in @title', array('@title' => $vat_country['title']));

      $rule->condition('entity_has_field', array('entity:select' => 'customer-profile', 'field' => 'commerce_customer_address'));
      $rule->condition(rules_condition('data_is_empty', array('data:select' => 'customer-profile:commerce-customer-address'))->negate());

      // Add the action to apply the current vat.
      $rule
      ->condition('data_is', array(
        'data:select' => 'customer-profile:commerce-customer-address:country',
        'op' => 'IN',
        'value' => array($iso2),
      ));

      $rules[$vat_country['rules_component_profile']] = $rule;

    }

    if ($vat_country['default_place_rules_component'] && !empty($vat_country['rules_component_place'])) {
      // Define a default rules component for checking the
      // place of supply for a country.
      $rule = rules_or(array(
        'line_item' => array(
          'label' => t('Commerce Line Item'),
          'type' => 'commerce_line_item',
        ),
      ));
      $rule->label = t('Place of Supply is in @title', array('@title' => $vat_country['title']));
      $rule->tags = array('Commerce VAT', 'Place of Supply');

      $rule_billing = rules_and()
      ->condition('entity_has_field', array('entity:select' => 'line-item:order', 'field' => 'commerce_customer_billing'))
      ->condition(rules_condition('data_is_empty', array('data:select' => 'line-item:order:commerce-customer-billing'))->negate())
      ->condition('component_' . $vat_country['rules_component_profile'], array('customer_profile:select' => 'line-item:order:commerce-customer-billing'));

      if (module_exists('commerce_shipping')) {
        $rule_shipping = rules_and()
        ->condition('entity_has_field', array('entity:select' => 'line-item:order', 'field' => 'commerce_customer_shipping'))
        ->condition(rules_condition('data_is_empty', array('data:select' => 'line-item:order:commerce-customer-shipping'))->negate())
        ->condition('component_' . $vat_country['rules_component_profile'], array('customer_profile:select' => 'line-item:order:commerce-customer-shipping'));

        $rule->condition($rule_shipping);

        $rule_billing
        ->condition('entity_has_field', array('entity:select' => 'line-item:order', 'field' => 'commerce_customer_shipping'))
        ->condition('data_is_empty', array('data:select' => 'line-item:order:commerce-customer-shipping'));
      }

      $rule->condition($rule_billing);

      if (variable_get('site_default_country', NULL) == $iso2) {
        $rule_default_country = rules_and()
        ->condition('entity_has_field', array('entity:select' => 'line-item:order', 'field' => 'commerce_customer_billing'))
        ->condition('data_is_empty', array('data:select' => 'line-item:order:commerce-customer-billing'));

        if (module_exists('commerce_shipping')) {
          $rule_default_country
          ->condition('entity_has_field', array('entity:select' => 'line-item:order', 'field' => 'commerce_customer_shipping'))
          ->condition('data_is_empty', array('data:select' => 'line-item:order:commerce-customer-shipping'));
        }

        $rule->condition($rule_default_country);
      }

      $rules[$vat_country['rules_component_place']] = $rule;
    }

    $country_rates = commerce_vat_country_rates($vat_country['iso2']);

    if (!empty($country_rates) && $vat_country['default_rules_component'] && !empty($vat_country['rules_component'])) {
      // Define a default rules component for calculating the VAT.
      $rule = rule(array(
        'line_item' => array(
          'label' => t('Commerce Line Item'),
          'type' => 'commerce_line_item',
        ),
      ));
      $rule->label = t('Calculate @title VAT', array('@title' => $vat_country['title']));
      $rule->tags = array('Commerce VAT');

      $rule->condition('component_' . $vat_country['rules_component_place'], array('commerce_line_item:select' => 'line-item'));

      $rule->action('commerce_vat_calculate', array(
        'commerce_line_item:select' => 'line-item',
        'country' => $iso2,
      ));

      $rules[$vat_country['rules_component']] = $rule;
    }
  }

  // Create a new product pricing rule.
  $rule = rules_reaction_rule();

  $rule->label = t('Calculate VAT');
  $rule->tags = array('Commerce VAT');
  $rule->weight = 10;
  $rule->active = TRUE;

  // Add the action to invoke every vat rate's component matching this type.
  $rule
  ->event('commerce_product_calculate_sell_price')
  ->action('commerce_vat_place_of_supply', array(
    'commerce_line_item:select' => 'commerce-line-item',
  ));

  $rules['commerce_vat_calculate'] = $rule;

  if (module_exists('commerce_shipping')) {
    // Create a new shipping pricing rule.
    $rule = rules_reaction_rule();

    $rule->label = t('Calculate Shipping VAT');
    $rule->tags = array('Commerce VAT', 'Commerce Shipping');
    $rule->active = TRUE;

    // Add the action to invoke every vat rate's component matching this type.
    $rule
    ->event('commerce_shipping_calculate_rate')
    ->action('commerce_vat_order_rate', array(
      'commerce_order:select' => 'commerce-line-item:order',
    ))
    ->action('commerce_vat_rate_apply', array(
      'commerce_line_item:select' => 'commerce_line_item',
      'vat_rate_name:select' => 'order-vat-rate',
    ));

    $rules['commerce_vat_calculate_shipping'] = $rule;
  }

  return $rules;
}
