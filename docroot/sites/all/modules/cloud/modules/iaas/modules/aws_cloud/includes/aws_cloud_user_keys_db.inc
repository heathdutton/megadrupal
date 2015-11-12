<?php


/**
 * @file
 * Database functions for SSH Keys
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Update the user keys table
 * @param $cloud_context
 * @param $ssh_keys
 * @return
 */
function _aws_cloud_download_all_user_keys($cloud_context, $ssh_keys) {

  $ec2_lib_user_keys_table    = aws_cloud_get_table_info( AWS_CLOUD_USER_KEYS_TABLE );
  $user_key_arr = cloud_get_all_nickname('key_name', 'owner', $ec2_lib_user_keys_table);

  // delete from user_keys table
  db_delete($ec2_lib_user_keys_table)
    ->condition('cloud_type', $cloud_context)
    ->execute();

  // prepare the insert query
  $insert_query = db_insert($ec2_lib_user_keys_table)
    ->fields(
      array(
        'key_name',
        'cloud_type',
        'owner',
      )
  );

  foreach ($ssh_keys as $key) {
    $tmp_key = cloud_check_in_array($user_key_arr, $key);
    $insert_query
      ->values(
        array(
          'key_name' => $key,
          'cloud_type' => $cloud_context,
          'owner' => $tmp_key == NULL ? $key : $tmp_key,
        )
     );
  }
  
  $insert_query->execute();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => 'Updated: User Key(s)',
      'link' => '',
    )
  );
  return TRUE;
}


/**
 * Store the user Key
 * @global     $user
 * @param     $cloud_context
 * @param     $key
 * @return
 */
function _aws_cloud_add_user_key($cloud_context, $key) {

  $ec2_lib_user_keys_table    = aws_cloud_get_table_info( AWS_CLOUD_USER_KEYS_TABLE );
  global $user;

  $owner = NULL;
  if (isset($user->name)) {
    $owner = $user->name;
  }

  if ($owner == NULL || empty($owner)) {
    $owner = $key;
  }

  $num_deleted = db_delete($ec2_lib_user_keys_table)
      ->condition('key_name', $key)
      ->condition('cloud_type', $cloud_context)
      ->execute();

  $insert_query = db_insert($ec2_lib_user_keys_table)
    ->fields(
      array(
        'key_name' => $key,
        'cloud_type' => $cloud_context,
        'owner' => $owner,
      ))
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('User Key has been added: @key', array('@key' => $key)),
      'link' => '',
    )
  );
  return;
}


/**
 * Remove a user key
 * @global     $user
 * @param     $cloud_context
 * @param     $key
 * @return
 */
function _aws_cloud_remove_user_key($cloud_context, $key) {

  $ec2_lib_user_keys_table    = aws_cloud_get_table_info( AWS_CLOUD_USER_KEYS_TABLE );
  global $user;

  $owner = $user->name;

  $num_deleted = db_delete($ec2_lib_user_keys_table)
    ->condition('key_name', $key)
    ->condition('cloud_type', $cloud_context)
    ->execute();
  return;
}


/**
 * Get user keys
 * @param     $cloud_context
 * @return
 */
function _aws_cloud_get_user_keys_query($cloud_context) {

  $ec2_lib_user_keys_table    = aws_cloud_get_table_info( AWS_CLOUD_USER_KEYS_TABLE );

  return $query = 'SELECT c.key_name,  c.owner from {' . $ec2_lib_user_keys_table . '} c WHERE key_name=:key_name and cloud_type = :cloud_type ';
}


/**
 * Check if a key exists
 * @param     $cloud_context
 * @param     $source
 * @return
 */
function _aws_cloud_check_key($cloud_context, $source) {

  global $user;

  $user_keys_all = array();
  $user_keys_all[] = $user->name;
  $query = _aws_cloud_get_user_keys_query($cloud_context);
  $query_args = array(
    ':key_name' => $user->name,
    ':cloud_type' => $cloud_context,
  );

  $result_keys = db_query( $query, $query_args );

  foreach ($result_keys as $key) {
    $user_keys_all[] = $key->key_name;
  }

  if (in_array($source, $user_keys_all)) {
    return TRUE;
  }

  return FALSE;
}


/**
 * Get all keys
 * @param     $cloud_context
 * @return
 */
function aws_cloud_get_all_keys($cloud_context) {
  global $user;
  // supress this function during cron jobs where user is not available
  if (!$user->uid) {
    return;
  }
  $user_keys_all = array();
  $user_keys_all[] = $user->name;

  $query = _aws_cloud_get_user_keys_query($cloud_context);
  $query_args = array(
    ':key_name' => $user->name,
    ':cloud_type' => $cloud_context,
  );


  $result_keys = db_query( $query, $query_args );

  foreach ($result_keys as $key) {
    $user_keys_all[] = $key->key_name;
  }
  return $user_keys_all;
}
