<?php

/**
 * @file
 * Provide Basic Line Item related Views integration.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_basic_line_item_views_data_alter(&$data) {
  // Add the line item filter for filtering by line items of a basic line item
  // type (based on $line_item_type['product']).
  $data['commerce_line_item']['basic_line_item_type'] = array(
    'title' => t('Line item is of a basic line item type'),
    'help' => t("Filter line items to those of a basic line item type."),
    'filter' => array(
      'handler' => 'commerce_basic_line_item_handler_filter_basic_line_item_type',
    ),
  );
}

/**
 * Implements hook_views_default_views_alter().
 *
 * Add filter to existing line item view.
 */
function commerce_basic_line_item_views_default_views_alter(&$views) {
  if (isset($views['commerce_line_item_table'])) {
    $view = $views['commerce_line_item_table'];
    $handler =& $view->display['default']->handler;

    $handler->display->display_options['filter_groups']['groups'] = array(
      1 => 'OR',
    );
    /* Filter criterion: Commerce Line Item: Line item is of a product line item type */
    $handler->display->display_options['filters']['product_line_item_type']['group'] = 1;
    /* Filter criterion: Commerce Line Item: Line item is of a product line item type */
    $handler->display->display_options['filters']['basic_line_item_type']['id'] = 'basic_line_item_type';
    $handler->display->display_options['filters']['basic_line_item_type']['table'] = 'commerce_line_item';
    $handler->display->display_options['filters']['basic_line_item_type']['field'] = 'basic_line_item_type';
    $handler->display->display_options['filters']['basic_line_item_type']['value'] = '1';
    $handler->display->display_options['filters']['basic_line_item_type']['group'] = 1;
  }
}
