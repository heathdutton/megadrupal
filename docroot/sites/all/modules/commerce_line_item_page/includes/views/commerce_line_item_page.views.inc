<?php

/**
 * Provide line item page related Views integration.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_line_item_page_views_data_alter(&$data) {
  $data['commerce_line_item']['line_item_label']['field']['handler'] = 'commerce_line_item_page_handler_field_line_item_label';
}

/**
 * Implements hook_views_plugins().
 */
function commerce_line_item_page_views_plugins() {
  return array();
}
