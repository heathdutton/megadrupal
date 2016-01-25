<?php

/**
 * @file
 * Implement default rules.
 */

/**
 * Implements hook_rules_defaults().
 */
 function ec_store_default_rules_configuration() {
   $configs = array();
   
   $action_set = rules_action_set(array('txn' => array('type' => 'ec_transaction', 'label' => t('ec_transaction'))));
   $action_set->label = 'Complete shipped transaction';
   $action_set->action('ec_store_rules_action_set_workflow', array('txn:select' => 'txn', 'status' => array(6)));
   
   $configs['ec_complete_shipped_transactions'] = $action_set;
   
   $rule = rules_reaction_rule();
   $rule->label = 'Mark zero price invoices as allocated';
   $rule->event('ec_store_event_transactions_bef_save')
     ->condition('ec_store_condition_gross', array('txn:select' => 'txn', 'condition' => array('='), 'amount' => 0))
     ->condition('ec_store_condition_allocation_status', array('txn:select' => 'txn', 'status' => array(0)))
     ->action('ec_store_rules_action_set_allocation', array('txn:select' => 'txn', 'status' => array(2)));

   $configs['ec_store_mark_zero_price_invoices_as_allocated'] = $rule;
   
   $rule = rules_reaction_rule();
   $rule->label = 'Transaction marked as shipped';
   $rule->event('ec_store_event_transactions_save')
     ->condition('ec_store_condition_workflow_status_changed', array('txn:select' => 'txn', 'orig_txn:select' => 'orig_txn'))
     ->condition('ec_store_condition_workflow_status', array('txn:select' => 'txn', 'status' => array('3')))
     ->action('schedule', array(
       'component' => 'ec_complete_shipped_transactions',
       'date' => '+6 hours',
       'identifier' => 'ec_transaction_shipped_[txn:txn-id]',
       'param_txn:select' => 'txn',
     ));
   
   $configs['ec_store_transaction_marked_as_shipped'] = $rule;
   
   $rule = rules_reaction_rule();
   $rule->label = 'Revert allocation to received';
   $rule->event('ec_store_event_transactions_bef_save')
     ->condition('ec_store_condition_allocation_status_changed', array('txn:select' => 'txn', 'orig_txn:select' => 'orig_txn'))
     ->condition('ec_store_condition_allocation_status', array('txn:select' => 'orig_txn', 'status' => array('2')))
     ->condition('ec_store_condition_allocation_status', array('txn:select' => 'txn', 'status' => array('2')))
     ->action('ec_store_rules_action_set_workflow', array('txn:select' => 'txn', 'status' => array(1)));
   
   $configs['ec_store_revert_allocation_to_received'] = $rule;
   
   $rule = rules_reaction_rule();
   $rule->label = 'Set non-shippable transaction as complete';
   $rule->event('ec_store_event_transactions_bef_save')
     ->condition(rules_condition('ec_store_condition_is_shippable', array('txn:select' => 'txn'))->negate())
     ->condition('ec_store_condition_allocation_status_changed', array('txn:select' => 'txn', 'orin_txn:select' => 'orig_txn'))
     ->condition('ec_store_condition_allocation_status', array('txn:select' => 'txn', 'status' => array(2)))
     ->condition('ec_store_condition_workflow_status', array('txn:select' => 'txn', 'status' => array(1, 2)))
     ->action('ec_store_rules_action_set_workflow', array('txn:select' => 'txn', 'status' => array(6)));
   
   $configs['ec_store_set_non_shippable_transaction_as_complete'] = $rule;
   
   $rule = rules_reaction_rule();
   $rule->label = 'Shippable orders now in picking';
   $rule->event('ec_store_event_transactions_bef_save')
     ->condition('ec_store_condition_is_shippable', array('txn:select' => 'txn'))
     ->condition('ec_store_condition_allocation_status_changed', array('txn:select' => 'txn', 'orin_txn:select' => 'orig_txn'))
     ->condition('ec_store_condition_allocation_status', array('txn:select' => 'txn', 'status' => array(2)))
     ->condition('ec_store_condition_workflow_status', array('txn:select' => 'txn', 'status' => array(1)))
     ->action('ec_store_rules_action_set_workflow', array('txn:select' => 'txn', 'status' => array(10)));
   
   $configs['ec_store_shippable_orders_now_in_picking'] = $rule;
   
   $rule = rules_reaction_rule();
   $rule->label = 'Mark shipped transaction as completed';
   $rule->event('ec_store_event_transactions_bef_save')
     ->condition('ec_store_condition_workflow_status_changed', array('txn:select' => 'txn', 'orig_txn:select' => 'orig_txn'))
     ->condition('ec_store_condition_workflow_status', array('txn:select' => 'txn', 'status' => array(3)))
     ->action('schedule', array(
       'component' => 'ec_complete_shipped_transactions',
       'date' => '+6 hours',
       'identifier' => 'ec_transaction_shipped_[txn:txnid]',
       'param_txn:select' => 'txn',
     ));
   
   $configs['ec_store_mark_shipped_txn_as_completed'] = $rule;
   return $configs;
 }