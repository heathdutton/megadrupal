<?php

/**
 * @file
 * Default rule configuration for Commerce Bank Account on File recurring.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_baof_recurring_default_rules_configuration() {
  $rules = array();

  $rule = rules_reaction_rule();
  $rule->label = 'Charge pending recurring orders for (Bank Account on File)';
  $rule->requires = array(
    'rules',
    'commerce_bank_account_onfile',
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
  $loop->action('commerce_bank_account_onfile_order_select_bank_account', array(
    'order:select' => 'order',
  ));
  $loop->action('commerce_bank_account_onfile_order_charge_bank_account', array(
    'order:select' => 'order',
    'charge:select' => 'order:commerce-order-total',
    'select_bank_account_response:select' => 'select-bank-account-response',
    'bank_account_data:select' => '',
  ));
  $rule->action($loop);
  $rules['charge_pending_recurring_orders_for_bank_account_onfile'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = 'Successful recurring order bank account charge';
  $rule->requires = array(
    'rules',
    'commerce_bank_account_onfile',
    'commerce_order',
  );
  $rule->event('commerce_bank_account_onfile_charge_success');
  $rule->condition('data_is', array(
    'data:select' => 'order:status',
    'value' => 'recurring_pending',
  ));
  $rule->action('commerce_order_update_status', array(
    'commerce_order:select' => 'order',
    'order_status' => 'bank_account_onfile_charged',
  ));
  $rules['recurring_order_bank_account_charged'] = $rule;

  return $rules;
}
