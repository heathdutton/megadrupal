<?php

/**
 * @file
 * Default rule configuration for Commerce Card on File recurring.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_cardonfile_recurring_default_rules_configuration() {
  $rules = array();

  $rule = rules_reaction_rule();
  $rule->label = 'Charge pending recurring orders';
  $rule->requires = array(
    'rules',
    'commerce_cardonfile',
    'commerce_recurring',
  );
  $rule->event('commerce_recurring_cron');
  $rule->action('entity_query', array(
    'type' => 'commerce_order',
    'property' => 'status',
    'value' => 'recurring_pending',
    'limit' => 5,
  ));
  $loop = rules_loop(array(
    'list:select' => 'entity-fetched',
    'item:var' => 'order',
    'item:label' => t('Current Order'),
  ));
  $loop->action('commerce_cardonfile_order_select_card', array(
    'order:select' => 'order',
  ));
  $loop->action('commerce_cardonfile_order_charge_card', array(
    'order:select' => 'order',
    'charge:select' => 'order:commerce-order-total',
    'select_card_response:select' => 'select-card-response',
    'card_data:select' => '',
  ));
  $rule->action($loop);
  $rules['charge_pending_recurring_orders'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = 'Successful recurring order card charge';
  $rule->requires = array(
    'rules',
    'commerce_cardonfile',
    'commerce_order',
  );
  $rule->event('commerce_cardonfile_charge_success');
  $rule->condition('data_is', array(
    'data:select' => 'order:status',
    'value' => 'recurring_pending',
  ));
  $rule->action('commerce_order_update_status', array(
    'commerce_order:select' => 'order',
    'order_status' => 'cardonfile_charged',
  ));
  $rules['recurring_order_card_charged'] = $rule;

  return $rules;
}
