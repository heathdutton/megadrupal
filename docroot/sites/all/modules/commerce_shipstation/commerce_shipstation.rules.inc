<?php
/**
 * @file
 *
 * Declares rules events, actions, and conditions for Commerce Shipstation module.
 */


/**
 * Implements hook_rules_event_info().
 */
function commerce_shipstation_rules_event_info() {
  $events = array();

  $events['commerce_shipstation_order_success'] = array(
    'label' => t('ShipStation reports an order has completed successfully'),
    'group' => t('Commerce ShipStation'),
    'variables' => array(
      'commerce_order' => array(
      'label' => t('Order', array(), array('context' => 'a Drupal Commerce order')),
      'type' => 'commerce_order',
        'skip save' => TRUE,
      ),
      'tracking_number' => array(
        'type' => 'text',
        'label' => 'Shipping tracking number',
      ),
      'carrier' => array(
        'type' => 'text',
        'label' => 'Shipping carrier',
      ),
      'service' => array(
        'type' => 'text',
        'label' => 'Shipping service',
      ),
    ),
  );

  return $events;
}
