<?php

/**
 * @file
 * Commerce Price Decimals Formatter definitions handlers.
 */

/**
 * Implements hook_views_data().
 */
function commerce_price_decimals_formatter_views_data() {
  $data = array();

  // Define a handler for an area used to summarize a set of line items.
  $data['commerce_order']['order_total_decimals'] = array(
    'title' => t('Order total (Commerce Price Decimals)'),
    'help' => t('Displays the order total field formatted with its components list; requires an Order ID argument.'),
    'area' => array(
      'handler' => 'commerce_price_decimals_formatter_handler_area_order_total_decimals',
    ),
  );
  // Define a handler for an area used to summarize a set of line items.
  $data['commerce_line_item']['line_item_summary_decimals'] = array(
    'title' => t('Line item summary (Commerce Price Decimals)'),
    'help' => t('Summarize the line items in a View with an optional link to checkout.'),
    'area' => array(
      'handler' => 'commerce_price_decimals_formatter_handler_area_line_item_summary_decimals',
    ),
  );

  return $data;
}
