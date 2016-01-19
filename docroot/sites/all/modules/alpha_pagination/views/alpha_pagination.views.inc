<?php

/**
 * @file
 * Set up our views hook data.
 */

/**
 * Implements hook_views_data().
 */
function alpha_pagination_views_data() {
  $data = array();
  $data['views']['alpha_pagination'] = array(
    'title' => t('Alpha Pagination'),
    'group' => 'Global',
    'help' => t('Area handler to add our alphabetic pagination.'),
    'area' => array(
      'handler' => 'alpha_pagination_handler_pagination',
    ),
  );
  return $data;
}
