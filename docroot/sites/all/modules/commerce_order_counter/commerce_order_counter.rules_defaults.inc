<?php

/**
 * @file   commerce_order_counter.rules_default.inc
 * @author Augustin Delaporte <guss.tissier@gmail.com>
 *         António P. P. Almeida <appa@perusio.net>
 * @date   Wed Mar 13 14:55:43 2013
 *
 * @brief Makes the rules for handling the commerce order counter available as
 *        defaults.
 *
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_order_counter_default_rules_configuration() {
  $rules = array();

  // Add a reaction so that whenever the checkout is completed we increment
  // the counter.
  $rule = rules_reaction_rule();

  $rule->label = t('Increment the order counter upon checkout completion');
  $rule->active = TRUE;

  // Associate the event with the action. When an checkout is completed so
  // that the order counter is incremented.
  $rule
    ->event('commerce_checkout_complete')
    ->action('commerce_order_counter_update_order_number', array(
               'commerce_order:select' => 'commerce-order',
               'order_number' => commerce_order_counter_order_get(),
             )
    );

  // Adjust the weight so that this rules runs last.
  $rule->weight = 10;

  // Install the rule.
  $rules['commerce_update_order_counter'] = $rule;

  return $rules;
} // commerce_order_counter_default_rules_configuration