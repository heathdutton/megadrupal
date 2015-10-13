<?php
/**
 * @file
 * Rules implementation for uc_ecorepay.
 */

/**
 * Implements hook_rules_action_info().
 */
function uc_ecorepay_rules_action_info() {
  // Capture action.
  $actions = array(
    'uc_ecorepay_action_capture' => array(
      'label' => t('Capture authorised payment'),
      'group' => t('Ecorepay'),
      'parameter' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
        ),
      ),
    ),
  );

  return $actions;
}

/**
 * Callback for capture.
 *
 * @param object $order
 *   Order object.
 */
function uc_ecorepay_action_capture($order) {
  uc_ecorepay_capture($order->order_id);
}
