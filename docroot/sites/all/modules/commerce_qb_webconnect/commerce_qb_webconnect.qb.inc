<?php

/*
 * Implements hook_commerce_qb_webconnect_export_info().
 */
function commerce_qb_webconnect_commerce_qb_webconnect_export_info() {
  $path = drupal_get_path('module', 'commerce_qb_webconnect') . '/qbxml';

  $exports = array(
    'add_customer' => array(
      'callback' => 'commerce_qb_webconnect_add_customer_variables',
      'template' => 'add_customer.qbxml',
      'template_path' => $path,
      'template_vars' => commerce_qb_webconnect_customer_vars(),
      'unique_id_callback' => 'commerce_qb_webconnect_add_customer_unique_id',
      'label_callback' => 'commerce_qb_webconnect_add_customer_label',
      'label' => t('Add customer'),
      'weight' => -2
    ),
    'add_inventory_product' => array(
      'callback' => 'commerce_qb_webconnect_add_product_variables',
      'template' => 'add_inventory_product.qbxml',
      'template_path' => $path,
      'template_vars' => commerce_qb_webconnect_product_vars(),
      'unique_id_callback' => 'commerce_qb_webconnect_add_product_unique_id',
      'label_callback' => 'commerce_qb_webconnect_add_product_label',  
      'label' => t('Add inventory product'),  
      'uri_callback' => 'commerce_qb_webconnect_add_product_uri',
      'weight' => -1,
    ),
    'add_non_inventory_product' => array(
      'callback' => 'commerce_qb_webconnect_add_product_variables',
      'template' => 'add_non_inventory_product.qbxml',
      'template_path' => $path,
      'template_vars' => commerce_qb_webconnect_product_vars(),
      'unique_id_callback' => 'commerce_qb_webconnect_add_product_unique_id',
      'label_callback' => 'commerce_qb_webconnect_add_product_label',  
      'label' => t('Add non-inventory product'),
      'uri_callback' => 'commerce_qb_webconnect_add_product_uri',
      'weight' => -1,
    ),
    'add_invoice' => array(
      'callback' => 'commerce_qb_webconnect_add_order_variables',
      'template' => 'add_invoice.qbxml',
      'template_path' => $path,
      'template_vars' => commerce_qb_webconnect_order_vars(),  
      'unique_id_callback' => 'commerce_qb_webconnect_add_invoice_unique_id',
      'label_callback' => 'commerce_qb_webconnect_add_invoice_label', 
      'label' => t('Add invoice'),  
      'uri_callback' => 'commerce_qb_webconnect_add_invoice_uri',
      'weight' => 0, // @TODO it should be able to interpret this without specifying 0
      'order_export' => TRUE
    ),
    'add_sales_receipt' => array(
      'callback' => 'commerce_qb_webconnect_add_order_variables',
      'template' => 'add_sales_receipt.qbxml',
      'template_path' => $path,
      'template_vars' => commerce_qb_webconnect_order_vars(),         
      'unique_id_callback' => 'commerce_qb_webconnect_add_sales_receipt_unique_id',
      'label_callback' => 'commerce_qb_webconnect_add_invoice_label', // same as add invoice
      'label' => t('Add sales receipt'),  
      'uri_callback' => 'commerce_qb_webconnect_add_invoice_uri', // same as add invoice
      'weight' => 0,
      'order_export' => TRUE
    ),
    'add_payment' => array(
      'callback' => 'commerce_qb_webconnect_add_payment_variables',
      'template' => 'add_payment.qbxml',
      'template_path' => $path,
      'template_vars' => commerce_qb_webconnect_payment_vars(),       
      'unique_id_callback' => 'commerce_qb_webconnect_add_payment_unique_id',
      'label_callback' => 'commerce_qb_webconnect_add_payment_label',
      'label' => t('Add payment'),  
      'weight' => 1,
    )
  );

  return $exports;
}

function commerce_qb_webconnect_product_vars() {
  return array(
    'accounts' => array(),
    'price' => NULL,
    'sku' => NULL,
    'parent_sku' => NULL,
    'title' => NULL,
    'product_id' => NULL
  );
}

function commerce_qb_webconnect_order_vars() {
  return array(
    'order_id' => NULL,
    'billing_address' => array(),
    'shipping_address' => array(),
    'tax_type' => NULL,
    'products' => array(),
    'shipping' => array(),
    'payment_method' => NULL
  );
}

function commerce_qb_webconnect_customer_vars() {
  return array(
    'data' => NULL,
    'has_shipping' => false,
    'shipping' => array(),
    'billing' => array()
  );
}

function commerce_qb_webconnect_payment_vars() {
  return array(
    'date' => NULL,
    'customer_name' => NULL,
    'ref_number' => NULL,
    'amount' => 0,
    'payment_method' => NULL
  );
}

/*
 * Unique id callback for add customer
 */
function commerce_qb_webconnect_add_customer_unique_id($data) {
  $lang = field_language('commerce_customer_profile', $data['billing'], 'commerce_customer_address');
  $address = $data['billing']->commerce_customer_address[$lang][0];
  return 'customer_' . str_replace(' ', '_', $address['name_line']);
}

/*
 * Unique id callback for add product
 */
function commerce_qb_webconnect_add_product_unique_id($data) {
  return 'product_' . $data->product_id;
}

/*
 * Unique id callback for add order
 */
function commerce_qb_webconnect_add_invoice_unique_id($data) {
  return 'invoice_' . $data->order_id;
}

/*
 * Unique id callback for add sales receipt
 */
function commerce_qb_webconnect_add_sales_receipt_unique_id($data) {
  return 'sales_receipt_' . $data->order_id;
}

/*
 * Unique id callback for add payment
 */
function commerce_qb_webconnect_add_payment_unique_id($data) {
  return 'payment_' . $data->transaction_id;
}

/*
 * Label callback for add customer
 */
function commerce_qb_webconnect_add_customer_label($data) {
  $lang = array_keys($data['billing']['commerce_customer_address']);
  $lang = reset($lang);
  return check_plain($data['billing']['commerce_customer_address'][$lang][0]['name_line']);
}

/*
 * Label callback for add product
 */
function commerce_qb_webconnect_add_product_label($data) {
  return check_plain($data['title']);
}

/*
 * Label callback for add invoice
 */
function commerce_qb_webconnect_add_invoice_label($data) {
  return t('Order') . ' ' . check_plain($data['order_id']);
}

function commerce_qb_webconnect_add_payment_label($data) {
  return t('Transaction') . ' ' . check_plain($data['transaction_id']);
}

/*
 * URI callback for add invoice
 */
function commerce_qb_webconnect_add_invoice_uri($data) {
  $order = commerce_order_load($data['order_id']);
  if ($order) {
    $uri = entity_uri('commerce_order', $order);

    if ($uri) {
      return $uri['path'];
    }
  }
}

/*
 * URI callback for add product
 */
function commerce_qb_webconnect_add_product_uri($data) {
  $product = commerce_product_load($data['product_id']);
  if ($product){
    $uri = entity_uri('commerce_product', $product);

    if ($uri) {
      return $uri['path'];
    }
  }
}

/*
 * QBXML callback for add customer.
 */
function commerce_qb_webconnect_add_customer_variables($data) {
  $lang = array_keys($data['billing']['commerce_customer_address']);
  $lang = reset($lang);
  $billing = $data['billing']['commerce_customer_address'][$lang][0];
  $billing_name = explode(' ', $data['billing']['commerce_customer_address'][$lang][0]['name_line']);
  $billing['first_name'] = $billing_name[0];
  $billing['last_name'] = $billing_name[1];
  $variables['billing'] = $billing;

  if (isset($data['shipping'])) {
    $variables['has_shipping'] = TRUE;
    $shipping_lang = _commerce_qb_webconnect_get_lang($data['shipping']['commerce_customer_address']);
    $variables['shipping'] = $data['shipping']['commerce_customer_address'][$shipping_lang][0];
  }

  $variables['data']['email'] = $data['email'];
  $variables['data']['phone'] = $data['phone'];

  return $variables;
}

/*
 * QBXML callback for add product
 */
function commerce_qb_webconnect_add_product_variables($data, $export_type) {
  $variables['accounts'] = array(
    'income' => variable_get('commerce_qb_webconnect_income_account', ''),
    'assets' => variable_get('commerce_qb_webconnect_assets_account', ''),
    'cogs' => variable_get('commerce_qb_webconnect_cogs_account', '')
  );

  // determine price
  $lang = _commerce_qb_webconnect_get_lang($data['commerce_price']);
  $amount = $data['commerce_price'][$lang][0]['amount'];
  $variables['price'] = commerce_currency_amount_to_decimal($amount, 'USD');

  // parse the SKU to account for parent-child relationships
  $sku = commerce_qb_webconnect_parse_sku($data['sku']);
  $variables['sku'] = $sku['sku'];

  if (isset($sku['parent'])) {
    $variables['parent_sku'] = $sku['parent'];
  }

  $variables['title'] = $data['title'];
  $variables['product_id'] = $data['product_id']; 

  return $variables;
}

/*
 * QBXML callback for add order
 */
function commerce_qb_webconnect_add_order_variables($data, $export_type) {
  $tax = '';

  // load customer profile
  $lang = _commerce_qb_webconnect_get_lang($data['commerce_customer_billing']);
  $profile_id = $data['commerce_customer_billing'][$lang][0]['profile_id'];
  $profile = commerce_customer_profile_load($profile_id);
  $profile_info = $profile->commerce_customer_address[$lang][0];

  $billing_name = explode(' ', $profile->commerce_customer_address[$lang][0]['name_line']);
  $variables['first_name'] = $billing_name[0];
  $variables['last_name'] = $billing_name[1];

  $variables['billing_address'] = $profile_info;
  $variables['order_id'] = $data['order_id'];
  $variables['date'] = format_date($data['created'], 'custom', 'Y-m-d');
  $order = commerce_order_load($data['order_id']);

  // sales tax account
  $tax_account = variable_get('commerce_qb_default_sales_tax_account', '');

  // allow modules to alter which sales tax account gets used
  drupal_alter('commerce_qb_webconnect_sales_tax_account', $tax_account, $order);

  $variables['tax_type'] = $tax_account;

  // load products
  $products = commerce_qb_webconnect_line_item_products($order);  

  // allow modules to alter the list of products being exported with this order
  drupal_alter('commerce_qb_webconnect_order_products', $products, $data);

  if (!empty($products)) {

    foreach ($products as $product) {
      $price_lang = _commerce_qb_webconnect_get_lang($product->commerce_price);
      $price_array = NULL;
      $price_array = $product->commerce_price[$price_lang][0];    
      $price = commerce_currency_amount_to_decimal($price_array['amount'], $price_array['currency_code']);

      $variables['products'][] = array(
        'price' => $price,
        'sku' => str_replace('_', ' ', $product->sku),
        'title' => $product->title,
        'quantity' => $product->quantity,
      );
    }

    // assess shipping costs
    $shipping = variable_get('commerce_qb_webconnect_shipping_service', '');
    $shipping_line_item = commerce_qb_webconnect_line_item_load($data['order_id'], 'shipping', TRUE);

    if ($shipping && $shipping_line_item) {
      $shipping_line_item_lang = _commerce_qb_webconnect_get_lang($shipping_line_item->commerce_unit_price);
      $price_array = $shipping_line_item->commerce_unit_price[$shipping_line_item_lang][0];
      $price = commerce_currency_amount_to_decimal($price_array['amount'], $price_array['currency_code']);

      $variables['shipping'] = array(
        'service' => $shipping,
        'description' => variable_get('commerce_qb_webconnect_shipping_service_description', ''),
        'quantity' => $shipping_line_item->quantity,
        'rate' => $price
      );
    }
  }

  // find shipping address if it is available
  if (module_exists('commerce_shipping')) {
    $shipping_profile_lang = _commerce_qb_webconnect_get_lang($data['commerce_customer_shipping']);
    $shipping_profile_id = $data['commerce_customer_shipping'][$shipping_profile_lang][0]['profile_id'];
    $shipping_profile = commerce_customer_profile_load($shipping_profile_id);
    $shipping_profile_info = $shipping_profile->commerce_customer_address[$shipping_profile_lang][0];
    $variables['shipping_address'] = $shipping_profile_info;
  }

  // find payment method if applicable
  if (isset($data['data']['payment_method'])) {
    $payment_method_instance = explode('|', $data['data']['payment_method']);
    $payment_method = $payment_method_instance[0];
    $variables['payment_method'] = commerce_qb_webconnect_payment_method_mapping($payment_method, 'add_payment', $data);
  }

  return $variables;
}

/*
 * QBXML callback for add payment
 */
function commerce_qb_webconnect_add_payment_variables($data) {
  $order = commerce_order_load($data['order_id']);
  $lang = _commerce_qb_webconnect_get_lang($order->commerce_customer_billing);

  // load customer profile
  $profile_id = $order->commerce_customer_billing[$lang][0]['profile_id'];
  $profile = commerce_customer_profile_load($profile_id);
  $variables['customer_name'] = $profile->commerce_customer_address[$lang][0]['name_line'];

  $variables['payment_method'] = commerce_qb_webconnect_payment_method_mapping($data['payment_method'], 'add_payment', $data);

  // amount
  $variables['amount'] = commerce_currency_amount_to_decimal($data['amount'], 'USD');

  // date
  $variables['date'] = format_date($data['created'], 'custom', 'Y-m-d');

  // ref number
  $variables['ref_number'] = 'ct-' . $data['order_id'];

  return $variables;
}

/*
 * Implements hook_commerce_qb_webconnect_order_customer_alter().
 */
function commerce_qb_webconnect_commerce_qb_webconnect_order_customer_alter(&$customer, $order) {
  $phone_field_instance_id = variable_get('commerce_qb_webconnect_customer_phone_field', NULL);

  // add the phone field to the customer array
  if ($phone_field_instance_id) {
    $phone_field_instances = field_read_instances(array('id' => $phone_field_instance_id));

    if ($phone_field_instances) {
      $phone_field_instance = array_pop($phone_field_instances);
      $profile_info = $customer[$phone_field_instance['bundle']];
      $lang = _commerce_qb_webconnect_get_lang($profile_info->{$phone_field_instance['field_name']});

      if (isset($profile_info->{$phone_field_instance['field_name']}[$lang])) {
        $customer['phone'] = $profile_info->{$phone_field_instance['field_name']}[$lang][0]['value'];
      }
    }
  }
}

/*
 * Implements hook_MODULE_NAME_commerce_qb_webconnect_payment_mapping on behalf of commerce_payment_example module
 */
function commerce_payment_example_commerce_qb_webconnect_payment_mapping($payment_type, $export_type, $data) {
  switch ($payment_type) {
    case 'commerce_payment_example':
      switch($export_type) {
        case 'add_sales_receipt':
        case 'add_payment':
          return variable_get('commerce_qb_webconnect_example_payment_method', '');
    
          break;
      }

      break;
  }
}
