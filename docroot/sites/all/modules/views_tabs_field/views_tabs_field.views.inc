<?php

/**
 * @file
 * Views Tabs Field views code.
 */

/**
 * Implements hook_views_data_alter().
 */
function views_tabs_field_views_data_alter(&$data) {

  $data['views']['tabs'] = array(
    'title' => t('Tabs'),
    'help' => t('Display tabbed content in the view.'),
    'field' => array(
      'handler' => 'views_tabs_field_views_handler_field',
    ),
  );

}
