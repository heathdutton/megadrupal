<?php

/**
 * @file
 * Services module recources functions for the Pushretail For Ubercart module.
 */

/**
* Gets an order object by id.
* @param int $id
* @return object
*/
function uc_pushretail_get_order($id) {
  $order = uc_order_load($id);
  $data = new stdClass();
  $data->order_id = $order->order_id;
  foreach ($order->products as $item) {
    $product = new stdClass();
    $product->nid = $item->nid;
    $product->title = $item->title;
    $product->model = $item->model;
    $product->qty = $item->qty;
    $product->cost = uc_currency_format($item->cost);
    $product->price = uc_currency_format($item->price);
    $product->total = uc_currency_format($item->price * $item->qty);
    $data->products[] = $product;
  }
  $delivery = array();
  if ($order->delivery_first_name) {
     $delivery[] = $order->delivery_first_name . ' ' . $order->delivery_last_name;
  }
  if ($order->delivery_company) {
    $delivery[] = $order->delivery_company;
  }
  if ($order->delivery_street1) {
    $delivery[] = $order->delivery_street1;
  }
  if ($order->delivery_street2) {
    $delivery[] = $order->delivery_street2;
  }
  if ($order->delivery_city) {
    $delivery[] = $order->delivery_city . ', ' . $order->delivery_postal_code;
  }
  if ($order->delivery_phone) {
    $delivery[] = $order->delivery_phone;
  }
  $data->delivery = implode('<br />', $delivery);
  
  $billing = array();
  if ($order->billing_first_name) {
    $billing[] = $order->billing_first_name . ' ' . $order->billing_last_name;
  }
  if ($order->billing_company) {
    $billing[] = $order->delivery_company;
  }
  if ($order->billing_street1) {
    $billing[] = $order->delivery_street1;
  }
  if ($order->billing_street2) {
    $billing[] = $order->delivery_street2;
  }
  if ($order->billing_city) {
    $billing[] = $order->delivery_city . ', ' . $order->delivery_postal_code;
  }
  if ($order->billing_phone) {
    $billing[] = $order->billing_phone;
  }
  $data->billing = implode('<br />', $billing);
  
  $data->total = uc_currency_format($order->order_total);
  foreach ($order->line_items as $line_item) {
    if ($line_item['type'] == 'subtotal') {
      $data->subtotal = uc_currency_format($line_item['amount']);
    }
  }
  return $data;
}

/**
 * Returns orders list.
 * Filter by status and order id.
 * @param int $page
 * @param array $parameters
 * @return \stdClass 
 */
function uc_pushretail_orders_list($page, $parameters) {
  $data = new stdClass();
  $orders = array();
  $res = uc_pushretail_db_get_orders_list($parameters);
  foreach ($res as $item) {
    $order = new stdClass();
    $order->order_id = $item->order_id;
    $order->name = $item->billing_first_name . ' ' . $item->billing_last_name;
    $order->total = uc_currency_format($item->order_total);
    $order->date = format_date($item->created);
    $order->status = t($item->status);
    $orders[] = $order;
  }
  $data->orders = $orders;
  $data->status_list = uc_order_status_list();
  $data->params = $parameters;
  return $data;
}

/**
 * Common store overview info.
 * @param int $page
 * @param array $parameters
 * @return array 
 */
function uc_pushretail_dashboard_overview($page, $parameters) {
  $week_ago = date('Y-m-d', strtotime('-1 week'));
  $day_yesterday = date('d', strtotime('-1 day'));
  $day_today = date('d', time());
  $timestamp = strtotime($week_ago) + 60*60*24;
  $result = uc_pushretail_db_get_dashboard_orders_data($timestamp);
  $chart = uc_pushretail_days_period($timestamp);
  $today = $yesterday = array('orders_all' => 0, 'orders_completed' => 0, 'sales' => 0);
  foreach ($result as $order) {
    $key = date('d', $order->created);
    $chart[$key]['orders']++;
    if ($key == $day_today) {
      $today['orders_all']++;
      if ($order->order_status == 'completed') {
        $today['orders_completed']++;
        $today['sales'] += $order->order_total;
      }
    }
    elseif ($key == $day_yesterday) {
      $yesterday['orders_all']++;
      if ($order->order_status == 'completed') {
        $yesterday['orders_completed']++;
        $yesterday['sales'] += $order->order_total;
      }
    }
  }
  $days = array();
  $orders_count = array();
  foreach ($chart as $day => $data) {
    $days[] = $day;
    $orders_count[] = $data['orders'];
  }
  $today['sales'] = uc_currency_format($today['sales']);
  $yesterday['sales'] = uc_currency_format($yesterday['sales']);
  $output = array(
    'chart' => array(
      'days' => implode('|', $days),
      'orders' => implode(',', $orders_count),
    ),
    'today' => $today,
    'yesterday' => $yesterday,
  );
  return $output;
}

function uc_pushretail_days_period($time_from) {
  $timestamp = $time_from;
  $now = time();
  for (; ; ) {
    if ($timestamp > $now) {
        break;
    }
    $key = date('d', $timestamp);
    $days[$key] = array('date' => $key, 'orders' => 0);
    $timestamp = $timestamp + 60*60*24;
  }
  return $days;
}

/**
 * Return cistomer info by user id.
 * @param int $uid
 * @return \stdClass 
 */
function uc_pushretail_get_customer($uid) {
  $result = uc_pushretail_db_get_customer_info($uid);
  $customer_info = $result->fetchObject();

  $data = new stdClass();
  $delivery = array();
  if ($customer_info->delivery_first_name) {
     $delivery[] = $customer_info->delivery_first_name . ' ' . $customer_info->delivery_last_name;
  }
  if ($customer_info->delivery_company) {
    $delivery[] = $customer_info->delivery_company;
  }
  if ($customer_info->delivery_street1) {
    $delivery[] = $customer_info->delivery_street1;
  }
  if ($customer_info->delivery_street2) {
    $delivery[] = $customer_info->delivery_street2;
  }
  if ($customer_info->delivery_city) {
    $delivery[] = $customer_info->delivery_city . ', ' . $customer_info->delivery_postal_code;
  }
  if ($customer_info->delivery_phone) {
    $delivery[] = $customer_info->delivery_phone;
  }
  if ($customer_info->mail) {
    $delivery[] = $customer_info->mail;
  }
  $data->delivery = implode('<br />', $delivery);
  
  $billing = array();
  if ($customer_info->billing_first_name) {
    $billing[] = $customer_info->billing_first_name . ' ' . $customer_info->billing_last_name;
  }
  if ($customer_info->billing_company) {
    $billing[] = $customer_info->delivery_company;
  }
  if ($customer_info->billing_street1) {
    $billing[] = $customer_info->delivery_street1;
  }
  if ($customer_info->billing_street2) {
    $billing[] = $customer_info->delivery_street2;
  }
  if ($customer_info->billing_city) {
    $billing[] = $customer_info->delivery_city . ', ' . $customer_info->delivery_postal_code;
  }
  if ($customer_info->billing_phone) {
    $billing[] = $customer_info->billing_phone;
  }
  if ($customer_info->mail) {
    $billing[] = $customer_info->mail;
  }
  $data->billing = implode('<br />', $billing);

  $result = uc_pushretail_db_get_customer_orders($uid);
  foreach ($result as $order) {
    $order->order_total = uc_currency_format($order->order_total);
    $order->date = format_date($order->modified);
    $data->orders[] = $order;
  }
  return $data;
}

/**
 * Returns customers list.
 * & Filter by name.
 * @param int $page
 * @param array $parameters
 * @return \stdClass 
 */
function uc_pushretail_customers_list($page, $parameters) {
  $data = new stdClass();
  $customers = array();
  $res = uc_pushretail_db_get_customers_list($parameters);
  foreach ($res as $item) {
    $customer = new stdClass();
    $customer->uid = $item->uid;
    $customer->name = $item->name;
    $customer->full_name = $item->billing_first_name . ' ' . $item->billing_last_name;
    $customer->orders_count = format_plural($item->count, '1 order', '@count orders');
    $customer->registered = format_date($item->created);
    $customers[] = $customer;
  }
  $data->customers = $customers;
  return $data;
}

/**
 * Returns products list.
 * @param int $page
 * @param array $parameters
 * @return \stdClass 
 */
function uc_pushretail_products_list($page, $parameters) {
  $list = array();
  $data = new stdClass();
  $options = array(
    'limit' => variable_get('uc_pushretail_list_limit', 15),
    'name' => $parameters['name'],
  );
  if (module_exists('uc_stock')) {
    $result = uc_pushretail_db_get_products_stock($options);
  }
  else {
    $result = uc_pushretail_db_get_products($options);
  }
  foreach ($result as $product) {
    $list[] = array(
      'title' => $product->title,
      'price' => uc_currency_format($product->price),
      'sku' => $product->sku,
      'stock' => isset($product->stock) ? $product->stock : t('n/a'),
    );
  }
  $data->products = $list;
  return $data;
}
