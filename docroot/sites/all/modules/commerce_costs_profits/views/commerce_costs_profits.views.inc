<?php

/**
 * @file
 * Provide costs related Views integration.
 */

/**
 * Implements hook_views_data().
 */
function commerce_costs_profits_views_data() {
  $data = array();

  $data['commerce_line_item']['line_items_costs_summary'] = array(
    'title' => t('Line items Costs summary'),
    'help' => t('Summarize the line items costs'),
    'area' => array(
      'handler' => 'commerce_costs_profits_handler_area_line_item_costs_summary',
    ),
  );
  $data['commerce_line_item']['profit'] = array(
    'field' => array(
      'title' => t('Line item Profit'),
      'help' => t('Calculated line item profit'),
      'handler' => 'commerce_costs_profits_handler_line_item_profit',
    ),
  );

  return $data;
}
