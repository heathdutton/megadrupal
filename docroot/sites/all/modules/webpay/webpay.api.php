<?php

/**
 * @file
 * Implements functionality for to use Webpay as payment gateway.
 *
 * This is a base module that has submodules to implement specific functions for
 * Ubercart and Drupal Coomerse. In case that a new commerce module appears, a
 * new Webpay gateway's module can be created with this implementations.
 */

/**
 * Declares the hook_webpay_commerce_system.
 * 
 * @return array
 *   An array with the following elements:
 *   - title: System's name
 *   - description: System's description
 * 	 - success: callback for success page
 * 	 - failure: callback for failiure page
 * 	 - close validate: callback to validate the order on closing page
 * 	 - order load: callback executed when validation on closing page, that
 *  loads the order information
 * 	 - save transaction(optional): callback invoked when the transaction is been
 *  saved on closing page
 *   - accept transaction(optional): callback invoked after all previous
 *  validations
 */
function hook_webpay_webpay_commerce_system() {
  $info = array();
  $info['my_commerce'] = array(
    'title' => 'My Commerce',
    'success' => 'my_commerce_webpay_success',
    'failure' => 'my_commerce_webpay_failure',
    'close validate' => 'my_commerce_webpay_validate_close',
    'order load' => 'my_commerce_webpay_order_load',
    'save transaction' => 'my_commerce_webpay_save_transaction',
    'accept transaction' => 'my_commerce_webpay_accept_transaction',
  );
  return $info;
}
