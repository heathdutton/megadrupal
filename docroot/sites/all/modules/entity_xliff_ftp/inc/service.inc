<?php

/**
 * @file
 * Hooks and functions related to web services exposed by this module.
 */


/**
 * Page callback for a web service endpoint that can be used to trigger syncs
 * from the configured remote FTP server.
 */
function entity_xliff_ftp_sync_from_remote() {
  // Always return JSON.
  $response = array();
  drupal_add_http_header('Content-Type', 'application/json');

  // If not POST, throw a 405 Method Not Allowed.
  if (strtolower($_SERVER['REQUEST_METHOD']) !== 'post') {
    drupal_add_http_header('Status', '405 Method Not Allowed');
    print drupal_json_encode((object) $response);
    drupal_exit();
  }

  // If cron key is not supplied, throw a 403 Forbidden.
  $expectedKey = variable_Get('cron_key', '');
  $params = drupal_get_query_parameters();
  if (!isset($params['key']) || $params['key'] !== $expectedKey) {
    drupal_add_http_header('Status', '403 Forbidden; missing or invalid key.');
    print drupal_json_encode((object) $response);
    drupal_exit();
  }

  // Otherwise, proceed as normal.
  try {
    // Allow this process to run indefinitely.
    set_time_limit(0);

    // Process all XLIFFs that can be.
    $response['processed'] = entity_xliff_ftp_process_all();

    // Return 200 on success. 500 on failure.
    drupal_add_http_header('Status', '200 OK');
    print drupal_json_encode((object) $response);
    drupal_exit();
  }
  catch (Exception $e) {
    // Return a 500 on failure.
    drupal_add_http_header('Status', '500 Internal Server Error; ' . $e->getMessage());
    print drupal_json_encode((object) $response);
    drupal_exit();
  }
}
