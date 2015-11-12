<?php
/**
 * @file
 *   commerce_unleashed_product.rules.inc
 */

/**
 * Impliments hook_rules_action_info().
 */
function commerce_unleashed_product_rules_action_info() {
  $actions = array();

  $actions['commerce_unleased_product_post'] = array(
    'label' => t('Post a product to Unleashed'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_product',
        'label' => t('Product'),
        'save' => TRUE,
      ),
    ),
    'group' => t('Commerce Unleashed'),
    'callbacks' => array(
      'execute' => 'commerce_unleased_product_post',
    ),
  );

  return $actions;
}
