<?php
/**
 * @file
 * Cron tasks for the YellowCube module.
 */

/**
 * Cron job for creating order updates.
 */
function yellowcube_jobs_fetch_order_updates() {
  module_load_include('client.inc', 'yellowcube');

  $service = yellowcube_client_get_service();

  //Find all pending orders
  $pending_orders = yellowcube_jobs_get_pending_orders();

  foreach ($pending_orders as $order_ref) {

    //Load the full order object
    $order = commerce_order_load($order_ref->order_id);

    //Check order status
    $response = $service->getYCCustomerOrderStatus($order_ref->yc_reference);

    //Skip problematic orders
    if ($response->isError()) {

      $order_link = l(
        t('view'),
        'admin/commerce/orders/' . $order->order_id . '/edit',
        array(
          'query' => array('destination' => 'admin/reports/dblog'),
        )
      );

      if ($response->isError()) {
        //log failed orders
        watchdog(
          'yellowcube',
          'Order @order_id could not be transferred to YellowCube. Received reference number @ref and status message “@status”.',
          array(
            '@order_id' => $order->order_number,
            '@ref' => $response->getReference(),
            '@status' => $response->getStatusText(),
          ),
          WATCHDOG_NOTICE,
          $order_link
        );

        commerce_order_status_update($order, 'yellowcube_send_failed');
        continue;
      }
    }

    //find orders that have been processed
    if ($response->isSuccess() && !$response->isPending() && !$response->isError()) {

      $goodsIssueList = $service->getYCCustomerOrderReply($order->order_id);

      foreach ($goodsIssueList as $goodsIssue) {
        $header = $goodsIssue->getCustomerOrderHeader();
        $shipment_no = $header->getPostalShipmentNo();

        //Multi packaging / shipping is not supported atm.
        if(!empty($shipment_no)) {
          //shipping number contains a semicolon, post api supports multiple values
          $shipping_url = 'http://www.post.ch/swisspost-tracking?formattedParcelCodes=' . $shipment_no;
          break;
        }
      }

      if(!empty($goodsIssueList)) {
        //Add a message to the order history incl. link to shipping infos
        $message = t('Your order has been shipped. You can use the following url for shipping tracking: !url', array('!url' => $shipping_url));
        yellowcube_jobs_add_custom_message($order, 'completed', $message);

        //Update the order status to complete
        commerce_order_status_update($order, 'complete');
      }

    }
  }

}

/**
 * Returns all pending orders on yellow cube
 */
function yellowcube_jobs_get_pending_orders() {

  $pending_orders = db_query(
    "
    SELECT co.order_id, co.order_number, co.status, co.type, co.created, co.changed,
           yo.yc_reference
    FROM {commerce_order} AS co
    LEFT JOIN {yellowcube_order} AS yo USING (order_id)
    WHERE (co.status = 'yellowcube_received');"
  );

  return $pending_orders;
}


/**
 * Returns all new orders
 */
function yellowcube_jobs_get_new_orders() {

  $new_orders = db_query(
    "
    SELECT co.order_id, co.order_number, co.status, co.type, co.created, co.changed,
           yo.yc_reference
    FROM {commerce_order} AS co
    LEFT JOIN {yellowcube_order} AS yo USING (order_id)
    WHERE (co.status = 'pending');"
  );

  return $new_orders;
}

/**
 * Returns all pending stock items
 */

function yellowcube_jobs_get_pending_stock($product) {

  $pending_stock = 0;

  foreach (commerce_order_load_multiple(array(), array('status' => 'yellowcube_received'), TRUE) as $order) {
    foreach (entity_metadata_wrapper('commerce_order', $order)->commerce_line_items as $delta => $line_item_wrapper) {
      if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
        //if we find a pending line item, add it to the stock
        $product_id = $line_item_wrapper->commerce_product->raw();
        if($product_id == $product->product_id) {
          $pending_stock += $line_item_wrapper->quantity->value();
        }
      }
    }
  }

  return $pending_stock;

}

/**
 * Add a message to the order
 */

function yellowcube_jobs_add_custom_message($order, $status, $message_body) {
  $message = message_create('commerce_order_admin_comment', array('uid' => $order->uid));
  $wrapper = entity_metadata_wrapper('message', $message);
  $wrapper->message_commerce_order->set($order);
  $wrapper->message_commerce_body->set(array(
    'value' => $message_body,
    'summary' => '',
    'format' => 'filtered_html',
  ));
  $wrapper->save();
}

function yellowcube_job_submit_new_orders() {
  module_load_include('rules.inc', 'yellowcube');
  $orders = yellowcube_jobs_get_new_orders();

  foreach ($orders as $order) {
    $order_loaded = commerce_order_load($order->order_id);
    yellowcube_send_order_action($order_loaded, false);
  }

}