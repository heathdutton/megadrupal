<?php

/**
 * @file
 * Implementation of all the different features which have been implemented by the product module
 */

/**
 * Implements hook_feature_attributes().
 */
function ec_product_feature_no_discount_attributes($node, $a1 = NULL, $a2 = NULL, $a3 = NULL, $a4 = NULL) {
  return array('no_discount' => TRUE);
}

/**
 * Implementation of hook_feature_load().
 */
function ec_product_feature_no_discount_load($node, $a1 = NULL, $a2 = NULL, $a3 = NULL, $a4 = NULL) {
  return array('no_discount' => TRUE);
}

/**
 * Implements hook_feature_attributes().
 */
function ec_product_feature_shippable_attributes($node, $a1 = NULL, $a2 = NULL, $a3 = NULL, $a4 = NULL) {
  return array('is_shippable' => TRUE);
}

/**
 * Implementation of hook_feature_load().
 */
function ec_product_feature_shippable_load($node, $a1 = NULL, $a2 = NULL, $a3 = NULL, $a4 = NULL) {
  return array('is_shippable' => TRUE);
}

/**
 * Implements hook_feature_attributes().
 */
function ec_product_feature_no_quantity_attributes($node, $a1 = NULL, $a2 = NULL, $a3 = NULL, $a4 = NULL) {
  return array('no_quantity' => TRUE);
}

/**
 * Implementation of hook_feature_load().
 */
function ec_product_feature_no_quantity_load($node, $a1 = NULL, $a2 = NULL, $a3 = NULL, $a4 = NULL) {
  return array('no_quantity' => TRUE);
}

/**
 * Implements hook_feature_ec_checkout_validate_item().
 */
function ec_product_feature_permission_ec_checkout_validate_item($node, $type, $qty, $data, $severity, $return) {
  return user_access('purchase products');
}
