<?php

/**
 * @file
 * Provides a token for the salsa donate page key in an order object.
 */

/**
 * Implements hook_token_info().
 */
function salsa_commerce_token_info() {
  // Tokens for dynamic sharing description.
  $info['tokens']['commerce-order']['salsa-commerce-donate-page-key'] = array(
    'name' => t('Donate page key'),
    'description' => t('The key of the donate page on which the order was created'),
  );
  return $info;
}

/**
 * Implements hook_tokens().
 */
function salsa_commerce_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'commerce-order' && !empty($data['commerce-order'])) {
    $order = $data['commerce-order'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'salsa-commerce-donate-page-key':
          $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
          foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
            if (!empty($line_item_wrapper->salsa_donate_page) && $line_item_wrapper->salsa_donate_page->value()) {
              $replacements[$original] = $line_item_wrapper->salsa_donate_page->value()->donate_page_KEY;
              break;
            }
          }
          break;

      }
    }
  }

  return $replacements;
}
