<?php

/**
 * @file
 * Lists module's rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_avangard_default_rules_configuration() {
  $rules = array();

  $rule = rules_reaction_rule();
  $rule->label = t("Refunds order paid by card, when it's canceled");
  $rule->active = TRUE;
  $rule->event('commerce_order_status_change')
          ->condition('data_is', array(
            'data:select' => 'commerce_order:status',
            'op' => '==',
            'value' => 'canceled',
          ))
          ->condition('commerce_payment_selected_payment_method', array(
            'commerce_order:select' => 'commerce-order',
            'method_id' => 'commerce_avangard',
          ))
          ->action('commerce_avangard_refund_order');
  $rules['commerce_avangard_refund_canceled_order'] = $rule;

  return $rules;
}
