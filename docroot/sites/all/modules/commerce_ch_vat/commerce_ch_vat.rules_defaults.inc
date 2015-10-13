<?php
/**
 * @file
 * commerce_ch_vat.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_ch_vat_default_rules_configuration() {
  // French Profile Address Conditions.
  $rule = rules_and(array(
    'customer_profile' => array(
      'label' => t('Customer Profile'),
      'type' => 'commerce_customer_profile',
    ),
  ));

  $rule->tags = array('Commerce VAT', 'Profile Address');

  $rule->label = t('Profile Address is in CH');

  $rule->condition('entity_has_field', array('entity:select' => 'customer-profile', 'field' => 'commerce_customer_address'));
  $rule->condition(rules_condition('data_is_empty', array('data:select' => 'customer-profile:commerce-customer-address'))->negate());

  $rule_ch = rules_and()
  ->condition('data_is', array(
    'data:select' => 'customer-profile:commerce-customer-address:country',
    'op' => 'IN',
    'value' => array('CH'),
  ));

  $rule_it = rules_and()
  ->condition('data_is', array(
    'data:select' => 'customer-profile:commerce-customer-address:country',
    'op' => 'IN',
    'value' => array('IT'),
  ))
  ->condition('text_matches', array(
    'text:select' => 'customer-profile:commerce-customer-address:postal-code',
    'operation' => 'regex',
    'match' => '(22060)',
  ));

  $rule_de = rules_and()
  ->condition('data_is', array(
    'data:select' => 'customer-profile:commerce-customer-address:country',
    'op' => 'IN',
    'value' => array('DE'),
  ))
  ->condition('text_matches', array(
    'text:select' => 'customer-profile:commerce-customer-address:postal-code',
    'op' => 'regex',
    'match' => '(78266)',
  ));

  $rule->condition(rules_or()
  ->condition($rule_ch)
  ->condition($rule_it)
  ->condition($rule_de)
  );

  $rules['commerce_vat_profile_address_ch'] = $rule;

  return $rules;
}