<?php

/**
 * Implements hook_rules_condition_info().
 */
function commerce_fulfilment_oms_rules_action_info() {
  $actions = array();

  $actions['commerce_fulfilment_oms_rules_sendorder_action'] = array(
    'label' => t('Send order to OMS'),
    'type' => 'commerce_order',
    'configurable' => FALSE,
    'parameter' => array(
      'commerce_order' => array(
        'label' => t('Order', array(), array('context' => 'a drupal commerce order')),
        'type' => 'commerce_order',
        'skip save' => TRUE,
      ),
    ),
    'group' => t('Commerce Fulfilment'),
  );

  return $actions;
}


/**
 * Implements hook_rules_event_info().
 */
function commerce_fulfilment_oms_rules_event_info() {
  $events = array();

  $events['commerce_fulfilment_oms_rules_orderfulfiled_event'] = array(
    'label' => t('An order has been fulfilled'),
    'group' => t('Commerce Fulfilment'),
    'variables' => array(
      'commerce_order' => array(
        'label' => t('Order', array(), array('context' => 'a drupal commerce order')),
        'type' => 'commerce_order',
        'skip save' => TRUE,
      ),
    ),
  );

  return $events;
}


/**
 * Action: Send an order to OMS for fulfilment.
 */
function commerce_fulfilment_oms_rules_sendorder_action($order, $context = array()) {
  $vars = array('@order_number' => $order->order_number);
  watchdog('commerce_fulfilment_oms',
    'The "Send order to OMS" action is about to be run on @order_number ...',
    $vars,
    WATCHDOG_INFO);

  $client = commerce_fulfilment_oms_getclient();
  $salesorder = commerce_fulfilment_oms_createsalesorder($order);
  $live_mode = variable_get('commerce_fulfilment_oms_live_mode', 0);

  try {
    $oms_order_id = $client->create_sales_order($salesorder, $live_mode);
    $log = 'Order @order_number was successfully sent to OMS via the API.';
    $status = 'processing';
    $loglevel = WATCHDOG_INFO;
  } catch (OMSClientException $e) {
    $log = 'OMS rejected order @order_number (!message).';
    $vars['!message'] = $e->getMessage();
    $status = 'review';
    $loglevel = WATCHDOG_WARNING;
  }

  watchdog('commerce_fulfilment_oms', $log, $vars, $loglevel);
  commerce_order_status_update($order, $status, FALSE, TRUE, t($log, $vars));
}
