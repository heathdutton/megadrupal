<?php
/**
 * @file
 */

/**
 * Implements hook_views_data().
 */
function views_http_headers_views_data() {
  $data['views']['http_headers'] = array(
    'title' => t('HTTP headers'),
    'group' => t('Global'),
    'help' => t('Add HTTP headers to the views output.'),
    'area' => array(
      'help' => t('Add HTTP headers to the views output.'),
      'handler' => 'views_http_headers_area',
    ),
  );

  return $data;
}
