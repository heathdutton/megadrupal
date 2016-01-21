<?php

/**
 * @file
 * DB functions Related to Volumes for Amazon EC2
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Complete function to create a new volume.
 * This function takes care of the API call and 
 * storing the results in the database 
 */
function aws_cloud_create_volume($cloud_context, $name, $size, $zone, $snapshot) {
  $result = FALSE;
  if (($response = aws_cloud_sdk_create_volume($cloud_context, $size, $zone, $snapshot)) !== FALSE) {
    $result = _aws_cloud_save_new_volume($cloud_context, $response, $name);
  }
  return $result;
}

/**
 * Complete function to delete a volume 
 */
function aws_cloud_delete_volume($cloud_context, $vol_id) {
  $result = FALSE;
  if (($response = aws_cloud_sdk_delete_volume($cloud_context, $vol_id)) !== FALSE) {
    if ($response->return == 'true') {
      $result = _aws_cloud_delete_volume($cloud_context, $vol_id);
    }
  }
  return $result;  
}

/**
 * Complete function to detach a volume 
 */
function aws_cloud_detach_volume($cloud_context, $vol_id) {
  $result = FALSE;
  if (($response = aws_cloud_sdk_detach_volume($cloud_context, $vol_id)) !== FALSE) {
    $result = _aws_cloud_save_attach_volume($cloud_context, $response);
  }
  return $result;
}

/**
 * Complete function to attach a volume.  Makes API call and db update
 */
function aws_cloud_attach_volume($cloud_context, $vol_id, $instance_id, $device) {
  $result = FALSE;
  if (($response = aws_cloud_sdk_attach_volume($cloud_context, $vol_id, $instance_id, $device)) !== FALSE) {
    $result = _aws_cloud_save_attach_volume($cloud_context, $response);
  }
  return $result;
}

/**
 * Update the database after the request to attach a volume to an instance was successfully completed
 *
 * @param $cloud_context
 *      The instance of sub-cloud on which attach volume operation was performed.
 * @param $myclient
 *      The response of the REST call
 * @return
 *      FALSE: If the reponse was empty
 *      TRUE:  If the database update was successfull.
 */
function _aws_cloud_save_attach_volume($cloud_context, $obj_response) {
  $ec2_lib_volumes_table    = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE );

  db_update($ec2_lib_volumes_table)
    ->fields(array(
      'instance_id' => $obj_response->instanceId,
      'device' => $obj_response->device,
      'status' => $obj_response->status,
      'attached' => $obj_response->attachTime,
    ))
    ->condition('volume_id', $obj_response->volumeId, '=')
    ->execute();

  // User Activity Log
  cloud_audit_user_activity(
    array(
      'type' => 'user_activity',
      'message' => t('Volume has been modified: @volume_id', array('@volume_id' => $obj_response->volumeId)),
      'link' => '',
    )
  );

  return TRUE;
}

/**
 * Update the Volume Nickname
 */
function _aws_cloud_update_attach_volume($volume_id, $volume_nickname) {
  $ec2_lib_volumes_table = aws_cloud_get_table_info(AWS_CLOUD_EBS_VOLUMES_TABLE);
  $result = db_update($ec2_lib_volumes_table)
    ->fields(array('nickname' => $volume_nickname))
    ->condition('volume_id', $volume_id, '=')
    ->execute();
}

/**
 * Volume Device Options Query 
 */
function _aws_cloud_get_volume_deviceoptions_query($cloud_context) {
  $ec2_lib_instances_table = aws_cloud_get_table_info( AWS_CLOUD_VOLUMES_DEVICE_OPTIONS_TABLE );
  return $query =  'SELECT c.deviceid, c.device FROM {' . $ec2_lib_volumes_device_opt_table . '} c where c.cloud_type = ' . $cloud_context;
}

/**
 * Get Volume by instance Query 
 */
function _aws_cloud_get_volumes_by_instance_query($cloud_context) {
  $ec2_lib_volumes_table   = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE );
  $ec2_lib_instances_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_TABLE   );

  return $query =  'SELECT c.volume_id,  c.nickname,  d.instance_nickname,  c.device,  c.status,  c.created,  c.vol_size,  c.snapshot_id,  c.vol_zone,  c.attached FROM {' . $ec2_lib_volumes_table . '} c
  LEFT JOIN  {' . $ec2_lib_instances_table . '} d on c.instance_id=d.instance_id WHERE c.instance_id=:instance_id and c.cloud_type = :cloud_type order by c.nickname ';
}

/**
 * Describe a volume based on instance_id
 * @param  $cloud_context
 * @param  $instance_id
 * @return
 */
function _aws_cloud_get_describe_volumes_query($cloud_context, $instance_id = '', $status = '') {
  $ec2_lib_volumes_table   = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE ) ;
  $ec2_lib_instances_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_TABLE ) ;
  
  $search_where = " and c.cloud_type = '$cloud_context'";
  if ($instance_id) {
    $search_where .= " and c.vol_zone in (select placement from {" . $ec2_lib_instances_table . "} where instance_id='$instance_id' ) ";
  }

  return $query =  "SELECT c.volume_id,  c.nickname,  d.instance_nickname, c.instance_id, c.device,  c.status,  c.created,  c.vol_size,  c.snapshot_id,  c.vol_zone,  c.attached,  CAST(SUBSTRING_INDEX(c.vol_size,  ' ',  1) as UNSIGNED) as volume_size 
  FROM {" . $ec2_lib_volumes_table . "} c LEFT JOIN  {" . $ec2_lib_instances_table . "} d on c.instance_id=d.instance_id  
  WHERE status like '%%$status%%' " . $search_where;
}


/**
 * Query to return a list of volumes
 * @param  $cloud_context
 * @param  $instance_id
 * @return
 */
function _aws_cloud_get_describe_volumes_list_query($cloud_context) {
  $ec2_lib_volumes_table   = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE );
  $ec2_lib_instances_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_TABLE );

  $query = db_select($ec2_lib_volumes_table, 'c');
  $query
    ->leftjoin($ec2_lib_instances_table, 'd', 'c.instance_id = d.instance_id');

  $query->fields('c');
  $query->fields('d', array('instance_nickname') );

  return $query;
}


/**
 * Get a volume based on instance_id
 * @param  $cloud_context
 * @param  $instance_id
 * @return
 */
function _aws_cloud_get_volume($cloud_context, $instance_id = '') {
  $ec2_lib_volumes_table   = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE );
  $ec2_lib_instances_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_TABLE );

  $search_where = " and c.cloud_type = :cloud_type ";
  if ($instance_id) {
    $search_where = " and c.vol_zone in (select placement from {" . $ec2_lib_instances_table . "} where instance_id='" . $instance_id . "')";
  }

  return $query =  "SELECT c.volume_id,  c.nickname,  d.instance_nickname,  c.device,  c.status,  c.created,  c.vol_size,  c.snapshot_id,  c.vol_zone,  c.attached,  CAST(SUBSTRING_INDEX(c.vol_size,  ' ',  1) as UNSIGNED) as volume_size FROM {" . $ec2_lib_volumes_table . "} c LEFT JOIN  {" . $ec2_lib_instances_table . "} d on c.instance_id=d.instance_id  WHERE nickname = :nickname " . $search_where;
}


/**
 *  Get all volumes
 * @param  $cloud_context
 * @return
 */
function _aws_cloud_get_volumes_query($cloud_context) {

  $ec2_lib_volumes_table   = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE );
  $ec2_lib_instances_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_TABLE   );

  return $query =  'SELECT c.volume_id,  c.nickname,  d.instance_nickname,  c.device,  c.status,  c.created,  c.vol_size,  c.snapshot_id,  c.vol_zone,  c.attached,  c.instance_id
  FROM {' . $ec2_lib_volumes_table . '} c LEFT JOIN  {' . $ec2_lib_instances_table . '} d
  on c.instance_id=d.instance_id WHERE volume_id= :volume_id and c.cloud_type = :cloud_type ';
}

/**
 * Get a count of all volumes
 * @param  $cloud_context
 * @return
 */
function _aws_cloud_get_volumes_list_count($cloud_context) {

  $ec2_lib_volumes_table   = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE );
  $ec2_lib_instances_table = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_TABLE );

  $query = 'SELECT count(*) as item_count FROM {' . $ec2_lib_volumes_table . '} c
  LEFT JOIN  {' . $ec2_lib_instances_table . '} d on c.instance_id=d.instance_id where c.cloud_type=:cloud_type';

  $args = array(
    ':cloud_type' => $cloud_context,
  );
  $result = db_query($query, $args);
  return $result->fetchField();
}

/**
 * Api function to download a all volumes from Amazon
 * @param  $cloud_context
 * @return
 */
function _aws_cloud_download_describe_volumes($cloud_context) {

  $ec2_lib_volumes_table = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE );

  if (($obj_response = aws_cloud_sdk_describe_volumes($cloud_context)) !== FALSE) {
    $id_nick_arr  = cloud_get_all_nickname('volume_id', 'nickname', $ec2_lib_volumes_table);

    db_delete($ec2_lib_volumes_table)
      ->condition('cloud_type', $cloud_context)
      ->execute();

    $insert_query = db_insert($ec2_lib_volumes_table)
      ->fields(
        array(
          'volume_id',
          'cloud_type',
          'nickname',
          'instance_id',
          'device',
          'status',
          'created',
          'vol_size',
          'snapshot_id',
          'vol_zone',
          'attached',
        )
    );
  
    $count = 0;
  
    foreach ($obj_response->volumeSet->item as $item) {
      $attachment   = $item->attachmentSet->item;
      $tmp_nickname = cloud_check_in_array($id_nick_arr, $item->volumeId);
      $insert_query->values(
        array(
          'volume_id' => $item->volumeId,
          'cloud_type' => $cloud_context,
          'nickname' => $tmp_nickname != NULL ? $tmp_nickname : $item->volumeId,
          'instance_id' => $attachment->instanceId,
          'device' => $attachment->device,
          'status' => isset( $attachment->status) ? $item->status . '(' . $attachment->status . ')' : $item->status,
          'created' => $item->createTime,
          'vol_size' => $item->size . ' GB',
          'snapshot_id' => ( isset($item->snapshotId) &&  empty($item->snapshotId) == FALSE ) ? $item->snapshotId : '',
          'vol_zone' => $item->availabilityZone,
          'attached' => $attachment->attachTime,
        )
      );
      $count++;
    }

    if ($count > 0) {
      $insert_query->execute();
      // User Activity Log
      cloud_audit_user_activity( 
        array(
          'type' => 'user_activity',
          'message' => 'Updated: Volume(s)',
          'link' => '',
        )
      );
    }
  }
  return TRUE;
}

/**
 * Insert a new volume into the database
 * @param  $cloud_context
 * @param  $myclient
 * @param  $nickname
 * @return
 */
function _aws_cloud_save_new_volume($cloud_context, $obj_response, $nickname ) {
  $ec2_lib_volumes_table    = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE );

  db_insert($ec2_lib_volumes_table)
    ->fields(
      array(
        'volume_id',
        'cloud_type',
        'nickname',
        'status',
        'created',
        'vol_size',
        'vol_zone',
        'snapshot_id',
      )
    )
    ->values(
      array(
        'volume_id' => $obj_response->volumeId,
        'cloud_type' => $cloud_context,
        'nickname' => $nickname,
        'status' => $obj_response->status,
        'created' => $obj_response->startTime,
        'vol_size' => $obj_response->volumeSize,
        'vol_zone' => $obj_response->availabilityZone,
        'snapshot_id' => $obj_response->snapshotId,
      )  
    )
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('Volume has been added: @volume_id', array('@volume_id' => $obj_response->volumeId)),
      'link' => '',
    )
  );

  return TRUE;
}

/**
 * REST request to delete a volume is submitted and this function will
 * store the response of this request
 *
 * @param  $cloud_context
 *           The sub-cloud who's volume is to be deleted
 * @param  $result
 *           The REST call response
 * @param  $vol_id
 *           The Id of volume to be deleted
 * @return
 */
function _aws_cloud_delete_volume($cloud_context, $vol_id ) {
  $ec2_lib_volumes_table = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE );
  db_delete($ec2_lib_volumes_table)
    ->condition('volume_id', $vol_id)
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('Volume has been deleted: @volume_id', array('@volume_id' => $vol_id)),
      'link' => '',
    )
  );
  return TRUE;
}

/**
 * Function to check if a volume is attached
 */
function _aws_cloud_check_volume_attached_status($volume_id) {
  $ec2_lib_volumes_table = aws_cloud_get_table_info(AWS_CLOUD_EBS_VOLUMES_TABLE);
  $result = db_select($ec2_lib_volumes_table, 'c')
    ->fields('c', array('status') )
    ->condition('volume_id', $volume_id, '=')
    ->execute()
    ->fetchField();
  return (($result !== FALSE && trim($result) == 'available') ? FALSE : TRUE);
}

