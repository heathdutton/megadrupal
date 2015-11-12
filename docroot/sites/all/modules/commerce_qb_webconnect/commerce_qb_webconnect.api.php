<?php

/**
 * Allow modules to provide logic to payment information to a string that will
 * be used as a PaymentMethodRef in Quickbooks
 *
 * @param string $payment_type
 *  Payment type looking for a mapping to a Quickbooks PaymentMethodRef
 *  @see commerce_payment_methods()
 *
 * @param string $export_type
 *  Type of export that is being processed
 *
 * @param type $data
 *  JSON decoded data found in the export entity being processed. Currently
 *  this will be an associative-array representation of whatever was stored.
 *
 * @return string
 *  Map result to be used as a PaymentMethodRef in quickbooks
 */
function hook_MODULE_NAME_commerce_qb_webconnect_payment_mapping($payment_type, $export_type, $data) {
  switch ($payment_type) {
    case 'commerce_payment_example':
      switch ($export_type) {
        case 'add_sales_receipt':
        case 'add_payment':
          return variable_get('commerce_qb_webconnect_example_payment_method', '');
    
          break;
      }

      break;
  }
}

/**
 * Allows modules to alter a customer data as it is being built from order data.
 *
 * @param type $customer
 *  Customer data structure
 *  @see commerce_qb_webconnect_get_order_customer()
 *
 * @param type $order
 *  Order being used to build the customer
 */
function hook_commerce_qb_webconnect_order_customer_alter(&$customer, $order) {
  $phone_field_instance_id = variable_get('commerce_qb_webconnect_customer_phone_field', NULL);

  // Add the phone field to the customer array.
  if ($phone_field_instance_id) {
    $phone_field_instances = field_read_instances(array('id' => $phone_field_instance_id));

    if ($phone_field_instances) {
      $phone_field_instance = array_pop($phone_field_instances);
      $profile_info = $customer[$phone_field_instance['bundle']];

      if (isset($profile_info->{$phone_field_instance['field_name']}['und'])) {
        $customer['phone'] = $profile_info->{$phone_field_instance['field_name']}['und'][0]['value'];
      }
    }
  }
}

/**
 * Allow modules to alter the list of products that is being exported to
 * quickbooks in an order-type export (e.g. add_invoice, add_sales_receipt).
 *
 * @param type $products
 *  List of products
 *
 * @param type $order
 *  Order entity
 */
function hook_commerce_qb_webconnect_order_products_alter(&$products, $order) {

  // Only export products to quickbooks if the price is greater than $100.
  foreach ($products as $product_id => $product) {
    $lang = field_language('commerce_product', $product, 'commerce_product_price');
    $amount = $product->commerce_product_price[$lang][0]['amount'];

    if (commerce_currency_amount_to_decimal($amount, 'USD') ) {
      unset($products[$product_id]);
    }
  }
}

/**
 * Allow modules to alter the
 *
 * @param string $tax_account
 *  The sales tax type used for this order
 *
 * @param type $data
 */
function hook_commerce_qb_webconnect_sales_tax_account_alter(&$tax_account, $data) {

  // Check customer profile shipping state, and if it is Michigan, set the tax account
  // to 'MI Sales Tax'
  $rates = array(
    'MI' => 'MI Sales Tax'
  );

  $profile = commerce_customer_profile_load($order->commerce_customer_shipping['und'][0]['profile_id']);

  if ($profile && $profile->commerce_customer_address['und'][0]['administrative area']) {

    $state = $profile->commerce_customer_address['und'][0]['administrative area'];

    if (isset($rates[$state])) {
      $tax_account = $rates[$state];
    }
  }

  if (!isset($tax_account)) {
    $tax_account = 'No Sales Tax';
  }
}

/**
 * Provide information about types of Quickbooks exports.
 *
 * @return array
 *  Elements:
 *
 *  - qbxml_callback: args = $data array representation of data saved as JSON in
 *     export->data. Use this to build a string of valid QBXML that
 *     Quickbooks will receive when the export is queried by the QBWC
 *     client. (string, required)
 *
 *  - unique_id_callback: args = $data; same as above. Must return a string that
 *    uniquely identifies the export to prevent duplicates. (string, required)
 *
 *  - label_callback: args = $data; same as above. Return a string to be used as
 *    a label for an individual export. (string, optional)
 *
 *  - label: Human readable name of the export type. (string, required)
 *
 *  - uri_callback: args = $data; same as above. Use $data to build a path to
 *    use for links to this export's data reference in the UI (e.g. in a view).
 *    (string, optional)
 *
 *  - weight: Some Quickbooks items must be exported before others (e.g. a
 *    customer must come before orders referencing that customer). Use weight
 *    to control export order. (int, required)
 *
 *  - order_export: Flag to determine whether or not export type is meant to
 *    represent an order. Necessary for certain parts of the configuration UI.
 *    (boolean, optional)
 */
function hook_commerce_qb_webconnect_export_info() {
  // label_callback and uri_callback is the same as add invoice
  $exports = array(
    'add_sales_receipt' => array(
      'qbxml_callback' => 'commerce_qb_webconnect_add_sales_receipt_qbxml',
      'unique_id_callback' => 'commerce_qb_webconnect_add_sales_receipt_unique_id',
      'label_callback' => 'commerce_qb_webconnect_add_invoice_label', 
      'label' => t('Add sales receipt'),  
      'uri_callback' => 'commerce_qb_webconnect_add_invoice_uri',
      'weight' => 0,
      'order_export' => true
    ),
  );

  return $exports;
}

/**
 * Allow modules to alter the result of hook_commerce_qb_webconnect_export_info().
 *
 * @param array $info
 *  @see hook_commerce_qb_webconnect_export_info()
 */
function hook_commerce_qb_webconnect_export_info_alter(&$info) {
  $info['add_sales_receipt_label_callback'] = 'MYMODULE_add_sales_receipt_label';
}

/**
 * Allow modules to alter the final string of QBXML right before it is return to
 * the client
 *
 * @param string $qbxml
 *  String of QBXML
 *
 * @param type $export_type
 *  Type of export being processed
 *
 * @param type $export
 *  Export object being processed
 */
function hook_commerce_qb_webconnect_qbxml_alter($qbxml, $export_type, $export) {
  $xml = simplexml_load_string($qbxml);

  foreach ($xml->children() as $child) {
    $child->addAttribute('onError', 'stopOnError');
  }
}
