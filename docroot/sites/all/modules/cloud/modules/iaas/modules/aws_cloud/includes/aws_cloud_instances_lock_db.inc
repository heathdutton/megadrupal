<?php

/**
 * @file
 * DB functions for Lock related to instance
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */



/**
 * Get lock status
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_get_lock_status_query($cloud_context) {
  $ec2_lib_instances_lock_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_LOCK_TABLE );
  $sql_instance = 'select instance_id, is_locked, monitor, script_status, ssh_user_name from {' . $ec2_lib_instances_lock_table . "} where instance_id=:instance_id and cloud_type = :cloud_type ";
  return $sql_instance;
}


/**
 * Get Lock status by ID
 * @param   $cloud_context
 * @param   $instance_id
 * @return
 */
function _aws_cloud_get_lock_status_by_id_query($cloud_context, $instance_id) {

  $get_instance_query      = _aws_cloud_get_lock_status_query($cloud_context);
  $get_instance_query_args = array(
    ':instance_id' => $instance_id,
    ':cloud_type' => $cloud_context,
  );
  $instance_found_flag = FALSE;

  $result_instance_query = db_query( $get_instance_query, $get_instance_query_args );


  foreach ($result_instance_query as $instance_data) {

    return $instance_data->is_locked;
  }

  return ''; // Default value
}

/**
 *
 * @param type $cloud_context
 * @param type $instance_id
 * @return type
 */
function _aws_cloud_get_failover_status_by_id_query($cloud_context, $instance_id) {

  $get_instance_query      = _aws_cloud_get_lock_status_query($cloud_context);
  $get_instance_query_args = array(
    ':instance_id' => $instance_id,
    ':cloud_type' => $cloud_context,
  );
  $instance_found_flag = FALSE;

  $result_instance_query = db_query( $get_instance_query, $get_instance_query_args );


  foreach ($result_instance_query as $instance_data) {

    return $instance_data->is_locked;
  }

  return ''; // Default value
}


/**
 * This function returns the lock status of all instances.
 * This will decide whether the instance will allow operations
 *
 */
function _aws_cloud_get_lock_status_all_instances_db($cloud_context) {

  $ec2_lib_instances_lock_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_LOCK_TABLE );
  $sql_instance = 'select instance_id,  is_locked,  monitor ,  script_status from {' . $ec2_lib_instances_lock_table . '} where cloud_type = :cloud_type ';


  $result = db_query( $sql_instance, array(':cloud_type' => $cloud_context) );

  $result_status = array();


  foreach ($result as $key) {

    $result_status[$key->instance_id] = array();
    $result_status[$key->instance_id]['is_locked'    ] = $key->is_locked;
    $result_status[$key->instance_id]['monitor'      ] = $key->monitor;
    $result_status[$key->instance_id]['script_status'] = $key->script_status;
  }

  return $result_status;
}

/**
 *
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_get_lock_status_by_cloud_query($cloud_context) {

  $ec2_lib_instances_lock_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_LOCK_TABLE );
  $sql_instance = 'select instance_id, is_locked, monitor, script_status from {' . $ec2_lib_instances_lock_table . "} where instance_id=:instance_id and cloud_type = :cloud_type";
  return $sql_instance;
}


/**
 *
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_get_instances_lock_status($cloud_context) {

  $ec2_lib_instances_lock_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_LOCK_TABLE );
  $lock_query = 'select instance_id, is_locked, monitor from {' . $ec2_lib_instances_lock_table . '} where cloud_type = :cloud_type ';

  $result = db_query( $lock_query, array(':cloud_type' => $cloud_context) );

  $result_status = array();


  foreach ($result as $key) {

    $result_status[$key->instance_id             ] = $key->is_locked;
    $result_status[$key->instance_id . '_monitor'] = $key->monitor;
  }

  return $result_status;
}


/**
 *
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_set_lock_status_query($cloud_context) {

  $ec2_lib_instances_lock_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_LOCK_TABLE );

  $insert_query = db_insert($ec2_lib_instances_lock_table)
      ->fields(array('instance_id', 'cloud_type', 'is_locked', 'ssh_user_name'));

  return $insert_query;

}


/**
 *
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_set_ssh_user_query($cloud_context, $instance_id, $lock_state, $ssh_user_name) {

  $ec2_lib_instances_lock_table = aws_cloud_get_table_info(AWS_CLOUD_INSTANCES_LOCK_TABLE);
  $insert_query = db_insert($ec2_lib_instances_lock_table)
    ->fields(array('instance_id', 'cloud_type', 'is_locked', 'ssh_user_name'))
    ->values(
      array(
        'instance_id' => $instance_id,
        'cloud_type' => $cloud_context,
        'is_locked' => $lock_state,
        'ssh_user_name' => $ssh_user_name,
      ))
    ->execute();
  //return $insert_query;

}

/**
 *
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_set_monitor_status_query($cloud_context) {

  $ec2_lib_instances_lock_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_LOCK_TABLE );
  $insert_query = db_insert($ec2_lib_instances_lock_table)
      ->fields(array('instance_id', 'cloud_type', 'is_locked', 'monitor'));

  return $insert_query;
}


/**
 *
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_update_lock_status_query($cloud_context) {

  $ec2_lib_instances_lock_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_LOCK_TABLE );

  return db_update($ec2_lib_instances_lock_table);
}



function _aws_cloud_update_ssh_user_query($cloud_context, $instance_id, $ssh_user_name) {

  $ec2_lib_instances_lock_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_LOCK_TABLE );
  $num_updated = db_update($ec2_lib_instances_lock_table) 
    ->fields(array(
    'ssh_user_name' => $ssh_user_name,
  ))
    ->condition('instance_id', $instance_id, '=')
    ->condition('cloud_type', $cloud_context, '=')
    ->execute();

  return $num_updated;
}


function _aws_cloud_ec2_lib_instance_monitoring_status_update_query($cloud_context, $instance_id, $monitor) {

  $ec2_lib_instances_lock_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_LOCK_TABLE );
  $num_updated = db_update($ec2_lib_instances_lock_table) 
    ->fields(array(
    'monitor' => $monitor,
  ))
    ->condition('instance_id', $instance_id, '=')
    ->condition('cloud_type', $cloud_context, '=')
    ->execute();

  return $num_updated;
}


/**
 *
 * @param type $cloud_context
 * @param type $instance_id
 * @param type $script_status
 * @return type
 */
function aws_cloud_instance_failover_status_update_temp($cloud_context, $instance_id, $failover) {

  $ec2_lib_instances_lock_table     = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_LOCK_TABLE );
  $get_instance_query               = _aws_cloud_get_lock_status_query($cloud_context);

  $get_instance_query_args = array(
    ':instance_id' => $instance_id,
    ':cloud_type' => $cloud_context,
  );


  $result_instance_query = db_query( $get_instance_query, $get_instance_query_args )->fetchObject();


  if ($result_instance_query != NULL ) {
    $status_update_query = 'update {' . $ec2_lib_instances_lock_table . "} set failover=:failover where instance_id=:instance_id and cloud_type = :cloud_type ";
    $update_args = array();
    $update_args[':failover']     = $failover;
    $update_args[':instance_id']  = $instance_id;
    $update_args[':cloud_type']   = $cloud_context;

    $result_update = db_query( $status_update_query, $update_args );
  }
  else {

    $insert_instances_lock_query = db_insert($ec2_lib_instances_lock_table);
    $result_update = $insert_instances_lock_query
      ->values(
        array(
      'instance_id' => $instance_id,
      'cloud_type' => $cloud_context,
      'is_locked' => 'Unlock',
      'monitor' => '0',
      'script_status' => $failover,
    ))
      ->execute();

  }

  return $result_update;
}

/**
 *
 * @param   $cloud_context
 * @param   $instance_id
 * @param   $script_status
 * @return
 */
function aws_cloud_instance_status_update($cloud_context, $instance_id, $script_status) {

  $ec2_lib_instances_lock_table     = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_LOCK_TABLE );
  $get_instance_query        = _aws_cloud_get_lock_status_query($cloud_context);
  $get_instance_query_args = array(
    ':instance_id' => $instance_id,
    ':cloud_type' => $cloud_context,
  );

  $instance_found_flag = FALSE;

  $result_instance_query = db_query( $get_instance_query, $get_instance_query_args );


  foreach ($result_instance_query as $instance_data) {
    $instance_found_flag = TRUE;
  }

  if ($instance_found_flag) {
    $status_update_query = 'update {' . $ec2_lib_instances_lock_table . "} set script_status=:script_status where instance_id=:instance_id and cloud_type = :cloud_type ";
    $update_args = array();
    $update_args[':script_status'] = $script_status;
    $update_args[':instance_id'] = $instance_id;
    $update_args[':cloud_type'] = $cloud_context;

    $result_update = db_query( $status_update_query, $update_args );
  }
  else {
    $insert_instances_lock_query = db_insert($ec2_lib_instances_lock_table);
    $result_update = $insert_instances_lock_query
      ->values(
        array(
      'instance_id' => $instance_id,
      'cloud_type' => $cloud_context,
      'is_locked' => 'Unlock',
      'monitor' => '0',
      'script_status' => $script_status,
    ))
      ->execute();

  }

  return $result_update;
}


/**
 *
 * @param   $cloud_context
 * @param   $instance_id
 * @param   $monitor_flag
 * @return
 */
function aws_cloud_instance_monitoring_status_update($cloud_context, $instance_id, $monitor_flag) {

  $ec2_lib_instances_lock_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_LOCK_TABLE );

  $found_record = FALSE  ;

  $query = _aws_cloud_get_lock_status_query($cloud_context);
  $query_args = array(
    ':instance_id' => $instance_id,
    ':cloud_type' => $cloud_context,
  );


  $result = db_query( $query, $query_args );
  foreach ($result as $elastic_ip) {

    $found_record = TRUE;
  }

  if ($found_record == TRUE ) { // Record already exists update the record
    $query_update = _aws_cloud_ec2_lib_instance_monitoring_status_update_query($cloud_context, $instance_id, $monitor_flag);

    // User Activity Log
    cloud_audit_user_activity( array(
      'type' => 'user_activity',
      'message' => t('Instance monitoing status has been modified: @instance_id', array('@instance_id' => $instance_id)),
      'link' => '',
    )
    );
  }
  else { // Insert a new record
    $query_insert = _aws_cloud_set_monitor_status_query($cloud_context);

    $query_insert
      ->values(
        array(
      'instance_id' => $instance_id,
      'cloud_type' => $cloud_context,
      'is_locked' => 'Unlock',
      'monitor' => $monitor_flag,
    )
      )
      ->execute();

    // User Activity Log
    cloud_audit_user_activity( array(
      'type' => 'user_activity',
      'message' => t('Instance has been added: @instance_id', array('@instance_id' => $instance_id)),
      'link' => '',
    )
    );
  }

  return;
}
