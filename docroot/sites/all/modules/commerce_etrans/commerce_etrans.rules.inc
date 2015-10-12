<?php

/**
 * @file
 * Rules integration for etrans shipping.
 *
 * @addtogroup rules
 *
 * @{
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_etrans_rules_action_info() {
  $actions = array();

  $actions['commerce_etrans_set_shipment'] = array(
    'label' => t('Set shipment in etrans'),
    'description' => t('Sets the shipment in etrans, so it will be picked up at the store after confirmation in the etrans website.'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce Shipping (Etrans)'),
  );

  return $actions;
}

/**
 * Action: Set shipment in etrans.
 */
function commerce_etrans_set_shipment($order) {
  if (($library = libraries_load('etrans')) && !empty($library['loaded'])) {
    try {
      $etrans = new CommerceEtransConnector();
      $shipment = $etrans->setEnvio($order);
      if (module_exists('commerce_shipment_message')) {
        commerce_shipment_message_log_message($order, 'etrans_home_delivery', $shipment['pickup'], $shipment['delivery']);
      }
    }
    catch (CommerceEtransUnsupportedDestinationException $e) {
      return FALSE;
    }
    catch (Exception $e) {
      watchdog_exception('commerce_etrans', $e);
    }
  }
}

/**
 * @}
 */
