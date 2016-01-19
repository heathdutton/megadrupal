<?php

/**
 * @file
 * This file handles status metrics
 */

/**
 * Returns modules table.
 */
function drupalmonitor_connector_status_listmodules() {
  $query = "SELECT * FROM {system}";
  $result = db_query($query);
  $data = array();
  foreach ($result as $record) {
    $infos = unserialize($record->info);
    unset($record->info);
    $record->info = $infos;
    unset($record->info['explanation']);
    unset($record->info['description']);
    $data[$record->name] = get_object_vars($record);
  }
  return $data;
}

/**
 * Returns variable table.
 */
function drupalmonitor_connector_status_listvars() {
  $query = "SELECT * FROM {variable}";
  $result = db_query($query);
  $data = array();
  foreach ($result as $record) {
    $data[$record->name] = $record->value;
  }
  return $data;
}
