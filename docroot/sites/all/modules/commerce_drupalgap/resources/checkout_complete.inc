<?php

/**
 * @file
 * This file implements the Commerce DrupalGap service resource call back
 * functions.
 */

/**
 * 
 */
function _commerce_drupalgap_resource_checkout_complete($data) {
 
  $order = commerce_order_load($data['order_id']);
  // complete the checkout
  commerce_checkout_complete($order);
  return TRUE;
}

/**
 * Determines an API user's access to create new pamyent transactions.
 */
function _commerce_drupalgap_resource_checkout_complete_access($data) {
  // Load the order from the data array to determine access to create payments
  // on it.
  $order = NULL;
  if (!empty($data['order_id'])) {
    $order = commerce_order_load($data['order_id']);
  }

  if (empty($order)) {
    return services_error(t('You must specify a valid order ID'), 400);
  }

  // If the user has access to update the order...
  if (commerce_order_access('update', $order)) {
    return TRUE;
  }
  else {
    return services_error(t('Access to this operation not granted'), 401);
  }
}
