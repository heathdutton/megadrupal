<?php

/**
 * Implements hook_views_data
 **/
function views_regex_rewrite_views_data() {
  $data['views']['regex_rewrite'] = array(
    'title' => t('Regular Expression rewrite'),
    'help' => t('Re-write views data using regular expression mappings'),
    'field' => array(
      'handler' => 'views_handler_field_regex_rewrite',
    ),
  );

  return $data;
}