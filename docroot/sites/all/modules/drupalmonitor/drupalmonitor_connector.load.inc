<?php
/**
 * @file
 * This file handles load metrics
 */

 /**
  * Returns performance/load data.
  */
function drupalmonitor_connector_get_loaddata() {

  // Total Requests.
  $time_offset = REQUEST_TIME - 5 * 60;
  $query = "SELECT count(*) FROM {drupalmonitor_log} WHERE dt > :time_offset";
  $info['load_requestscount_300s'] = db_query($query, array('time_offset' => $time_offset))->fetchField();

  // Returns requests per second in the last 300s.
  $info['load_requestspersecondcount_300s'] = $info['load_requestscount_300s'] / (5 * 60);

  // Returns slow requests more than 1000 milliseconds.
  $query = 'SELECT * FROM {drupalmonitor_log} WHERE dt > :time_offset  AND execution_time > :execution_time';
  $info['load_slowrequests_300s'] = db_query($query, array(':time_offset' => $time_offset, ':execution_time' => 1000))->fetchAll();

  // Returns slow requests more than 1000 milliseconds.
  $query = 'SELECT count(*) FROM {drupalmonitor_log} WHERE dt > :time_offset AND execution_time > :execution_time';
  $info['load_slowrequestscount_300s'] = count($info['load_slowrequests_300s']);

  // Returns max memory usage.
  $query = 'SELECT MAX(memory) FROM {drupalmonitor_log} WHERE dt > :time_offset';
  $maxmemory = db_query($query, array(':time_offset' => $time_offset))->fetchField();
  $info['load_maxmemoryusage'] = $maxmemory / 1000000;

  $info['load_apcenabled'] = drupalmonitor_connector_load_apcenabled();

  return $info;
}

/**
 * Returns true or false if apc is enabled.
 */
function drupalmonitor_connector_load_apcenabled() {
  if (!function_exists('apc_cache_info')) {
    return FALSE;
  }
  return TRUE;
}
