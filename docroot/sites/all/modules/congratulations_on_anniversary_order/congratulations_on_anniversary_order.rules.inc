<?php

/**
 * @file
 * Create new Rules event which is being called after anniversary order.
 */

/**
 * Implements hook_rules_event_info().
 *
 * That Rules event allows site admin to trigger any action on anniversary order
 * creation. That will give additional flexibility to him/her.
 */
function congratulations_on_anniversary_order_rules_event_info() {
  $items = array(
    'anniversary_order_completed' => array(
      'label' => t('After anniversary order was completed'),
      'group' => t('Anniversary Order'),
      'variables' => array(
        'commerce_order' => array(
          'label' => t('Anniversary_order_completed'),
          'type' => 'commerce_order',
        ),
      ),
    ),
  );
  return $items;
}
