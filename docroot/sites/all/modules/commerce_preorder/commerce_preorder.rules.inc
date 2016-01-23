<?php

/**
 * @file
 * Rules integration for the preorder process.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_preorder_rules_action_info() {
  $actions = array();

  $actions['commerce_preorder_generate_pending_orders'] = array(
    'label' => t('Generate pending orders'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Pre-order'),
        'description' => t('The pre-order that will be used to generate the future pending orders.
                           This is typically the current checkout order on checkout complete.'),
      ),
      'order_status' => array(
        'type' => 'text',
        'label' => t('New Order status'),
        'options list' => 'commerce_order_status_options_list',
        'default value' => 'preordered',
        'optional' => TRUE,
        'allow null' => TRUE,
      ),
    ),
    'provides' => array(
      'pending_orders' => array(
        'label' => t('Pending orders'),
        'type' => 'list<commerce_order>',
      ),
    ),
    'group' => t('Commerce Pre-order'),
    'callbacks' => array(
      'execute' => 'commerce_preorder_rules_generate_pending_orders',
    ),
  );

  return $actions;
}

/**
 * Rules action: updates an order's status using the Order API.
 */
function commerce_preorder_rules_generate_pending_orders($order, $new_status = 'preordered') {
  $new_orders = commerce_preorder_generate_orders($order, $new_status);
  return array('pending_orders' => !empty($new_orders) ? $new_orders : array());
}

/**
 * @}
 */
