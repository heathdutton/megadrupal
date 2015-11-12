<?php

/**
 * @file
 * Corporate Disk Fulfillment settings administration UI.
 */

/**
 * Page for base listing of all orders ordered by created at descending.
 */
function corporate_disk_company_basicpage() {
  $rows = array();
  $results = db_query("select order_number, confirmation, created_at from reporting_diskcom_shipments order by created_at desc");
  foreach ($results as $node) {
    $rows[] = array($node->order_number,
      $node->confirmation,
      $node->created_at,
    );
  }
  $table = array(
    '#theme' => 'table',
    '#header' => array(t('Order Number'), t('Confirmation'), t('Created At')),
    '#rows' => $rows
  );
  return $table;
}

/**
 * Test page.
 */
function corporate_disk_testpage() {
  $i = 0;
  $orders = array();
  $orders_update = array();
  $responsearray = array();
  // Lets pull orders that need to be updated with tracking numbers.
  $sql = 'SELECT * from {corporate_disk_company_shipments} where order_updated = \'0\' and shipping_service NOT LIKE \'%First Class%\'';
  $result = db_query($sql)
    ->fetchAll();
  // Lets create an array of items that need to be updated.
  foreach ($result as $items) {
    $orders[] = array(
      'ordernumber' => $items->order_number,
      'orderconfirm' => $items->confirmation,
    );
  }
  $id = variable_get('corporate_disk_company_companyid', '');
  foreach ($orders as $key => $value) {
    $context = stream_context_create(array('http' => array('header' => 'Accept: application/xml')));
    $url = 'https://customers.disk.com/shippinginfo.jsp?cid=' . $id . '&packageshipmentid=' . $value['orderconfirm'];
    $response = file_get_contents($url, false, $context);
    $response = simplexml_load_string($response);
    $vars = get_object_vars($response);
    foreach ($vars as $key => $val) {
      $responsearray[$key] = $val;
    }
    if ($responsearray['status'] == 'completed' && !empty($responsearray['tracking-number'])) {
      // Create an array of order numbers and tracking numbers to do DB updates with.
      $orders_update[] = array(
        'status' => $responsearray['status'],
        'ordernumber' => $value['ordernumber'],
        'trackingno' => $responsearray['tracking-number'],
      );
    }
  }
  foreach ($orders_update as $items) {
    db_update('corporate_disk_company_shipments')
      ->fields(array(
        'tracking_number' => $items['trackingno'],
        'order_updated' => 1,
      ))
      ->condition('order_number', $items['ordernumber'], '=')
      ->execute();
  }


  /**
   * Migration Script
   *

    $sql = 'select * from reporting_diskcom_shipments';
    $result = db_query($sql)
    ->fetchAll();
    foreach ($result as $items) {
    $p = explode('|', $items->service_post);
    $service = urldecode($p[11]);

    $queue = db_insert('corporate_disk_company_shipments')
    ->fields(array(
    'order_number' => $items->order_number,
    'confirmation' => $items->confirmation,
    'service_post' => $items->service_post,
    'shipping_service' => $service,
    'created_at' => $items->created_at,
    ))
    ->execute();
    }
   */
  /*
    if (corporate_disk_company_process('testpage')) {
    $page = array(
    'intro' => array('#markup' => 'hello'),
    );
    $shippingrow = db_select('commerce_line_item', 'c')
    ->fields('c')
    ->condition('order_id', 6599, '=')
    ->condition('type', 'shipping', '=')
    ->execute()
    ->fetchAssoc();
    $commerce_shipping_services = commerce_shipping_services();
    $commerce_shipping_service_names = array();
    foreach ($commerce_shipping_services as $key => $value) {
    $commerce_shipping_service_names[$value['title']] = $key;
    }
    $disk_shipping_services = variable_get('corporate_disk_company_shipping_services');
    //firep($commerce_shipping_service_names, 'Shipping names reversal');
    //firep($disk_shipping_services, 'Disk shipping services');
    firep($shippingrow, 'order shipping');

    $shippingvar = 'corporate_disk_company_shipping_services_' . $commerce_shipping_service_names[$shippingrow['line_item_label']];
    firep($shippingvar, 'shipping var');
    $shippingmethod = variable_get($shippingvar);
    firep($shippingmethod, 'pullled shipping method');
    return $page;
    }
    else {
    $page = array(
    'intro' => array('#markup' => 'Disk process did not run'),
    );
    return $page;
    } */
  $page = array(
    'intro' => array('#markup' => 'hello'),
  );
  return $page;
}
