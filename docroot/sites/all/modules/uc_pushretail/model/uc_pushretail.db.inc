<?php

/**
 * @file
 * Database queries functions for the Pushretail For Ubercart module.
 */

/**
 * Get products list.
 * @param array $options
 * @return db query result 
 */
function uc_pushretail_db_get_products($options = array()) {
  $filter = array();
  $sql = 'SELECT n.title, up.sell_price AS price, up.model AS sku
    FROM {uc_products} up
    LEFT JOIN {node} n ON n.nid = up.nid';
  if (isset($options['name'])) {
    $sql .= " WHERE LOWER(n.title) LIKE LOWER(:name) OR
      LOWER(sku) LIKE LOWER(:name) ";
    $filter[':name'] = '%%' . $options['name'] . '%%';
  }
  $sql .= ' ORDER BY up.nid';
  $result = db_query_range($sql, 0, $options['limit'], $filter);
  return $result;
}

/**
 * Get product list with stock levels.
 * @param array $options
 * @return db query result  
 */
function uc_pushretail_db_get_products_stock($options = array()) {
  $filter = array();
  $sql = 'SELECT n.title, up.sell_price AS price, up.model AS sku, ups.stock
    FROM {uc_products} up
    LEFT JOIN {node} n ON n.nid = up.nid
    LEFT JOIN {uc_product_stock} ups ON ups.nid = n.nid';
  if (isset($options['name'])) {
    $sql .= " WHERE LOWER(n.title) LIKE LOWER(:name) OR
      LOWER(sku) LIKE LOWER(:name) ";
    $filter[':name'] = '%%' . $options['name'] . '%%';
  }
  $sql .= ' ORDER BY up.nid';
  $result = db_query_range($sql, 0, $options['limit'], $filter);
  return $result;
}

/**
 * Get store orders list.
 * @param array $parameters
 * @return db query result  
 */
function uc_pushretail_db_get_orders_list($parameters = array()) {
  $filter = array();
  $sql = 'SELECT uo.order_id, uo.billing_first_name, uo.billing_last_name, uo.order_total, uo.created,
    uos.title AS status
    FROM {uc_orders} uo
    LEFT JOIN {uc_order_statuses} uos ON uos.order_status_id = uo.order_status ';
  if (isset($parameters['order_id']) && is_numeric($parameters['order_id'])) {
    $sql .= ' WHERE uo.order_id = :order_id ';
    $filter[':order_id'] = $parameters['order_id'];
  }
  if (isset($parameters['status']) && $parameters['status'] != 'all') {
    if (is_numeric($parameters['order_id'])) {
      $sql .= ' AND uo.order_status = :order_status ';
    }
    else {
      $sql .= ' WHERE uo.order_status = :order_status ';
    }
    $filter[':order_status'] = $parameters['status'];
  }
  $sql .= 'ORDER BY uo.modified DESC';
  return db_query_range($sql, 0, variable_get('uc_pushretail_list_limit', 15), $filter);
}

/**
 * Get orders data for dashboard.
 * @param int $timestamp
 * @return db query result
 */
function uc_pushretail_db_get_dashboard_orders_data($timestamp) {
  $sql = 'SELECT uo.order_id, uo.order_total, uo.created, uo.order_status  
    FROM {uc_orders} uo
    WHERE uo.created > :date
    ORDER BY uo.created DESC';
  return db_query($sql, array(':date' => $timestamp));
}

/**
 * Get specific customer info.
 * @param int $uid
 * @return db query result 
 */
function uc_pushretail_db_get_customer_info($uid) {
  $sql = 'SELECT uo.*, COUNT(uo.order_id) AS count, u.created, u.name, u.mail
    FROM {users} u
    LEFT JOIN {uc_orders} uo ON u.uid = uo.uid 
    WHERE u.uid = :uid
    GROUP BY uo.uid ORDER BY uo.created DESC ';
  return db_query($sql, array(':uid' => $uid));
}

/**
 * Get customers orders list.
 * @param int $uid
 * @return db query result  
 */
function uc_pushretail_db_get_customer_orders($uid) {
  $sql = 'SELECT uo.order_id, uos.title AS status, uo.order_total, uo.product_count, uo.modified
    FROM {uc_orders} uo 
    LEFT JOIN {uc_order_statuses} uos ON uos.order_status_id = uo.order_status
    WHERE uo.uid = :uid
    ORDER BY uo.modified DESC';
  return db_query($sql, array(':uid' => $uid));
}

/**
 * Get store customers list.
 * @param array $parameters
 * @return db query result  
 */
function uc_pushretail_db_get_customers_list($parameters) {
  $filter = array();
  $sql = 'SELECT uo.uid, uo.billing_first_name, uo.billing_last_name, COUNT(uo.order_id) AS count, u.created, u.name
    FROM {uc_orders} uo
    LEFT JOIN {users} u ON u.uid = uo.uid ';
  if (isset($parameters['name'])) {
    $sql .= " WHERE LOWER(u.name) LIKE LOWER(:name) OR
      LOWER(uo.billing_first_name) LIKE LOWER(:name) OR 
      LOWER(uo.billing_last_name) LIKE LOWER(:name)";
    $filter[':name'] = '%%' . $parameters['name'] . '%%';
  }
  $sql .= 'GROUP BY uo.uid ORDER BY uo.created DESC ';
  return db_query_range($sql, 0, variable_get('uc_pushretail_list_limit', 15), $filter);
}
