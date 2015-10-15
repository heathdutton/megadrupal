<?php

/**
 * Implements hook_views_data()
 */
function bookit_price_views_data() {
  $data = array();

  // Bookable item's calculated price for specific date range.
  $data['bookit_item']['price'] = array(
    'title' => t('Price'),
    'help' => t('The calculated price of a bookable item for a specific date range.'),
    'field' => array(
      'handler' => 'bookit_item_views_handler_field_price',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'bookit_item_views_handler_filter_price',
    ),
    'sort' => array(
      'handler' => 'bookit_item_views_handler_sort_price',
    ),
  );

  return $data;
}
