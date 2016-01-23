<?php

/**
 * @file
 * Rules actions and conditions for working with marketplace order.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_marketplace_order_rules_action_info() {
  $actions = array(
    'commerce_marketplace_order_load_store_orders_from_marketplace_order' => array(
      'label' => t('Load related Store orders from Marketplace order'),
      'group' => t('Commerce Marketplace'),
      'parameter' => array(
        'marketplace_order' => array(
          'type' => 'commerce_order',
          'label' => t('Marketplace Order'),
        ),
      ),
      'provides' => array(
        'store_orders' => array(
          'type' => 'list<commerce_order>',
          'label' => t('Store orders'),
        ),
      ),
    ),
  );
  return $actions;
}

/**
 * Action callback for loading store orders from a marketplace order.
 */
function commerce_marketplace_order_load_store_orders_from_marketplace_order($order) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'commerce_order')
    ->entityCondition('bundle', 'commerce_store_order')
    ->propertyCondition('uid', $order->uid)
    ->fieldCondition('cmp_order', 'target_id', $order->order_id);
  $results = $query->execute();
  $orders = array();
  if (isset($results['commerce_order'])) {
    foreach ($results['commerce_order'] as $res) {
      $orders[] = commerce_order_load($res->order_id);
    }
  }
  return array('store_orders' => $orders);
}

