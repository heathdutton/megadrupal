<?php
/**
 * @file
 * Contains hooks/functions relating to Rules integration.
 */

/**
 * Implements hook_rules_action_info().
 */
function shopify_rules_action_info() {
  return array(
    'shopify_api_call_action' => array(
      'label' => t('Shopify API call'),
      'group' => t('Shopify'),
      'access callback' => 'shopify_rules_api_call_action_access',
      'parameter' => array(
        'method' => array(
          'type' => 'text',
          'label' => t('Method'),
          'description' => t('API method: GET, POST, PUT, DELETE'),
        ),
        'path' => array(
          'type' => 'text',
          'label' => t('Path'),
          'description' => t('API path to resource. For a path of /admin/products.json, only type "products".'),
        ),
        'data' => array(
          'type' => 'text',
          'label' => t('Data (JSON)'),
          'description' => t('Data to pass the API in JSON format.'),
        ),
      ),
    ),
  );
}

/**
 * Access callback for the Shopify API rules action.
 */
function shopify_rules_api_call_action_access($type, $name) {
  // Only allow Shopify admins to use this action.
  return user_access('administer shopify');
}

/**
 * Callback for Shopify rules action.
 */
function shopify_api_call_action($method, $path, $data) {
  return shopify_api_call($method, $path, drupal_json_decode($data));
}

/**
 * Implements hook_rules_event_info().
 */
function shopify_rules_event_info() {
  return array(
    'shopify_webhook_fired' => array(
      'label' => t('Webhook fired'),
      'module' => 'shopify',
      'group' => t('Shopify Webhook'),
      'variables' => array(
        'hook' => array(
          'type' => 'text',
          'label' => t('Hook'),
        ),
      ),
    ),
    'shopify_webhook_product_created' => array(
      'label' => t('Product created'),
      'module' => 'shopify',
      'group' => t('Shopify Webhook'),
      'variables' => array(
        'shopify_product' => array(
          'type' => 'shopify_product',
          'label' => t('Shopify Product'),
        ),
      ),
    ),
    'shopify_webhook_product_updated' => array(
      'label' => t('Product updated'),
      'module' => 'shopify',
      'group' => t('Shopify Webhook'),
      'variables' => array(
        'shopify_product' => array(
          'type' => 'shopify_product',
          'label' => t('Shopify Product'),
        ),
      ),
    ),
    'shopify_webhook_product_deleted' => array(
      'label' => t('Product deleted'),
      'module' => 'shopify',
      'group' => t('Shopify Webhook'),
      'variables' => array(
        'shopify_product' => array(
          'type' => 'shopify_product',
          'label' => t('Shopify Product'),
        ),
      ),
    ),
    'shopify_webhook_collection_created' => array(
      'label' => t('Collection created'),
      'module' => 'shopify',
      'group' => t('Shopify Webhook'),
      'variables' => array(
        'taxonomy_term' => array(
          'type' => 'taxonomy_term',
          'label' => t('Collection'),
        ),
      ),
    ),
    'shopify_webhook_collection_updated' => array(
      'label' => t('Collection updated'),
      'module' => 'shopify',
      'group' => t('Shopify Webhook'),
      'variables' => array(
        'taxonomy_term' => array(
          'type' => 'taxonomy_term',
          'label' => t('Collection'),
        ),
      ),
    ),
    'shopify_webhook_collection_deleted' => array(
      'label' => t('Collection deleted'),
      'module' => 'shopify',
      'group' => t('Shopify Webhook'),
      'variables' => array(
        'taxonomy_term' => array(
          'type' => 'taxonomy_term',
          'label' => t('Collection'),
        ),
      ),
    ),
    'shopify_webhook_order_paid' => array(
      'label' => t('Order paid'),
      'module' => 'shopify',
      'group' => t('Shopify Webhook'),
      'variables' => shopify_rules_webhook_order_paid_variables(),
    ),
  );
}

/**
 * Gets variables for the order_paid webhook rule.
 */
function shopify_rules_webhook_order_paid_variables() {
  return array(
    'buyer_accepts_marketing' => array(
      'type' => 'boolean',
      'label' => t('Buyer accepts marketing'),
    ),
    'cancel_reason' => array(
      'type' => 'text',
      'label' => t('Cancel reason'),
    ),
    'cancelled_at' => array(
      'type' => 'date',
      'label' => t('Cancelled at'),
    ),
    'cart_token' => array(
      'type' => 'text',
      'label' => t('Cart token'),
    ),
    'checkout_token' => array(
      'type' => 'text',
      'label' => t('Checkout token'),
    ),
    'closed_at' => array(
      'type' => 'date',
      'label' => t('Closed at'),
    ),
    'confirmed' => array(
      'type' => 'boolean',
      'label' => t('Confirmed'),
    ),
    'created_at' => array(
      'type' => 'date',
      'label' => t('Created at'),
    ),
    'currency' => array(
      'type' => 'text',
      'label' => t('Currency'),
    ),
    'email' => array(
      'type' => 'text',
      'label' => t('Email'),
    ),
    'financial_status' => array(
      'type' => 'text',
      'label' => t('Financial status'),
    ),
    'id' => array(
      'type' => 'integer',
      'label' => t('ID'),
    ),
    'landing_site' => array(
      'type' => 'text',
      'label' => t('Landing site'),
    ),
    'name' => array(
      'type' => 'text',
      'label' => t('Name'),
    ),
    'note' => array(
      'type' => 'text',
      'label' => t('Note'),
    ),
    'number' => array(
      'type' => 'integer',
      'label' => t('Number'),
    ),
    'processed_at' => array(
      'type' => 'date',
      'label' => t('Processed at'),
    ),
    'referring_site' => array(
      'type' => 'text',
      'label' => t('Referring site'),
    ),
    'subtotal_price' => array(
      'type' => 'decimal',
      'label' => t('Subtotal price'),
    ),
    'taxes_included' => array(
      'type' => 'boolean',
      'label' => t('Taxes included'),
    ),
    'test' => array(
      'type' => 'boolean',
      'label' => t('Test'),
    ),
    'token' => array(
      'type' => 'text',
      'label' => t('Token'),
    ),
    'total_discounts' => array(
      'type' => 'decimal',
      'label' => t('Total discounts'),
    ),
    'total_line_items_price' => array(
      'type' => 'decimal',
      'label' => t('Total line items price'),
    ),
    'total_price' => array(
      'type' => 'decimal',
      'label' => t('Total price'),
    ),
    'total_tax' => array(
      'type' => 'decimal',
      'label' => t('Total tax'),
    ),
    'total_weight' => array(
      'type' => 'integer',
      'label' => t('Total weight'),
    ),
    'updated_at' => array(
      'type' => 'date',
      'label' => t('Updated at'),
    ),
    'browser_ip' => array(
      'type' => 'text',
      'label' => t('Browser IP'),
    ),
    'order_number' => array(
      'type' => 'integer',
      'label' => t('Order number'),
    ),
    'source' => array(
      'type' => 'text',
      'label' => t('Source'),
    ),
    'fulfillment_status' => array(
      'type' => 'text',
      'label' => t('Fulfillment status'),
    ),
    'tags' => array(
      'type' => 'text',
      'label' => t('Tags'),
    ),
    'customer' => array(
      'type' => 'struct',
      'label' => t('Customer'),
      'property info' => shopify_rules_customer_property_info(),
    ),
    'billing_address' => array(
      'type' => 'struct',
      'label' => t('Billing address'),
      'property info' => shopify_rules_address_property_info(),
    ),
    'shipping_address' => array(
      'type' => 'struct',
      'label' => t('Shipping address'),
      'property info' => shopify_rules_address_property_info(),
    ),
    'shipping_lines' => array(
      'type' => 'list<struct>',
      'label' => t('Shipping lines'),
      'property info' => shopify_rules_shipping_line_property_info(),
    ),
    'tax_lines' => array(
      'type' => 'list<struct>',
      'label' => t('Tax lines'),
      'property info' => shopify_rules_tax_line_property_info(),
    ),
    'line_items' => array(
      'type' => 'list<struct>',
      'label' => t('Line Items'),
      'property info' => shopify_rules_line_item_property_info(),
    ),
  );
}

/**
 * Property info for customer data.
 */
function shopify_rules_customer_property_info() {
  return array(
    'first_name' => array(
      'type' => 'text',
      'label' => t('First name'),
    ),
    'last_name' => array(
      'type' => 'text',
      'label' => t('Last name'),
    ),
    'accepts_marketing' => array(
      'type' => 'boolean',
      'label' => t('Accepts marketing'),
    ),
    'created_at' => array(
      'type' => 'date',
      'label' => t('Created at'),
    ),
    'email' => array(
      'type' => 'text',
      'label' => t('Email'),
    ),
    'id' => array(
      'type' => 'integer',
      'label' => t('ID'),
    ),
    'orders_count' => array(
      'type' => 'integer',
      'label' => t('Orders count'),
    ),
    'state' => array(
      'type' => 'text',
      'label' => t('State'),
    ),
    'total_spent' => array(
      'type' => 'decimal',
      'label' => t('Total spent'),
    ),
    'updated_at' => array(
      'type' => 'date',
      'label' => t('Updated at'),
    ),
    'verified_email' => array(
      'type' => 'boolean',
      'label' => t('Verified email'),
    ),
    'tags' => array(
      'type' => 'text',
      'label' => t('Tags'),
    ),
    'default_address' => array(
      'type' => 'struct',
      'label' => t('Address'),
      'property info' => shopify_rules_address_property_info(),
    ),
  );
}

/**
 * Property info for order shipping lines.
 */
function shopify_rules_shipping_line_property_info() {
  return array(
    'code' => array(
      'type' => 'text',
      'label' => t('Code'),
    ),
    'price' => array(
      'type' => 'decimal',
      'label' => t('Price'),
    ),
    'source' => array(
      'type' => 'text',
      'label' => t('Source'),
    ),
    'title' => array(
      'type' => 'text',
      'label' => t('Title'),
    ),
    'tax_lines' => array(
      'type' => 'list<struct>',
      'label' => t('Tax lines'),
      'property info' => shopify_rules_tax_line_property_info(),
    ),
  );
}

/**
 * Property info for order line items.
 */
function shopify_rules_line_item_property_info() {
  return array(
    'product_id' => array(
      'type' => 'integer',
      'label' => t('Product ID'),
    ),
    'variant_id' => array(
      'type' => 'integer',
      'label' => t('Variant ID'),
    ),
    'quantity' => array(
      'type' => 'integer',
      'label' => t('Quantity'),
    ),
    'gift_card' => array(
      'type' => 'boolean',
      'label' => t('Gift Card'),
    ),
    'price' => array(
      'type' => 'decimal',
      'label' => t('Price'),
    ),
    'requires_shipping' => array(
      'type' => 'boolean',
      'label' => t('Requires Shipping'),
    ),
    'sku' => array(
      'type' => 'text',
      'label' => t('SKU'),
    ),
    'taxable' => array(
      'type' => 'boolean',
      'label' => t('Taxable'),
    ),
    'title' => array(
      'type' => 'text',
      'label' => t('Title'),
    ),
    'vendor' => array(
      'type' => 'text',
      'label' => t('Vendor'),
    ),
    'product_exists' => array(
      'type' => 'boolean',
      'label' => t('Product exists'),
    ),
    'fulfillable_quantity' => array(
      'type' => 'integer',
      'label' => t('Fulfillable quantity'),
    ),
    'total_discount' => array(
      'type' => 'decimal',
      'label' => t('Total discount'),
    ),
    'id' => array(
      'type' => 'integer',
      'label' => t('Order ID'),
    ),
    'fulfillment_service' => array(
      'type' => 'text',
      'label' => t('Fulfillment service'),
    ),
    'fulfillment_status' => array(
      'type' => 'text',
      'label' => t('Fulfillment status'),
    ),
    'grams' => array(
      'type' => 'integer',
      'label' => t('Grams'),
    ),
  );
}

/**
 * Property info for order tax lines.
 */
function shopify_rules_tax_line_property_info() {
  return array(
    'price' => array(
      'type' => 'decimal',
      'label' => t('Price'),
    ),
    'rate' => array(
      'type' => 'decimal',
      'label' => t('Rate'),
    ),
    'title' => array(
      'type' => 'text',
      'label' => t('Title'),
    ),
  );
}

/**
 * Property info for order addresses.
 */
function shopify_rules_address_property_info() {
  return array(
    'address1' => array(
      'type' => 'text',
      'label' => t('Address 1'),
    ),
    'address2' => array(
      'type' => 'text',
      'label' => t('Address 2'),
    ),
    'city' => array(
      'type' => 'text',
      'label' => t('City')
    ),
    'company' => array(
      'type' => 'text',
      'label' => t('Company'),
    ),
    'country' => array(
      'type' => 'text',
      'label' => t('Country'),
    ),
    'phone' => array(
      'type' => 'text',
      'label' => t('Phone'),
    ),
    'province' => array(
      'type' => 'text',
      'label' => t('Province'),
    ),
    'zip' => array(
      'type' => 'text',
      'label' => t('Zip'),
    ),
    'name' => array(
      'type' => 'text',
      'label' => t('Name'),
    ),
    'province_code' => array(
      'type' => 'text',
      'label' => t('Province code'),
    ),
    'country_code' => array(
      'type' => 'text',
      'label' => t('Country code'),
    ),
  );
}

/**
 * Parses order args to reformat dates.
 */
function shopify_rules_parse_order_args($order) {
  // Need to format various variables as dates.
  $vars = shopify_rules_webhook_order_paid_variables();
  shopify_rules_parse_order_dates($order, $vars);
  shopify_rules_parse_order_empty_variables($order, $vars);
  return $order;
}

/**
 * Create and fill-in empty order vars.
 */
function shopify_rules_parse_order_empty_variables(&$order, $vars) {
  foreach ($vars as $key => $info) {
    if (!isset($order[$key])) {
      $order[$key] = FALSE;
    }
    if (isset($info['property info'])) {
      if ($info['type'] == 'list<struct>') {
        foreach ($order[$key] as $index => &$item) {
          shopify_rules_parse_order_empty_variables($item, $info['property info']);
        }
      }
      else {
        shopify_rules_parse_order_empty_variables($order[$key], $info['property info']);
      }
    }
  }
}

/**
 * Recursively go through the order object and set dates to be timestamps.
 */
function shopify_rules_parse_order_dates(&$data, $vars) {
  foreach ($data as $key => &$value) {
    if (is_numeric($key)) {
      // This is an array of arrays.
      shopify_rules_parse_order_dates($value, $vars);
      continue;
    }
    if (is_array($value) && $value) {
      // Parse the sub array.
      shopify_rules_parse_order_dates($value, $vars[$key]['property info']);
      continue;
    }
    if (isset($vars[$key]) && $vars[$key]['type'] == 'date') {
      // Reformat this as a date.
      $date = strtotime($data[$key]);
      $value = $date ? $date : 0;
    }
  }
}
