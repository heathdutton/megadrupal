<?php

/**
 * @file
 * Admin functionality for the Update Log module.
 */

/**
 * Page callback for module update log.
 */
function update_log_page_callback() {
  $module_info = system_get_info('module');

  $module_updates = update_log_get_completed_update_list();

  $header = array(
    'Name',
    'Schema Version',
    'Description',
  );

  $rows = array();

  foreach ($module_updates as $module => $updates) {
    foreach ($updates as $schema_version => $description) {
      $rows[] = array(
        'data' => array(
          $module_info[$module]['name'],
          $schema_version,
          $description,
        ),
      );
    }
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
