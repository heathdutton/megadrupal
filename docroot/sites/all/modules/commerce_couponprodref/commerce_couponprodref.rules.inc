<?php
/**
 * @file
 * Contains Rules hooks, callbacks and helper functions for Commerce Coupon by
 * product reference.
 */

/**
 * Implements hook_rules_condition_info()
 */
function commerce_couponprodref_rules_condition_info() {
  $conditions = array();

  $conditions['commerce_couponprodref_order_has_referenced_product'] = array(
    'label' => t('Order contains a product that is referenced by the coupon.'),
    'group' => t('Commerce Coupon'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'description' => t('Specifies the order to check.'),
      ),
      'commerce_coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('Coupon'),
        'description' => t("Specifies the coupon with the product reference field."),
      ),
      'field' => array(
        'type' => 'text',
        'label' => t("Field"),
        'description' => t("Specifies the product reference field that should be used."),
        'options list' => 'commerce_couponprodref_product_field_options',
        'restriction' => 'input',
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_couponprodref_condition_order_has_referenced_product',
    ),
  );

  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_couponprodref_rules_action_info() {
  $actions['commerce_couponprodref_action_get_referencing_coupons_by_product'] = array(
    'label' => t("Get order coupons referencing product"),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t("Commerce order"),
      ),
      'commerce_product' => array(
        'type' => 'commerce_product',
        'label' => t("Commerce product")
      ),
      'field' => array(
        'type' => 'text',
        'label' => t("Field"),
        'description' => t("Specifies the product reference field that should be used."),
        'options list' => 'commerce_couponprodref_product_field_options',
        'restriction' => 'input',
      ),
    ),
    'provides' => array(
      'order_referencing_coupons' => array(
        'type' => 'list<commerce_coupon>',
        'label' => t("Coupons attached to this order that reference the product"),
      ),
    ),
    'group' => t('Commerce Coupon'),
  );

  return $actions;
}

/**
 * Rule condition callback for checking if an order contains products referenced
 * by a coupon.
 * 
 * @return
 *   TRUE if $order contains a product referenced by $field on $commerce_coupon;
 *   FALSE otherwise.
 */
function commerce_couponprodref_condition_order_has_referenced_product($order, $commerce_coupon, $field) {
  
  // Build list of referenced product IDs.
  $coupon_wrapper = entity_metadata_wrapper('commerce_coupon', $commerce_coupon);
  
  if (isset($coupon_wrapper->{$field})) {
  
    $products = array();
    foreach ($coupon_wrapper->{$field} as $product) {
      $id = $product->product_id->value();
      $products[$id] = $id;
    }
    
    // Go through line items looking for a match.
    if ($products) {
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
        if (isset($line_item_wrapper->commerce_product) && isset($line_item_wrapper->commerce_product->product_id)) {
          $id = $line_item_wrapper->commerce_product->product_id->value();
          if (isset($products[$id])) {
            // At least one product matches.
            return TRUE;
          }
        }
      }
    }
  }
  
  // Product wasn't found.
  return FALSE;
}

/**
 * Rules action callback that provides a list of coupons attached to the order
 * that reference the product.
 *
 * @see commerce_couponprodref_get_referencing_coupons_by_product()
 */
function commerce_couponprodref_action_get_referencing_coupons_by_product($order, $product, $field) {
  if (!$order || !$product || !$field) {
    return array();
  }
  return array(
    'order_referencing_coupons' =>
      commerce_couponprodref_get_referencing_coupons_by_product(
        $order,
        $product->product_id,
        $field
      )
  );
}

/**
 * Options list callback that returns a list of fields that are attached to the
 * entity commerce_coupon and could reference a product.
 */
function commerce_couponprodref_product_field_options() {
  
  $fields = array();
  
  foreach (field_info_fields() as $field) {
  
    // Check if the field is attached to the entity commerce_coupon.
    if (isset($field['bundles']['commerce_coupon'])) {
    
      // The field is considered a valid reference field if *any* of the
      // following are true:
      // a) it's a product reference field;
      // b) it's an entity reference field set to target commerce_product
      //    entities;
      // c) it's an entity reference field that uses a View for its source.
      $type = $field['type'];
      
      $valid_product_reference = ($type == 'commerce_product_reference');
      $valid_entity_reference = ($type == 'entityreference') && in_array($field['settings']['target_type'], array('commerce_product', 'view'));
      
      if ($valid_product_reference || $valid_entity_reference) {
        $fields[$field['field_name']] = $field['field_name'];
      }
    }
  }
  
  return $fields;
}
