<?php
/**
 * @file
 * Rules module integration.
 */

/**
 * Implements hook_rules_action_info().
 */
function webducateapi_commerce_rules_action_info() {
  $actions = array();

  $actions['webducateapi_commerce_create_token'] = array(
    'label' => t('Create token'),
    'base' => 'rules_actions_webducateapi_commerce_create_token',
    'group' => t('WebducateAPI - Commerce'),
    'parameter' => array(
      'line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Commerce line item'),
      ),
    ),
  );

  return $actions;
}

/**
 * Rules action callback for 'webducateapi_commerce_create_token'.
 */
function rules_actions_webducateapi_commerce_create_token($line_item) {
  $product = commerce_product_load($line_item->commerce_product[LANGUAGE_NONE][0]['product_id']);
  if ('webducate_course' == $product->type) {
    $order = commerce_order_load($line_item->order_id);
    webducateapi_create_token($product->sku, $order->mail);
  }
}
