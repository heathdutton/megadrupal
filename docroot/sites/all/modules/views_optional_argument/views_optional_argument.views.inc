<?php

/**
 * Implements hook_views_data_alter()
 */
function views_optional_argument_views_data_alter(&$data) {
  // Replace argument handlers for node and user tables.
  if (isset($data['node']['nid']['argument']['handler'])) {
    $data['node']['nid']['argument']['handler'] = 'views_optional_argument_node_nid';
  }
  if (isset($data['node_revision']['nid']['argument']['handler'])) {
    $data['node_revision']['nid']['argument']['handler'] = 'views_optional_argument_node_nid';
  }
  if (isset($data['users']['uid']['argument']['handler'])) {
    $data['users']['uid']['argument']['handler'] = 'views_optional_argument_user_uid';
  }

  // Replace views_handler_argument_numeric argument handler.
  foreach ($data as $table_name => $table_info) {
    foreach ($table_info as $field_name => $field_info) {
      if ((isset($field_info['argument']['handler'])) && ($field_info['argument']['handler'] == 'views_handler_argument_numeric')) {
        $data[$table_name][$field_name]['argument']['handler'] = 'views_optional_argument_numeric';
      }
    }
  }
}
