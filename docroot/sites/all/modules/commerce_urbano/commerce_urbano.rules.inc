<?php

/**
 * @file
 * Rules integration for Urbano shipping.
 *
 * @addtogroup rules
 *
 * @{
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_urbano_rules_action_info() {
  $actions = array();

  $actions['commerce_urbano_set_shipment'] = array(
    'label' => t('Set shipment in Urbano'),
    'description' => t('Sets the shipment in Urbano, so it will be picked up at the store after confirmation in the Urbano website.'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce Shipping (Urbano)'),
  );

  return $actions;
}

/**
 * Action: Set shipment in Urbano.
 */
function commerce_urbano_set_shipment($order) {
  try {
    module_load_include('inc', 'commerce_urbano', 'includes/commerce_urbano.service');

    $urbano = new CommerceUrbanoConnector();
    $urbano->setPickup($order);

    if (module_exists('commerce_shipment_message')) {
      commerce_shipment_message_log_message($order, 'urbano_home_delivery');
    }
  }
  catch (Exception $e) {
    watchdog_exception('commerce_urbano', $e);

    return FALSE;
  }
}

/**
 * @}
 */
