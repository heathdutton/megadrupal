<?php

/**
 * @file
 * SSH Key DB functions
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 */


/**
 * Complete function to delete ssh key pair 
 */
function _aws_cloud_delete_ssh_key($cloud_context, $key_name) {
  $result = FALSE;
  if (($response = aws_cloud_sdk_delete_keypair($cloud_context, $key_name)) !== FALSE) {
    if ($response->return == 'true') {
      $result = _aws_cloud_delete_key_pair($cloud_context, $key_name);
    }
  }
  return $result;
}

/**
 * Function to create ssh key 
 */
function _aws_cloud_create_ssh_key($cloud_context, $key_name) {
  $result = FALSE;
  if (($response = aws_cloud_sdk_create_keypair($cloud_context, $key_name)) !== FALSE) {
    $result = _aws_cloud_save_new_ssh_keypair($cloud_context, $response);
  }
  return $result;
}

/**
 * Delete key pair from database
 * @param     $cloud_context
 * @param     $result
 * @param     $key_name
 * @return
 *
 */
function _aws_cloud_delete_key_pair($cloud_context, $key_name ) {
  $ec2_lib_ssh_keys_table = aws_cloud_get_table_info( AWS_CLOUD_SSH_KEYS_TABLE )  ;
  $num_deleted = db_delete($ec2_lib_ssh_keys_table)
    ->condition('key_name', $key_name)
    ->execute();

  _aws_cloud_remove_user_key($cloud_context, $key_name);
  
    // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('SSH Key has been deleted: @key_name', array('@key_name' => $key_name)),
      'link' => '',
    )
  );
  return TRUE;
}

/**
 * Save a new ssh key pair
 * @param     $cloud_context
 * @param     $myclient
 * @return
 *
 */
function _aws_cloud_save_new_ssh_keypair($cloud_context, $obj_response) {

  $ec2_lib_ssh_keys_table    = aws_cloud_get_table_info( AWS_CLOUD_SSH_KEYS_TABLE )  ;

  $num_insert = db_insert($ec2_lib_ssh_keys_table)
    ->fields(array(
      'key_name' => $obj_response->keyName,
      'cloud_type' => $cloud_context,
      'fingerprint' => $obj_response->keyFingerprint,
      'key_material' => $obj_response->keyMaterial,
    ))
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('SSH Key has been added: @ssh_key', array('@ssh_key' => $obj_response->keyName)),
      'link' => '',
    )
  );
  _aws_cloud_add_user_key($cloud_context, $obj_response->keyName);

  return TRUE;
}


/**
 * Get ssh keys
 * @param     $params
 * @return
 *
 */
function _aws_cloud_get_ssh_key($params) {
  $key = "";

  $key_name = $params['key_name'];
  $cloud_context = $params['cloud_context'];
  $query = aws_cloud_get_ssh_keys_query($cloud_context);
  $query_args   = array(
    ':key_name' => $key_name,
    ':cloud_type' => $cloud_context,
  );

  $result = db_query($query, $query_args);
  foreach ($result as $row) {
    $key = $row->key_material;
  }
  return $key;
}


/**
 * Get ssh key query
 * @param     $cloud_context
 * @return
 *
 */
function aws_cloud_get_ssh_keys_query($cloud_context) {
  $ec2_lib_ssh_keys_table    = aws_cloud_get_table_info( AWS_CLOUD_SSH_KEYS_TABLE )  ;

  return $query =   'SELECT c.key_name,  c.fingerprint ,  c.key_material FROM {' . $ec2_lib_ssh_keys_table . '} c WHERE key_name=:key_name  and cloud_type = :cloud_type order by key_name'  ;
}


/**
 * Get all rows from ssh keys table
 * @param     $cloud_context
 * @return
 *
 */
function _aws_cloud_get_describe_ssh_keys_query($cloud_context) {

  $ec2_lib_ssh_keys_table    = aws_cloud_get_table_info( AWS_CLOUD_SSH_KEYS_TABLE )  ;

  $query = db_select($ec2_lib_ssh_keys_table, 'c');
  $query
    ->fields('c');

  return $query;
}


/**
 * Download all keys from API
 * @param     $cloud_context
 * @return
 */
function _aws_cloud_download_describe_ssh_keys($cloud_context) {

  $ec2_lib_ssh_keys_table    = aws_cloud_get_table_info( AWS_CLOUD_SSH_KEYS_TABLE )  ;
  if (($obj_response = aws_cloud_sdk_describe_keypairs($cloud_context)) !== FALSE) {

    // fetch all the key materials for insert
    $key_material_arr = db_select($ec2_lib_ssh_keys_table, 'c')
      ->fields('c', array('key_name', 'key_material'))
      ->condition('c.cloud_type', $cloud_context)
      ->execute()
      ->fetchAllAssoc('key_name');
    
    // delete all keys for a particular cloud_context 
    db_delete($ec2_lib_ssh_keys_table)
      ->condition('cloud_type', $cloud_context)
      ->execute();
    
    // prep the insert query
    $insert_query = db_insert($ec2_lib_ssh_keys_table)
      ->fields(array(
        'key_name'      ,
        'cloud_type'    ,
        'fingerprint'   ,
        'key_material'  ,
    ));
  
    // use base_cloud because cloud_context is now user entered
    // and cannot be guaranteed
    $base_cloud = cloud_get_base_cloud($cloud_context);
    switch ($base_cloud) { // Needed for OpenStack
      case AWS_CLOUD_OPENSTACK_NOVA_CONTEXT:
        $items = $obj_response->keypairsSet->item
               ? $obj_response->keypairsSet->item
               : $obj_response->keySet->item;
        break;
  
      default:
        $items = $obj_response->keySet->item;
    }
  
    if (!empty($items)) {
      $ssh_keys = array();

      foreach ($items as $item) {
        $tmp_key_material = cloud_check_in_array($key_material_arr, $item->keyName);
        $ssh_keys[] = $item->keyName;
        
        $insert_query->values(
          array(
            'key_name' => '' . $item->keyName,
            'cloud_type' => $cloud_context,
            'fingerprint' => '' . $item->keyFingerprint,
            'key_material' => $tmp_key_material->key_material != NULL ? $tmp_key_material->key_material : '',
          ));
      }
      
      // insert the new keys
      $insert_query->execute();
      // save in user-info
      _aws_cloud_download_all_user_keys($cloud_context, $ssh_keys);
      
      // User Activity Log
      cloud_audit_user_activity( 
        array(
          'type' => 'user_activity',
          'message' => 'Updated: SSH Key(s)',
          'link' => '',
        )
      );
    

    }
  }
  return TRUE;
}


/**
 * Update the ssh keys
 * @param     $cloud_context
 * @param     $key_name
 * @param     $key_material
 * @return
 *
 */
function _aws_cloud_update_key_material($cloud_context, $key_name, $key_material) {

  $ec2_lib_ssh_keys_table    = aws_cloud_get_table_info( AWS_CLOUD_SSH_KEYS_TABLE )  ;

  $key_material = str_replace("\r\n",    "\n", $key_material );
  $key_material = trim($key_material);

  $num_updated = db_update($ec2_lib_ssh_keys_table)
    ->fields(
      array(
        'key_material' => $key_material,
    ))
    ->condition('key_name', $key_name, '=')
    ->condition('cloud_type', $cloud_context, '=')
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('SSH Key has been modified: @key_name', array('@key_name' => $key_name)),
      'link' => '',
    )
  );

  return;
}

/**
 * Get the ssh keys
 * @param     $cloud_context
 * @return
 */
function _aws_cloud_get_ssh_keys($cloud_context) {
  $ec2_lib_ssh_keys_table    = aws_cloud_get_table_info( AWS_CLOUD_SSH_KEYS_TABLE )  ;
  $query =   'SELECT c.key_name,  c.fingerprint ,  c.key_material FROM {' . $ec2_lib_ssh_keys_table . '} c
          WHERE cloud_type =  :cloud_type ';

  $query_args = array(
    ':cloud_type' => $cloud_context,
  );

  $ssh_keys_result = db_query( $query, $query_args );
  $ssh_key_options = array(
    '-1' => AWS_CLOUD_CLOUD_NONE,
  );

  foreach ($ssh_keys_result as $ssh_key) {
    $ssh_key_options[$ssh_key->key_name] = drupal_strtolower($ssh_key->key_name);
  }
  asort( $ssh_key_options);
  return $ssh_key_options;
}


/**
 * Get the ssh key count
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_get_ssh_keys_list_count($cloud_context) {
  $ec2_lib_ssh_keys_table = aws_cloud_get_table_info( AWS_CLOUD_SSH_KEYS_TABLE );
  $query     = 'SELECT COUNT(*) as ssh_count FROM {' . $ec2_lib_ssh_keys_table . '} where cloud_type = :cloud_type '  ;
  $result    = db_query( $query, array(':cloud_type' => $cloud_context) );
  return $result->fetchField();
}


/**
 * Get SSH uers based on instance id
 * @param     $cloud_context
 * @param     $instance_id
 * @return
 */
function aws_cloud_get_ssh_user($cloud_context, $instance_id) {
  $ssh_user_query   = _aws_cloud_get_lock_status_query($cloud_context);
  $query_args = array(
    ':instance_id' => $instance_id,
    ':cloud_type' => $cloud_context,
  );

  $ssh_user_name = CLOUD_SSH_USER_NAME;
  $result = db_query( $ssh_user_query, $query_args );

  foreach ($result as $key) {
    $ssh_user_name = $key->ssh_user_name;
  }

  $ssh_user_name = empty( $ssh_user_name ) ? CLOUD_SSH_USER_NAME : $ssh_user_name  ;
  return $ssh_user_name;
}

/**
 * Get ssh key owner 
 */
function aws_cloud_get_ssh_key_owner($cloud_context, $key) {
  $ec2_lib_ssh_userkeys_table = aws_cloud_get_table_info( AWS_CLOUD_USER_KEYS_TABLE );
  return db_select($ec2_lib_ssh_userkeys_table, 's')
    ->fields('s', array('owner'))
    ->condition('s.key_name', $key)
    ->condition('s.cloud_type', $cloud_context)
    ->execute()
    ->fetchField();
}
