<?php
/**
 * @file
 * Token integration for the Commerce Stock Reserve module.
 */

/**
 * Implements hook_token_info().
 */
function commerce_stock_reserve_token_info() {
  $info = array();
  $info['tokens']['commerce-product']['user-available-stock'] = array(
    'name' => t('Product stock available for a user'),
    'description' => t("The product's stock plus the amount the user has already reserved"),
  );
  return $info;
}

/**
 * Implements hook_tokens().
 */
function commerce_stock_reserve_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  if ($type == 'commerce-product' && !empty($data['commerce-product'])) {
    $product = $data['commerce-product'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'user-available-stock':
          $replacements[$original] = commerce_stock_reserve_get_user_available_amount($product);
          break;
      }
    }
  }
  return $replacements;
}
