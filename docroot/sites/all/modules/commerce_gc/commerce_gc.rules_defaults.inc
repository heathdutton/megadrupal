<?php

/*
 * Implements hook_default_rules_configuration().
 */
function commerce_gc_default_rules_configuration() {
  $rules = array();
  
  // Authorize giftcard transactions on checkout complete.
  $rule = rules_reaction_rule();
  $rule->label = t('Authorize giftcard transactions');
  $rule->tags = array('Commerce Giftcard');
  
  $rule
    ->event('commerce_checkout_complete')
    ->action('commerce_gc_order_transaction_status_change', array(
      'commerce_order:select' => 'commerce_order',
      'statuses' => array(COMMERCE_GC_TRANSACTION_PENDING_STATUS),
      'target_status' => COMMERCE_GC_TRANSACTION_AUTHORIZED_STATUS
    ));
  
  $rules['authorize_giftcard_transactions'] = $rule;
  
  // Complete giftcard transactions on order paid in full.
  $rule = rules_reaction_rule();
  $rule->label = t('Complete giftcard transactions');
  $rule->tags = array('Commerce Giftcard');
  
  $rule
    ->event('commerce_payment_order_paid_in_full')
    ->action('commerce_gc_order_transaction_status_change', array(
      'commerce_order:select' => 'commerce_order',
      'statuses' => array(COMMERCE_GC_TRANSACTION_AUTHORIZED_STATUS, COMMERCE_GC_TRANSACTION_PENDING_STATUS),
      'target_status' => COMMERCE_GC_TRANSACTION_COMPLETE_STATUS
    ));
  
  $rules['complete_giftcard_transactions'] = $rule;
  
  return $rules;  
}
