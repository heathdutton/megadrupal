<?php
/**
 * @file
 */


/**
 * Page callback: node/%node/dtc
 *
 * Using the querystring parameters, find the appropriate product variants for
 * the given product display and add them to the cart.
 *
 * @todo add full uri/querystring to watchdog logs.
 */
function commerce_dtc_add_to_cart($node) {
  global $user;

  $query_parameters = drupal_get_query_parameters();

  $display_path = 'node/' . $node->nid;

  // Make sure we're dealing with a product display node by looking for
  // field_product on the node.
  if (!isset($node->field_product) || empty($node->field_product)) {
    watchdog('commerce_dtc', 'Attempted a direct-to-cart call on node !nid. Unable to locate field_product(s) on the node.', array('!nid' => $node->nid));
    _commerce_dtc_goto_product_display($node, $query_parameters);
  }

  // Evaluate field settings to determine if products should be combined when
  // added to the cart.
  $field_info = field_info_instance('node', 'field_product', $node->type);
  $display_mode = !empty($field_info['display']['full']) ? 'full' : 'default';
  $combine = $field_info['display'][$display_mode]['settings']['combine'];
  $qty = !empty($query_parameters['qty']) ? $query_parameters['qty'] : $field_info['display'][$display_mode]['settings']['default_quantity'];
  $bundle = module_exists('commerce_bundle') && $field_info['display'][$display_mode]['module'] == 'commerce_bundle';

  $products_to_add = array();

  $node_wrapper = entity_metadata_wrapper('node', $node);

  // We first build an array of the product variants that we'll eventually add
  // to the cart.

  // Standard Product
  if (!$bundle) {
    $id = $node->nid;
    $expected_product_count = 1;

    if ($product_details = _commerce_dtc_find_product_details($node_wrapper->field_product, $query_parameters)) {
      $products_to_add[$id] = array(
        'product' => $product_details['product'],
        'qty' => $qty,
      );
    }
  }
  // Product Bundles (commerce_bundle)
  else {
    $expected_product_count = $node_wrapper->field_product->count();

    foreach ($node_wrapper->field_product as $bundle_group_wrapper) {
      $bundle_qty = $qty * $bundle_group_wrapper->commerce_bundle_unit_quantity->value();
      $id = $bundle_group_wrapper->getIdentifier();

      if ($product_details = _commerce_dtc_find_product_details($bundle_group_wrapper->commerce_bundle_items, $query_parameters, TRUE)) {
        $products_to_add[$id] = array(
          'product' => $product_details['product'],
          'bundle_item' => $product_details['bundle_item'],
          'bundle_group' => $bundle_group_wrapper->value(),
          'product_display' => $node,
          'qty' => $bundle_qty,
        );
      }
    }
  }

  // We were unable to location any product variants to add to the cart. This
  // could be due to several reasons.
  if (empty($products_to_add) || count($products_to_add) != $expected_product_count) {
    watchdog('commerce_dtc', 'Attempted a direct-to-cart call on node !nid. Unable to locate all product variants to add to the cart.', array('!nid' => $node->nid));
    _commerce_dtc_goto_product_display($node, $query_parameters);
  }

  // Add the products to the cart.
  // Standard Product
  if (!$bundle) {
    foreach ($products_to_add as $product_to_add) {
      $data['context'] = array(
        'add_to_cart_combine' => $combine,
        'display_path' => $display_path,
      );
      $line_item = commerce_product_line_item_new($product_to_add['product'], $product_to_add['qty'], 0, $data);
      $line_item = commerce_cart_product_add($user->uid, $line_item, $combine);
    }
  }
  // Bundled Products (commerce_bundle)
  else {
    $bundled_line_items = array();

    foreach ($products_to_add as $index => $product_details) {
      $data['context'] = array(
        'add_to_cart_combine' => $combine,
        'display_path' => $display_path,
      );

      $bundled_line_items[] = $products_to_add[$index]['line_item'] = commerce_bundle_product_line_item_new(
        $product_details['product'],
        $product_details['bundle_item'],
        $product_details['bundle_group'],
        $product_details['product_display'],
        $product_details['qty'],
        0,
        $data
      );
    }

    $bundle_configs = commerce_bundle_line_item_get_bundle_configs($bundled_line_items);
    foreach ($bundle_configs as $bundle_config_id => $bundle_config_line_items) {
      foreach ($bundle_config_line_items as $line_item) {
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
        if (isset($line_item_wrapper->commerce_bundle_config_id)) {
          $line_item_wrapper->commerce_bundle_config_id = $bundle_config_id;
        }
      }
    }

    foreach ($bundled_line_items as $delta => $line_item) {
      $bundled_line_items[$delta] = commerce_cart_product_add(
        $user->uid,
        $line_item,
        $combine
      );
    }
  }

  // Adding a product to the cart should automatically take the user to the cart
  // however in case we make it this far we'll automatically take them there.
  drupal_goto('cart');
}

/**
 * Helper function: Redirect to the product display page.
 *
 * If we're unable to add products directly to the cart then we need to land the
 * user on a page. Ideally that will be the product page. We pass along the
 * querystring parameters in case commerce_pdas (or similar) module is enabled
 * that would pre-select attribute fields, etc.
 */
function _commerce_dtc_goto_product_display($node, $query_parameters) {
  $display_path = 'node/' . $node->nid;
  $options = array('query' => $query_parameters);
  drupal_goto($display_path, $options);
}

/**
 * Helper Function: Find the product details in a product reference field.
 *
 * Look through an array of product variants to find the variant that matches
 * the provided querystring parameters. If no attributes are provided the
 * default variant (first) will be used. If one or more attributes fail on the
 * variant then FALSE is returned.
 *
 * @param  object  $product_field_wrapper
 *   Entity metadata wrapper of field_product.
 * @param  array  $query_parameters
 *   Array of querystring params provided by drupal_get_query_parameters().
 * @param  boolean $bundle
 *   Indicator if dealing with commerce_bundle products.
 *
 * @return array
 *   An array containing the product variant. Additional bundle details may be
 *   included.
 */
function _commerce_dtc_find_product_details($product_field_wrapper, $query_parameters, $bundle = FALSE) {
  $default_product_wrapper = !$bundle ? $product_field_wrapper[0] : $product_field_wrapper[0]->commerce_bundle_product;

  $attribute_querystring_values = _commerce_dtc_get_querystring_filtered_attribute_fields($query_parameters, $default_product_wrapper->type->value());

  $product_details = array(
    'product' => $default_product_wrapper->value(),
    'bundle_item' => $bundle ? $product_field_wrapper[0]->value() : FALSE,
  );

  if (empty($attribute_querystring_values)) {
    return $product_details;
  }

  foreach ($product_field_wrapper as $product_wrapper) {
    $prod_wrapper_to_evaluate = !$bundle ? $product_wrapper : $product_wrapper->commerce_bundle_product;

    $product_details = array(
      'product' => $prod_wrapper_to_evaluate->value(),
      'bundle_item' => $bundle ? $product_wrapper->value() : FALSE,
    );

    if (_commerce_dtc_do_product_attributes_match_querystring($prod_wrapper_to_evaluate, $attribute_querystring_values)) {
      return $product_details;
    }
  }

  return FALSE;
}


/**
 * Helper Function: Determine whether variant matches provided attributes.
 *
 * We evaluate each provided attribute. The first attribute that does not
 * match we return FALSE. If all attributes pass we return TRUE.
 *
 * @param  object $product_wrapper
 *   An entity metadata wrapper instance of a product entity.
 * @param  array $attribute_querystring_values
 *   An array of attribute fields and the associated querystring values.
 *
 * @return boolean
 *   Whether all attribute fields on the variant matches provided values.
 */
function _commerce_dtc_do_product_attributes_match_querystring($product_wrapper, $attribute_querystring_values) {
  foreach ($attribute_querystring_values as $field_name => $query_value) {
    if (!isset($product_wrapper->{$field_name}) || $product_wrapper->{$field_name}->getIdentifier() != $query_value && $query_value != COMMERCE_DTC_ONE_OPTION) {
      return FALSE;
    }
  }

  // All attributes passed.
  return TRUE;
}

/**
 * Helper Function: Get a map of querystring values to attribute field names.
 *
 * Starting with a list of all attribute fields, cycle through each field and
 * see if a querystring parameter is provided for that field. Return a list of
 * those querystring values for their fields.
 *
 * @param  array $query_parameters
 *   Array of querystring parameters as provided by drupal_get_query_parameters().
 * @param  string $product_type
 *   The product entity type to provide fields for.
 *
 * @return array
 *   An associative array of attribute field names and the querystring value.
 */
function _commerce_dtc_get_querystring_filtered_attribute_fields($query_parameters, $product_type) {
  $attribute_querystring_values = &drupal_static(__FUNCTION__);

  if (isset($attribute_querystring_values[$product_type])) {
    return $attribute_querystring_values[$product_type];
  }

  $attribute_querystring_values[$product_type] = array();

  $query_parameters = array_map('strtolower', $query_parameters);
  $attribute_fields = _commerce_dtc_get_cart_attribute_fields();

  foreach ($attribute_fields as $field_name => $attribute_field) {
    // We only care about attribute fields for this particular product type.
    if (!in_array($product_type, $attribute_field['product_types'])) {
      continue;
    }

    // Convert to lowercase to allow for case insensitive querystring values.
    $field_options = array_flip(array_map('strtolower', $attribute_field['options']));
    $short_name = _commerce_dtc_field_shortname($field_name);

    // Querystring found for shortname. Attach the querystring value to the
    // array for the field_name.
    if (!empty($query_parameters[$short_name])) {
      $attribute_querystring_values[$product_type][$field_name] = !empty($field_options[$query_parameters[$short_name]])? $field_options[$query_parameters[$short_name]] : FALSE;
    }
    // No querystring value provided however attribute only has one option so
    // we allow this to pass.
    elseif (empty($query_parameters[$short_name]) && count($field_options) == 1) {
      $attribute_querystring_values[$product_type][$field_name] = COMMERCE_DTC_ONE_OPTION;
    }
  }

  return $attribute_querystring_values[$product_type];
}
