<?php

/**
 * @file
 * Manipulates pattern files to add information about the history of
 * merging and pulling.
 */ 

/**
 * Adds a parent tag in the info section of the pattern.
 * 
 * @param array $pattern 
 *   Pattern in which the new header will be appended
 * @param string $host
 *   URL of the instance that performed the action
 * @param string
 *   $d2d_id  D2D ID of the instance that performed the action
 * @param string $action
 *   Action that has produced this pattern, for now 'merge', 'pull', or 'push'
 * @param array $parents
 *   Array of UUUIDs of the parents
 * 
 * @return Pattern including the new header
 */
function patterns_history_set_parent(&$pattern, $host, $d2d_id, $action, $parents) {
  $pattern['info']['parent'] = array();
  $pattern['info']['parent']['host'] = $host;
  $pattern['info']['parent']['d2d_id'] = $d2d_id;
  $pattern['info']['parent']['action'] = $action;
  $pattern['info']['parent']['date'] = date_timestamp_get(date_create());
  foreach ($parents as $uuuid) {
    $pattern['info']['parent']['uuuids'][] = $uuuid;
  }
}