<?php

/**
 * @file
 * Hook documentation for the NetCommerce Payment module.
 */


/**
 * Allows modules to alter the data array used to create a NetCommerce redirect
 * form prior to its form elements being created.
 *
 * @param &$data
 *   The data array used to create redirect form elements.
 * @param $order
 *   The full order object the redirect form is being generated for.
 */
function hook_commerce_netcommerce_form_data_alter(&$data, $order) {
  // No example.
}
