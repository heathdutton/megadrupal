<?php

/**
 * Implements hook_views_data()
 */
function bookit_calendar_views_data() {
  $data = array();

  // Bookable item's calendar date filter.
  $data['bookit_item']['date'] = array(
    'title' => t('Calendar Date'),
    'help' => t('Filters the bookable items by calendar date.'),
    'filter' => array(
      'handler' => 'bookit_item_views_handler_filter_date',
    ),
  );

  return $data;
}
