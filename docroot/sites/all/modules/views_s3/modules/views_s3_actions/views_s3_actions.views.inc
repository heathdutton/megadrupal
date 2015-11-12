<?php

/**
 * @file
 *   View API implementation
 */

/**
 * Implements hook_views_data().
 */
function views_s3_actions_views_data() {
  // Extension
  $data['views_s3']['actions'] = array(
    'title' => t('Action links'),
    'help' => t('List of links of actions to be performed on the current file.'),
    'field' => array(
      'handler' => 'views_s3_actions_handler_field',
    ),
  );

  return $data;
}

