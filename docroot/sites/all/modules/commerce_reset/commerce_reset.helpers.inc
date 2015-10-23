<?php
/**
 * @file
 * Contains helper functions for commerce reset.
 */

/**
 * Return orders.
 */
function commerce_reset_get_orders_ids() {
  return commerce_reset_get_entity_data('commerce_order', 'order_id');
}

/**
 * Return order count.
 */
function commerce_reset_order_count() {
  return commerce_reset_get_data_count('commerce_order', 'order_id');
}

/**
 * Return payment transactions.
 */
function commerce_reset_get_transaction_ids() {
  return commerce_reset_get_entity_data('commerce_payment_transaction', 'transaction_id');
}

/**
 * Return payment transaction count.
 */
function commerce_reset_transaction_count() {
  return commerce_reset_get_data_count('commerce_payment_transaction', 'transaction_id');
}

/**
 * Return customer profiles.
 */
function commerce_reset_get_customer_profiles_ids() {
  return commerce_reset_get_entity_data('commerce_customer_profile', 'profile_id');
}

/**
 * Return customer profile count.
 */
function commerce_reset_customer_profiles_count() {
  return commerce_reset_get_data_count('commerce_customer_profile', 'profile_id');
}

/**
 * Return products.
 */
function commerce_reset_get_products_ids() {
  return commerce_reset_get_entity_data('commerce_product', 'product_id');
}

/**
 * Return product count.
 */
function commerce_reset_products_count() {
  return commerce_reset_get_data_count('commerce_product', 'product_id');
}

/**
 * Database query helper function.
 *
 * @param string $table_name
 *   Database table name.
 * @param string $primary_key
 *   Primary key of the table.
 *
 * @return \DatabaseStatementInterface
 *   Database object.
 */
function commerce_reset_get_entity_data($table_name, $primary_key) {
  return db_select($table_name, $table_name)
    ->fields($table_name, array($primary_key))
    ->execute();
}

/**
 * Database record count function.
 *
 * @param string $table_name
 *   Database table name.
 * @param string $primary_key
 *   Primary key of the table.
 *
 * @return int
 *   Total count of the records.
 */
function commerce_reset_get_data_count($table_name, $primary_key) {
  $data = commerce_reset_get_entity_data($table_name, $primary_key);
  return $data->rowCount();
}
