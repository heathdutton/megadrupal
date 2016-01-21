<?php

/**
 * @file
 * Rules integration for line items.
 *
 * @addtogroup rules
 * @{
 */


/**
 * Implements hook_rules_action_info().
 */
function commerce_pricelist_rules_action_info() {
$actions = array();

  $actions['commerce_pricelist_rate_apply'] = array(
    'label' => t('Apply a pricelist rate to a line item'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
        'getter callback' => 'entity_property_verbatim_get',
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
      ),
    ),
    'provides' => array(
      'commerce_pricelist_price_array' => array(
        'type' => 'list<commerce_price>',
        'label' => t('Commerce Pricelist price')
      ),
    ),
    'group' => t('Commerce pricelist'),
    'callbacks' => array(
      'execute' => 'commerce_pricelist_rate_rules_apply',
    ),
  );

  return $actions;
}

/**
 * Rules action: loads and applies a pricelist rate to the given line item.
 */
function commerce_pricelist_rate_rules_apply($line_item, $user = FALSE) {
  $commerce_pricelist_price_array = array();
  // @todo commerce_product is maybe just one of many possible product types?
  $product_id = $line_item->commerce_product['und'][0]['product_id'];
  if ($product_id) {
    // We have a regular commerce product.
    $quantity = $line_item->quantity;
    $product = commerce_product_load($product_id);
    $sku = $product->sku;

    // Default to current user
    if (!$user) {
      global $user;
    }

    $time = ($line_item->created == "") ? FALSE : $line_item->created;
    $pricelist_price = commerce_pricelist_get_user_price($user, $sku, $quantity, $time);

    // If pricelist was applied, return the price array as a new variable for use in
    // subsequent actions.
    if ($pricelist_price) {
      $commerce_pricelist_price_array[] = $pricelist_price;
    }
  }

  return array('commerce_pricelist_price_array' => $commerce_pricelist_price_array);
}

/**
 * @}
 */
