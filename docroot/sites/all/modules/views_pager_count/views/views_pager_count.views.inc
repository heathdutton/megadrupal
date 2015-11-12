<?php

/**
 * Implements hook_views_data()
 */
function views_pager_count_views_data() {
  $data = array();

  // Define an area handler that displays a form allowing the user
  // to specify the number of items to be displayed on one view page.
  $data['views']['view_pager_count_form'] = array(
    'title' => t('View pager count form'),
    'help' => t('Display a form allowing the user to specify the number of items to be displayed on one view page.'),
    'area' => array(
      'handler' => 'views_pager_count_handler_area_view_count_form'
    ),
  );

  return $data;
}
