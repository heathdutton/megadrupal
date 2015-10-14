<?php
/**
 * @file
 * Default rule configurations for Commerce eWay Multi.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_eway_multi_default_rules_configuration() {

  $rules = array();

  // Create a new reaction rule.
  $rule = rules_reaction_rule();

  $rule->label = t('Switch eWay Customer Id');
  $rule->active = FALSE;

  $rule
    ->event('commerce_eway_multi_before_payment')
    ->action('commerce_eway_multi_change_customer_id', array('new_customer_id' => 'Change to eWay Id.'));

  // Set our rule.
  $rules['commerce_eway_multi_change_customer_id'] = $rule;

  return $rules;
}
