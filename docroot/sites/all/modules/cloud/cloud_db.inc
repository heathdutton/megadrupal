<?php

/**
 * @file
 * Provides common database-related functionalites for cloud management.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */


/**
 * update cloud table with latest timestamp
 * @return unknown_type
 */
function cloud_update_host_entries_last_update_time($cloud_context) {

  $num_updated = db_update(CLOUD_CLOUDS_TABLE)
    ->fields(array(
    'last_update_time' => REQUEST_TIME,
  ))
    ->condition('cloud_name', $cloud_context, '=')
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( array(
    'type' => 'user_activity',
    'message' => t('Cloud has been modified: @cloud_context', array('@cloud_context' => $cloud_context)),
    'link' => '', //'design/alerts/create&id=' . $alert_id
  )
  );

  return;
}

/**
 * Add a cloud row into the cloud_clouds table
 * @param unknown_type $query_args
 */
function cloud_add_cloud_row($query_args) {

  $num_inserted = db_insert(CLOUD_CLOUDS_TABLE)
      ->fields(
        array(
    'cloud_name' => $query_args[0],
    'cloud_display_name' => $query_args[1],
    'last_update_time' => REQUEST_TIME -10,
    'host_entries_refresh_time' => $query_args[2],
    'module' => $query_args[3],
    'base_cloud' => $query_args[4],
  ))
      ->execute();

  return $num_inserted;
}

/**
 * Get the module responsible for this cloud_context.
 */
function cloud_get_module($cloud_context) {
  static $modules;
  $return = NULL;
  if (isset($modules[$cloud_context])) {
    $return = $modules[$cloud_context];
  }
  else {

    $module = db_query('SELECT module from {' . CLOUD_CLOUDS_TABLE . '} where cloud_name = :cloud_name ', array(':cloud_name' => $cloud_context))->fetchField();
    if ($module !== FALSE) {
      $return = $module;
      $modules[$cloud_context] = $return;
    }
  }
  return $return;
}

/**
 * Get the base cloud type for this cloud_context
 */
function cloud_get_base_cloud($cloud_context) {

  return db_query("SELECT base_cloud from {" . CLOUD_CLOUDS_TABLE . "} where cloud_name = :cloud_name ", array(':cloud_name' => $cloud_context))->fetchField();
}

/**
 * Check for an existing cloud_name in the database
 * @param unknown_type $cloud_name
 */
function cloud_check_cloud_name($cloud_name) {
  return cloud_get_db_results("SELECT cloud_name from {" . CLOUD_CLOUDS_TABLE . "} where cloud_name = :cloud_name ", array(':cloud_name' => $cloud_name));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cloud_get_all_scripts($type) {


  $query = _cloud_scripting_get_scripts();
  $query->condition('type', $type, '=');
  $result = $query->execute();

  $script_options = array();
  foreach ($result as $script) {

    $script_options[$script->script_id] = $script->name;
  }

  return $script_options;
}


