<?php

/**
 * @file
 * Licensed under the GNU GPLv2 License
 */

// ======================================
// CONDITIONS
// ======================================
/*
 * Conditions:
 * + Check if the balance is greater than X
 */

/**
 * Implements hook_rules_condition_info().
 */
function ms_marketplace_rules_condition_info() {
  return array(
    'ms_marketplace_condition_balance_greater_than' => array(
      'label' => t('Check if affiliate balance is greater than X.'),
      'parameter' => array(
        'account' => array('type' => 'user', 'label' => t('Seller')),
        'amount' => array('type' => 'decimal', 'label' => t('Amount'))
      ),
      'group' => 'Marketplace Suite',
    ),
  );
}

/**
 * Condition: Check if the balance is greater than X
 */
function ms_marketplace_condition_balance_greater_than($account, $amount) {
  return $account->af_balance > $amount;
}

// ======================================
// ACTIONS
// ======================================

/*
 * Actions:
 * + Add an amount to the balance
 * + Calculate the correct commission
 */

/**
 * Implements hook_rules_action_info().
 * @ingroup rules
 */
function ms_marketplace_rules_action_info() {
  return array(
    'ms_marketplace_action_add_balance' => array(
      'label' => t('Add or subtract an amount to the balance'),
      'parameter' => array(
        'account' => array('type' => 'user', 'label' => t('Seller')),
        'order' => array('type' => 'ms_order', 'label' => t('Order', array(), array('context' => 'ms_core'))),
        'product' => array('type' => 'ms_product', 'label' => t('Product')),
        'amount' => array('type' => 'decimal', 'label' => t('Amount')),
        ),
      'group' => 'Marketplace Suite',
    ),
    'ms_marketplace_action_calculate_commission' => array(
      'label' => t('Calculate the commission for a sale'),
      'parameter' => array(
        'account' => array('type' => 'user', 'label' => t('Seller')),
        'order' => array('type' => 'ms_order', 'label' => t('Order', array(), array('context' => 'ms_core'))),
        'product' => array('type' => 'ms_product', 'label' => t('Product')),
      ),
      'provides' => array(
        'commission' => array(
          'type' => 'decimal',
          'label' => t('Commission'),
        ),
      ),
      'group' => 'Marketplace Suite',
    ),
  );
}

/**
* Action: Calculate the commission for a sale
*/
function ms_marketplace_action_calculate_commission($account, $order, $product) {
  $amount = ms_marketplace_get_product_adjusted_price($order, $product);
  $commission = ms_marketplace_calculate_commission($amount, $account, $product->module);
  ms_core_debug(t('The calculated commission for @name is @amount',
    array('@name' => $account->name, '@amount' => $commission)), FALSE);
  return array('commission' => $commission);
}

/**
 * Action: Add an amount to the balance
 */
function ms_marketplace_action_add_balance($account, $order, $product, $amount) {
  return ms_marketplace_grant_commission($account->uid, $order, $product, $amount);
}

// ======================================
// TRIGGERS
// ======================================

/*
 * Triggers:
 * + A new sale has been made
 * + Commission has been paid
 */

/**
 * Implements hook_rules_event_info()
 */
function ms_marketplace_rules_event_info() {
  $items = array(
    'ms_marketplace_event_new_sale' => array(
      'label' => t('New Sale Completed'),
      'group' => 'Marketplace Suite',
      'help' => t("Act when a new sale has been completed"),
      'variables' => array(
        'payment' => array('type' => 'ms_payment', 'label' => t('Payment')),
        'product' => array('type' => 'ms_product', 'label' => t('Product')),
        'order' => array('type' => 'ms_order', 'label' => t('Order', array(), array('context' => 'ms_core'))),
        'account' => array('type' => 'user', 'label' => t('Seller')),
      ),
    ),
    'ms_marketplace_event_commission_paid' => array(
      'label' => t('Commission Paid'),
      'group' => 'Marketplace Suite',
      'help' => t("Act when commission has been sent to a seller"),
      'variables' => array(
        'commission' => array('type' => 'decimal', 'label' => t('Commission Amount')),
        'product' => array('type' => 'ms_product', 'label' => t('Product')),
        'order' => array('type' => 'ms_order', 'label' => t('Order', array(), array('context' => 'ms_core'))),
        'account' => array('type' => 'user', 'label' => t('Seller')),
      ),
    ),
  );

  return $items;
}


// ======================================
// DATA TYPES
// ======================================

