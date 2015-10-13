<?php

/**
 * @file
 * Hooks provided by the Commerce Pay module.
 */

/**
 * Allow modules to alter the order data to be sent to USAePay API.
 *
 * @param $order_data
 *   Existing order data built by default from the billing profile.
 * @param $order
 *   The order object to which the payment applies.
 *
 * @see commerce_pay_submit_form_submit()
 * @see commerce_pay_usaepay_api_transaction()
 * @see http://help.usaepay.com/developer/phplibrary#reference_tables
 */
function hook_commerce_pay_order_data_alter(&$order_data, $order) {
  // Set charge description (optional)..
  $order_data['description'] = 'Some description';
  // Set the portion of amount that is sales tax.
  $order_data['tax'] = '12.33';
}

/**
 * Allow modules to alter billing data to be sent to USAePay API.
 *
 * @param $billing_data
 *   Existing billing data built by default from the billing profile.
 * @param $order
 *   The order object to which the payment applies.
 *
 * @see commerce_pay_submit_form_submit()
 * @see commerce_pay_usaepay_api_transaction()
 * @see http://help.usaepay.com/developer/phplibrary#reference_tables
 */
function hook_commerce_pay_billing_data_alter(&$billing_data, $order) {
  // Custom phone field or data.
  $billing_data['testmode'] = 'Custom-Phone-Field-Value';
  // Custom website url field or data.
  $billing_data['website'] = 'Custom-Website-Field-Value.com';
}

/**
 * Allow modules to alter shipping data to be sent to USAePay API.
 * Available only if there a shipping customer profile defined.
 *
 * @param $shipping_data
 *   Existing shipping data built by default from the shipping profile.
 * @param $order
 *   The order object to which the payment applies.
 *
 * @see commerce_pay_submit_form_submit()
 * @see commerce_pay_usaepay_api_transaction()
 * @see http://help.usaepay.com/developer/phplibrary#reference_tables
 */
function hook_commerce_pay_shipping_data_alter(&$shipping_data, $order) {
  // Custom phone field.
  $shipping_data['phone'] = 'Custom-Phone-Field-Value';
}
