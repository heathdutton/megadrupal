<?php

/**
 * @file
 * Implement customer interface for ec_address module.
 */

/**
 * Implements hook_customer_get_address().
 */
function ec_address_customer_get_address($customer, $aid = NULL) {
  if (!empty($aid)) {
    return (array) ec_address_address_load($aid);
  }
  else {
    return array_map('_ec_address_array_address', ec_address_address_load_all($customer->exid));
  }
}

function _ec_address_array_address($a) {
  return (array) $a;
}

/**
 * Implements hook_customer_links().
 */
function ec_address_customer_links($customer, $op) {
  switch ($op) {
    case 'add_address':
      return "user/{$customer->exid}/ec_address/add";
      break;

    case 'edit':
      return "user/{$customer->exid}/edit";
      break;

    case 'view':
      return "user/{$customer->exid}";
      break;
  }
}

/**
 * Implements hook_customer_address_save().
 */
function ec_address_customer_address_save($customer, $address) {
  $address['uid'] = ec_customer_get_uid($customer);
  drupal_write_record('ec_address', $address);
}

