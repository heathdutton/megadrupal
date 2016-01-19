<?php

/**
 * @file
 * Commerce surcharge default rule configurations.
 */

/**
 * Implements hook_default_rules_configuration().
 * Add a rule.
 */
function commerce_surcharge_default_rules_configuration() {
  $rules = array();
  $rule = rules_reaction_rule();
  $rule->label = t('Apply credit card surcharge to an order');
  $rule->tags = array('Commerce surcharge');
  $rule->active = variable_get('commerce_surcharge_enabled', FALSE);
  $rule
    ->event('commerce_surcharge_order')
    ->action('commerce_surcharge_apply', array(
      'entity:select' => 'commerce-order',
      'currency_code' => 'AUD',
    ));
  $rules['commerce_surcharge'] = $rule;
  return $rules;
}
