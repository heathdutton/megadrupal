<?php

/**
 * Implements hook_drush_help().
 */
function commerce_fulfilment_oms_drush_help($section) {
  switch ($section) {
    case 'drush:check-oms-status':
      return dt('Rebuild the registry or module cache in a Drupal install.');
  }
}


/**
 * Implements hook_drush_command().
 */
function commerce_fulfilment_oms_drush_command() {
  $items = array();

  $items['check-oms-status'] = array(
    'description' => 'Check order status against OMS.',
    'callback' => 'commerce_fulfilment_oms_drush_checkstatus',
    'options' => array(
      'dry-run' => 'Do no fire the rules event.',
    ),
    'examples' => array(
      'drush cos --dry-run' => 'Check the order status and report the results, but do not fire rules event.',
    ),
    'aliases' => array('cos'),
  );

  return $items;
}

/**
 * Check all "processing" orders against OMS.
 *
 * If the order is completed, fire an event to let Rules take care of it.
 */
function commerce_fulfilment_oms_drush_checkstatus() {
  $orders = commerce_order_load_multiple(array(), array('status' => 'processing'));
  if (count($orders) === 0) {
    drush_log(dt('No orders found with status "processing".'), 'success');
    return;
  }

  // Make sure orders are not locked during our long running process
  entity_get_controller('commerce_order')->resetCache(array_keys($orders));

  $order_numbers = array();
  foreach ($orders as $order) {
    $order_numbers[$order->order_number] = $order;
  }

  $client = commerce_fulfilment_oms_getclient();

  // Create batches
  $batches = array_chunk($order_numbers, 50, TRUE);
  $msg = 'Preparing to process @total orders in @batches batches.';
  drush_log(dt($msg, array('@total' => count($order_numbers), '@batches' => count($batches))), 'status');

  $completed = $failed = 0;
  foreach ($batches as $batch) {
    $status = _commerce_fulfilment_oms_drush_checkstatus_batch($client, $batch);
    if ($status) {
      $completed += $status['completed'];
      $failed += $status['failed'];
    } else {
      $completed += 0;
      $failed += count($batch);
    }
  }

  // Report the status
  $vars = array('@total' => count($orders), '@completed' => $completed, '@failed' => $failed);
  drush_log(dt('Reviewed @total processing orders, @failed failed, reported @completed completed orders.', $vars), 'success');
}


/**
 * Process a batch.
 *
 * It's necessary to break the task down into batches of 50 orders because
 * OMS runs out of memory somewhere between 100 and 200 orders.
 */
function _commerce_fulfilment_oms_drush_checkstatus_batch($client, $orders) {

  // Grab the OMS client
  try {
    end($orders); $end = key($orders);
    reset($orders); $start = key($orders);
    $msg = '- Requesting information from OMS (orders @start to @end).';
    drush_log(dt($msg, array('@start' => $start, '@end' => $end)), 'status');
    $oms_orders = $client->export_orders('SHP', NULL, array_keys($orders));
  } catch (OMSClientException $e) {
    $vars = array('%error' => $e->getMessage());
    watchdog('commerce_fulfilment_oms', 'Failed to fetch order status from OMS: %error', $vars, WATCHDOG_ERROR);
    drush_log(dt('- Unable to fetch order status from OMS.'), 'warning');
    return;
  }

  // Make a lookup table of the orders vs OMS orders
  //
  // OMS could have multiple orders for the same merchant ID (e.g. if it's resent)
  // so we only use the latest order.
  $order_lookuptable = array();
  foreach ($oms_orders as $oms_order) {
    if (isset($order_lookuptable[$oms_order->Merchant_ID])) {
      $replacement = strtotime($oms_order->DateCreated);
      $current = strtotime($order_lookuptable[$oms_order->Merchant_ID]->DateCreated);
      if ($current > $replacement) {
        echo sprintf("Multiple orders returned for order number %s, discarding %s (created %s)\n",
          $oms_order->Merchant_ID, $oms_order->Order_Number, $oms_order->DateCreated);
        continue;
      }
    }
    $order_lookuptable[$oms_order->Merchant_ID] = $oms_order;
  }

  // Iterate the orders and fire the event.
  $completed = $failed = 0;
  foreach ($orders as $order_number => $order) {
    if (!isset($order_lookuptable[$order->order_number])) {
      $failed++;
      drush_log(dt('Could not find order @order_number in OMS export', array('@order_number' => $order->order_number)));
    }
    $oms_order = $order_lookuptable[$order->order_number];

    if ($oms_order->Status === 'Complete') {
      $shipment = $oms_order->Shipments->Shipment;
      $tracking_url = commerce_fulfilment_oms_get_tracking_url($shipment->ShipMethod, $shipment->TrackingNumber);
      $order->tracking_url = $tracking_url ? $tracking_url : '(unavailable)'; // Wasn't able to find a better way to do this.

      if (!drush_get_option('dry-run')) {
        rules_invoke_event('commerce_fulfilment_oms_rules_orderfulfiled_event', $order);
      }
      $completed++;
    }
  }

  // Report the status
  $vars = array('@total' => count($orders), '@completed' => $completed, '@failed' => $failed);
  drush_log(dt('- Examined @total processing orders, @failed failed, reported @completed completed orders.', $vars), 'success');

  return array('completed' => $completed, 'failed' => $failed);
}
