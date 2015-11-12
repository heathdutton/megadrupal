<?php

/**
 * @file
 * Create default Rules used for handling funds accounts and transactions.
 */

// @todo Change default Rules.

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_marketplace_funds_default_rules_configuration() {
  rules_clear_cache();
  // Manually trigger cache rebuilding of all caches.
  rules_get_cache();
  _rules_rebuild_component_cache();
  RulesEventSet::rebuildEventCache();
  rules_get_cache();

  $component = rule(array('order'=>array(
    'type' => 'commerce_order',
    'label' => t('Order'),
  )));
  $component->label = t('Create funds payment transaction for store order');
  $component->condition(rules_condition('data_is', array(
    'data:select' => 'order:type',
    'op' => '==',
    'value' => 'commerce_store_order',
  )));
  $component->action(rules_action('commerce_marketplace_funds_rules_account_load_by_owner', array(
    'owner_type' => 'commerce_store',
    'owner_id:select' => 'order:cmp-o-store:id',
    'funds_account:label' => t('Funds account'),
    'funds_account:var' => 'funds_account'
  )));
  $component->action(rules_action('commerce_marketplace_funds_rules_create_transaction', array(
    'transaction_type' => 'payment',
    'amount:select' => 'order:commerce-order-total:amount',
    'originator' => NULL,
    'destination:select' => 'funds-account',
    'order:select' => 'order',
    'notes' => 'Created for original order #[order:cmp_order:order_id] placed by [order:cmp_order:owner:name].',
    'status' => 'success',
    'transaction:label' => t('Transaction'),
    'transaction:var' => 'transaction',
  )));
  $component->action(rules_action('commerce_marketplace_funds_rules_apply_transaction_fees', array(
    'transaction:select' => 'transaction',
    'fee_type' => 'percent',
    'fee' => 0.05,
  )));
  $component->action(rules_action('entity_save', array(
    'data:select' => 'transaction',
    'immediate' => 1,
  )));
  $component->action(rules_action('commerce_marketplace_funds_rules_apply_transaction_to_accounts', array(
    'transaction:select' => 'transaction',
    'destination:select' => 'funds-account',
    'originator' => NULL,
  )));
  $component->action(rules_action('entity_save', array(
    'data:select' => 'funds-account',
    'immediate' => 1,
  )));
  $rules['rules_create_funds_payment_transaction_for_store_order'] = $component;

  $rule = new RulesReactionRule();
  $rule->label = 'Make payments to stores';
  $rule->active = TRUE;
  $rule->event('commerce_payment_order_paid_in_full');
  $rule->condition(rules_condition('data_is', array(
    'data:select' => 'commerce-order:type',
    'op' => '==',
    'value' => 'commerce_marketplace_order',
  )));
  $rule->action(rules_action('commerce_marketplace_order_load_store_orders_from_marketplace_order', array(
    'marketplace_order:select' => 'commerce-order',
    'store_orders:label' => t('Store orders'),
    'store_orders:var' => 'store_orders',
  )));
  $rule->action(rules_loop(array(
    'list:select' => 'store-orders',
    'item:var' => 'store_order',
    'item:label' => 'Store order',
    'item:type' => 'commerce_order',
  ))->action(rules_action('component_rules_create_funds_payment_transaction_for_store_order', array(
      'order:select' => 'store-order'
    ))));
  $rules['rules_make_payments_to_stores'] = $rule;

  $rule = new RulesReactionRule();
  $rule->label = 'Complete deposit process';
  $rule->active = TRUE;
  $rule->event('commerce_checkout_complete');
  $rule->condition(rules_condition('data_is', array(
    'data:select' => 'commerce-order:type',
    'op' => '==',
    'value' => 'cmp_funds_deposit_order',
  )));
  $rule->action(rules_action('commerce_marketplace_funds_load_deposit_transaction_from_order', array(
    'order:select' => 'commerce-order',
    'funds_transaction:label' => t('Funds transaction'),
    'funds_transaction:var' => 'funds_transaction',
  )));
  $rule->action(rules_action('data_set', array(
    'data:select' => 'funds-transaction:status',
    'value' => 'success',
  )));
  $rule->action(rules_action('entity_save', array(
    'data:select' => 'funds-transaction',
    'immediate' => 1,
  )));
  $rule->action(rules_action('variable_add', array(
    'type' => 'cmp_funds_account',
    'value:select' => 'funds-transaction:destination-account',
    'variable_added:label' => t('Destination account'),
    'variable_added:var' => 'destination_account',
  )));
  $rule->action(rules_action('commerce_marketplace_funds_rules_apply_transaction_to_accounts', array(
    'transaction:select' => 'funds-transaction',
    'destination:select' => 'destination-account',
    'originator' => NULL,
  )));
  $rule->action(rules_action('entity_save', array(
    'data:select' => 'destination-account',
    'immediate' => 1,
  )));

  $rules['rules_complete_deposit_process'] = $rule;

  $rule = new RulesReactionRule();
  $rule->label = 'Apply Fees on deposit';
  $rule->active = TRUE;
  $rule->event('cmp_funds_transaction_insert');
  $rule->condition(rules_condition('entity_is_of_bundle', array(
    'entity:select' => 'cmp-funds-transaction',
    'type' => 'cmp_funds_transaction',
    'bundle' => 'deposit',
  )));
  $rule->action(rules_action('commerce_marketplace_funds_rules_apply_transaction_fees', array(
    'transaction:select' => 'cmp-funds-transaction',
    'fee_type' => 'percent',
    'fee' => '0.05',
  )));
  $rule->action(rules_action('entity_save', array(
    'data:select' => 'cmp-funds-transaction',
    'immediate' => 1,
  )));
  $rules['rules_apply_fees_on_deposit'] = $rule;

  return $rules;
}
