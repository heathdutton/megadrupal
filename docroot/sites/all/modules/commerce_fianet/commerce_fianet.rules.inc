<?php

/**
 * Implements hook_rules_action_info().
 */
function commerce_fianet_rules_action_info() {
  return array(
    'commerce_fianet_queue_order_verification' => array(
      'label' => t('Queue order for FIA-NET verification'),
      'group' => t('Commerce FIA-NET'),
      'parameter' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Commerce order'),
        ),
      ),
      'base' => 'commerce_fianet_queue_order_verification',
    )
  );
}

/**
 * Store the commerce order to process in a FIA-NET dedicated Drupal queue.
 * We only store commerce order ids.
 * @param $commerce_order
 *   Commerce order to store in the queue.
 */
function commerce_fianet_queue_order_verification($commerce_order) {
  $queue = new SystemQueue('commerce_fianet_orders_verification');
  $queue->createQueue();  // There is no harm in trying to recreate existing.

  // Queue the string.
  $queue->createItem($commerce_order->order_id);
  $order_link = l('#' . $commerce_order->order_id, 'admin/commerce/orders/' . $commerce_order->order_id);
  watchdog('Commerce FIA-NET', 'The order !order has been queued for FIA-NET validation.', array('!order' => $order_link));
}
