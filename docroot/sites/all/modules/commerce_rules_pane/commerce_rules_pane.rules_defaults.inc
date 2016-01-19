<?php

/**
 * @file
 * Default rule configurations for Commerce Rules Pane.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_rules_pane_default_rules_configuration() {

  $rules = array();

  // Add a reaction rule to display a custom message in the checkout
  // process.
  $rule = rules_reaction_rule();

  $rule->label = t('Display a custom checkout message');
  $rule->active = TRUE;

  $rule
    ->event('determine_rules_checkout_pane_content')
    ->action('commerce_rules_pane_set_message', array(
      'commerce_order:select' => 'commerce-order',
      'content' => t('Thank you for your order. The state of your order is [commerce-order:state]. The id of your order is [commerce-order:order-id].'),
    ));

  $rules['commerce_rules_pane_show_message'] = $rule;

  return $rules;
}
