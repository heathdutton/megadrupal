<?php

/**
 * Preview Sync progress AJAX callback.
 */
function preview_sync_ajax() {
  // Response data.
  $output = array(
    'tasks' => preview_sync_get_latest_batch(),
    'last_run' => variable_get('preview_sync_last_run', 0),
    'elapsed_time' => format_interval(REQUEST_TIME - variable_get('preview_sync_last_run', 0)),
  );

  drupal_json_output($output);
  drupal_exit();
}
