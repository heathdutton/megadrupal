<?php

/**
 * Implements hook_views_data()
 */
function bookit_availability_views_data() {
  $data = array();

  // Bookable item's calculated availability for specific date range.
  $data['bookit_item']['availability'] = array(
    'title' => t('Availability'),
    'help' => t('The calculated availability of a bookable item for a specific date range.'),
    'field' => array(
      'handler' => 'bookit_item_views_handler_field_availability',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'bookit_item_views_handler_filter_availability',
    ),
    'sort' => array(
      'handler' => 'bookit_item_views_handler_sort_availability',
    ),
  );

  return $data;
}
