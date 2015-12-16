<?php


// ======================================
// ACTIONS
// ======================================

/*
 * Actions:
 * + Load Node from PTP Node Record
 * + Load PTP Node Record from Node
 */

/**
 * Implements hook_rules_action_info().
 * @ingroup rules
 */
function ms_paypublish_rules_action_info() {
  return array(
    'ms_paypublish_action_load_node' => array(
      'label' => t('Load the node for a Pay to Publish Node Record'),
      'parameter' => array(
        'purchase' => array('type' => 'ms_products_purchase', 'label' => t('Pay to Publish Node Record')),
      ),
      'provides' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
        ),
      ),
      'group' => 'Pay to Publish',
    ),
    'ms_paypublish_action_load_pp_node' => array(
      'label' => t('Load the Pay to Publish Node Record from a node'),
      'parameter' => array(
        'node' => array('type' => 'node', 'label' => t('Node')),
      ),
      'provides' => array(
        'purchase' => array(
          'type' => 'ms_products_purchase',
          'label' => t('Pay to Publish Node Record'),
        ),
      ),
      'group' => 'Pay to Publish',
    ),
  );
}

/**
 * Action: Load a node from a Pay to Publish Node
 */
function ms_paypublish_action_load_node($purchase) {
  $node = node_load($purchase->nid);
  return array('node' => $node);
}

/**
 * Action: Load a Pay to Publish Node from a Node
 */
function ms_paypublish_action_load_pp_node($node) {
  $purchase = ms_products_load_purchase_by_field('ms_paypublish', 'nid', $node->nid);
  return array('purchase' => $purchase);
}

// ======================================
// TRIGGERS
// ======================================

/*
 * Triggers:
 * + Pay to Publish Node is purchased
 * + Pay to Publish Node is expiring
 * + Pay to Publish Node is cancelled
 * + Pay to Publish Node has a payment
 */

/**
 * Implements hook_rules_event_info()
 */
function ms_paypublish_rules_event_info() {
  $items = array(
    'ms_paypublish_event_paypublish_cancel' => array(
      'label' => t('Pay to Publish Node Cancelled'),
      'group' => 'Pay to Publish',
      'help' => t("Triggers when a Pay to Publish Node is cancelled."),
      'variables' => array(
        'account' => array('type' => 'user', 'label' => t('User Account')),
        'node' => array('type' => 'node', 'label' => t('Pay to Publish Node')),
        'purchase' => array('type' => 'ms_products_purchase', 'label' => t('Pay to Publish Node Record')),
        'plan' => array('type' => 'ms_products_plan', 'label' => t('Pay to Publish Plan')),
      ),
    ),
    'ms_paypublish_event_paypublish_purchase' => array(
      'label' => t('Pay to Publish Node Purchase'),
      'group' => 'Pay to Publish',
      'help' => t("Triggers when a user pays to publish a new node."),
      'variables' => array(
        'account' => array('type' => 'user', 'label' => t('User Account')),
        'node' => array('type' => 'node', 'label' => t('Pay to Publish Node')),
        'purchase' => array('type' => 'ms_products_purchase', 'label' => t('Pay to Publish Node Record')),
        'plan' => array('type' => 'ms_products_plan', 'label' => t('Pay to Publish Plan')),
      ),
    ),
    'ms_paypublish_event_paypublish_payment' => array(
      'label' => t('Pay to Publish Node Payment Received'),
      'group' => 'Pay to Publish',
      'help' => t("Triggers when a payment is received for a paypublish."),
      'variables' => array(
        'account' => array('type' => 'user', 'label' => t('User Account')),
        'node' => array('type' => 'node', 'label' => t('Pay to Publish Node')),
        'purchase' => array('type' => 'ms_products_purchase', 'label' => t('Pay to Publish Node Record')),
        'plan' => array('type' => 'ms_products_plan', 'label' => t('Pay to Publish Plan')),
      ),
    ),
    'ms_paypublish_event_paypublish_expiring' => array(
      'label' => t('Pay to Publish Node Expiring'),
      'group' => 'Pay to Publish',
      'help' => t("Triggers when a Pay to Publish Node is expiring."),
      'variables' => array(
        'account' => array('type' => 'user', 'label' => t('User Account')),
        'node' => array('type' => 'node', 'label' => t('Pay to Publish Node')),
        'purchase' => array('type' => 'ms_products_purchase', 'label' => t('Pay to Publish Node Record')),
        'plan' => array('type' => 'ms_products_plan', 'label' => t('Pay to Publish Plan')),
      ),
    ),
  );

  return $items;
}
