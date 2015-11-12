<?php
/**
 * @file
 *   commerce_unleashed_order.rules.inc
 */

/**
 * Impliments hook_rules_action_info().
 */
function commerce_unleashed_order_rules_action_info() {
  $actions = array();

  $actions['commerce_unleased_order_post'] = array(
    'label' => t('Post an order to Unleashed'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'save' => TRUE,
      ),
    ),
    'group' => t('Commerce Unleashed'),
    'callbacks' => array(
      'execute' => 'commerce_unleased_order_post',
    ),
  );

  return $actions;
}
