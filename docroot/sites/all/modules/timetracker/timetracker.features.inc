<?php
/**
 * @file
 * timetracker.features.inc
 */

/**
 * Implements hook_views_api().
 */
function timetracker_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_default_time_entry_type().
 */
function timetracker_default_time_entry_type() {
  $items = array();
  $items['timetrack'] = entity_import('time_entry_type', '{
    "type" : "timetrack",
    "label" : "Time track",
    "weight" : "0",
    "data" : null,
    "rdf_mapping" : []
  }');
  return $items;
}
