<?php

/**
 * @file
 * DB functions Related to Snapshot for Amazon EC2
 *
 * Copyright (c) 2010-2012 DOCOMO Innovations, Inc.
 *
 */

/**
 * Complete function for deleting a snapshot 
 */
function _aws_cloud_delete_snapshot($cloud_context, $ids) {
  $result = FALSE;
  if (($response = aws_cloud_sdk_delete_snapshot($cloud_context, $ids)) !== FALSE) {
    if ($response->return == 'true') {
      $result = _aws_cloud_delete_snapshot_db($cloud_context, $ids);
    }
  }
  return $result;
}

/**
 * Get Snapshot query
 * @param     $cloud_context
 * @return
 */
function _aws_cloud_get_describe_snapshots_query($cloud_context) {

  $ec2_lib_snapshot_table = aws_cloud_get_table_info( AWS_CLOUD_SNAPSHOTS_TABLE );
  $ec2_lib_ebs_volumes_table = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE );

  $query = db_select($ec2_lib_snapshot_table, 'c');
  $query
    ->leftjoin($ec2_lib_ebs_volumes_table, 'vol', 'c.volume_id = vol.volume_id');

  $query->fields('c');
  $query->addField('vol', 'nickname', 'vol_nickname');

  return $query;
}




/**
 * Get Snapshot List count
 * @param     $cloud_context
 * @return
 */
function _aws_cloud_get_snapshots_list_count($cloud_context) {
  $ec2_lib_snapshot_table = aws_cloud_get_table_info( AWS_CLOUD_SNAPSHOTS_TABLE );
  $query      = 'SELECT COUNT(*) as item_count FROM {' . $ec2_lib_snapshot_table . '} where cloud_type = :cloud_type ';
  $result     = db_query( $query, array(':cloud_type' => $cloud_context) );
  return $result->fetchField();
}

/**
 * Count query for snapshots 
 */
function _aws_cloud_get_count_name_snapshots_query($cloud_context, $nickname) {
  $ec2_lib_snapshot_table = aws_cloud_get_table_info( AWS_CLOUD_SNAPSHOTS_TABLE );
  $query = 'SELECT count(*) as count FROM {' . $ec2_lib_snapshot_table . '} c WHERE nickname =:nickname ';
  $result = db_query($query, array(':nickname' => $nickname));
  return $result->fetchField();
}


/**
 * Get snapshot by id
 * @param     $cloud_context
 * @param     $ss_id
 * @return
 */
function _aws_cloud_get_snapshots_by_id_db($cloud_context, $ss_id) {

  $ec2_lib_snapshot_table    = aws_cloud_get_table_info( AWS_CLOUD_SNAPSHOTS_TABLE );
  $ec2_lib_ebs_volumes_table = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE );

  $query =  'select snap.nickname    as snap_nickname  ,
                    snap.snapshot_id as snapshot_id    ,
                    snap.status      as snap_status    ,
                    snap.volume_id   as snap_volume_id ,
                    snap.created     as snap_created   ,
                    vol.nickname as vol_nickname ,
                    snap.volume_size as vol_size ,
                    vol.vol_zone as vol_zone
                    from
                    ( SELECT c.snapshot_id,  c.nickname,   c.volume_id,  c.status,  c.created,  c.progress, c.volume_size FROM {' . $ec2_lib_snapshot_table . '} c
                    WHERE c.snapshot_id= :snapshot_id  ) snap
                    LEFT JOIN
                     {' . $ec2_lib_ebs_volumes_table . '} vol
                    on snap.volume_id = vol.volume_id
                    and cloud_type = :cloud_type
                    ';
  $query_args = array(
    ':snapshot_id' => $ss_id,
    ':cloud_type' => $cloud_context,
  );

  $result_arr = array();

  $db_result  = db_query( $query, $query_args );

  foreach ($db_result as $row) {

    foreach ($row as $key => $val) {

      $result_arr[$key] = $val;
    }
  }

  return $result_arr;
}


/**
 * Download snapshots from EC2 and stores them in the database
 * @param     $cloud_context
 * @return
 */
function _aws_cloud_download_describe_snapshots($cloud_context) {

  $ec2_lib_snapshot_table = aws_cloud_get_table_info( AWS_CLOUD_SNAPSHOTS_TABLE );
  
  if (($obj_response = aws_cloud_sdk_describe_snapshots($cloud_context)) !== FALSE) {
    $id_nick_arr = cloud_get_all_nickname('snapshot_id', 'nickname', $ec2_lib_snapshot_table);
    
    db_delete($ec2_lib_snapshot_table)
      ->condition('cloud_type', $cloud_context)
      ->execute();
  
    $insert_query = db_insert($ec2_lib_snapshot_table)
      ->fields(
        array(
          'snapshot_id',
          'cloud_type',
          'nickname',
          'volume_id',
          'status',
          'created',
          'progress',
          'owner_id',
          'volume_size',
          'description',
        )
    );
  
    $count = 0;
    $owner_id = cloud_get_info($cloud_context , 'user_id');
  
    foreach ($obj_response->snapshotSet->item as $item) {
      $tmp_nickname = cloud_check_in_array($id_nick_arr, $item->snapshotId);

      $insert_query
        ->values(
          array(
            'snapshot_id' => $item->snapshotId,
            'cloud_type'  => $cloud_context,
            'nickname'    => $tmp_nickname != NULL ? $tmp_nickname : $item->snapshotId,
            'volume_id'   => $item->volumeId,
            'status'      => $item->status,
            'created'     => $item->startTime,
            'progress'    => $item->progress,
            'owner_id'    => isset($item->ownerId) ? $item->ownerId : '',
            'volume_size' => isset($item->volumeSize) ? $item->volumeSize : '',
            'description' => isset($item->description) ? $item->description : '',
          )
       );
      $count++;
    }
  
    if ($count) {
      $insert_query->execute();
      
      // User Activity Log
      cloud_audit_user_activity( 
        array(
          'type' => 'user_activity',
          'message' => 'Updated: Snapshot(s)',
          'link' => '',
        )
      );
    }
  }
  return TRUE;
}


/**
 * Complete function for creating a snapshot 
 */
function aws_cloud_save_snapshot($cloud_context, $volume_id, $snapshot_name, $snapshot_description) {
  if (($response = aws_cloud_sdk_create_snapshot($cloud_context, $volume_id, $snapshot_description)) !== FALSE) {
    $result = _aws_cloud_save_create_snapshot($cloud_context, $response, $snapshot_name, $snapshot_description);
    if ($result) {
      return $response->snapshotId; 
    }
  }
  return FALSE;
}

/**
 * DB function for saving snapshot
 * @param     $cloud_context
 * @param     $myclient
 * @param     $ss_name
 * @param     $ss_desc
 * @return
 */
function _aws_cloud_save_create_snapshot($cloud_context, $obj_response, $ss_name, $ss_desc) {

  $ec2_lib_snapshot_table = aws_cloud_get_table_info(AWS_CLOUD_SNAPSHOTS_TABLE);
  
  db_insert($ec2_lib_snapshot_table)
    ->fields(
      array(
        'snapshot_id',
        'cloud_type',
        'nickname',
        'volume_id',
        'status',
        'created',
        'progress',
        'owner_id',
        'volume_size',
        'description',
      )
    )
    ->values(
      array(
        'snapshot_id' => $obj_response->snapshotId,
        'cloud_type'  => $cloud_context,
        'nickname'    => $ss_name,
        'volume_id'   => $obj_response->volumeId,
        'status'      => $obj_response->status,
        'created'     => $obj_response->startTime,
        'progress'    => $obj_response->progress,
        'owner_id'    => isset($obj_response->ownerId) ? $obj_response->ownerId : '',
        'volume_size' => isset($obj_response->volumeSize) ? $obj_response->volumeSize : '',
        'description' => isset($ss_desc) ? $ss_desc : '',
      )
    )
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('Snapshot has been added: @snapshot_name', array('@snapshot_name' => $ss_name)),
      'link' => '',
    )
  );

  return TRUE;
}


/**
 * DB function to delete snapshot
 * @param     $cloud_context
 * @param     $result
 * @param     $ss_id
 * @return
 */
function _aws_cloud_delete_snapshot_db($cloud_context, $ss_id) {
  $ec2_lib_snapshot_table = aws_cloud_get_table_info( AWS_CLOUD_SNAPSHOTS_TABLE );
  db_delete($ec2_lib_snapshot_table)
    ->condition('snapshot_id', $ss_id)
    ->condition('cloud_type', $cloud_context)
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('Snapshot has been deleted: @snapshot_id', array('@snapshot_id' => $ss_id)),
      'link' => '',
    )
  );
  return TRUE;
}


/**
 * Get a snapshot
 * @param     $cloud_context
 * @return
 */
function _aws_cloud_get_snapshot_options($cloud_context) {

  $snapshot_options = array(
    AWS_CLOUD_CLOUD_NONE => AWS_CLOUD_CLOUD_NONE,
  );

  $ec2_lib_snapshot_table = aws_cloud_get_table_info( AWS_CLOUD_SNAPSHOTS_TABLE );
  $ec2_lib_ebs_volumes_table = aws_cloud_get_table_info( AWS_CLOUD_EBS_VOLUMES_TABLE );

  $query = 'SELECT c.snapshot_id, vol.nickname as vol_nickname , c.nickname, c.volume_id, c.status, c.created, c.progress FROM {' . $ec2_lib_snapshot_table . '} c LEFT JOIN
                     {' . $ec2_lib_ebs_volumes_table . '} vol
                    on c.volume_id = vol.volume_id WHERE c.snapshot_id like :snapshot_id and c.cloud_type = :cloud_type ';

  $query_args = array(
    ':snapshot_id' => '%',
    ':cloud_type' => $cloud_context,
  );

  $snapshot_result = db_query($query, $query_args);

  foreach ($snapshot_result as $snap) {

    $snapshot_options[$snap->snapshot_id] = $snap->nickname . ' (' . $snap->snapshot_id . ')';
  }

  return $snapshot_options;
}


/**
 * Update a snapshot
 * @param     $cloud_context
 * @param     $snapshot_id
 * @param     $sql_col
 * @param     $value
 * @return
 */
function _aws_cloud_snapshot_update($cloud_context, $snapshot_id, $sql_col, $value) {

  $ec2_lib_snapshot_table    = aws_cloud_get_table_info( AWS_CLOUD_SNAPSHOTS_TABLE );

  $num_updated = db_update($ec2_lib_snapshot_table)
    ->fields(array(
      $sql_col => $value,
    ))
    ->condition('snapshot_id', $snapshot_id, '=')
    ->condition('cloud_type', $cloud_context, '=')
    ->execute();
  return TRUE;
}

/**
 * Checks snapshot completion 
 */
function _aws_cloud_check_snapshot_completion($snapshot_id) {
  $ec2_lib_snapshots_table = aws_cloud_get_table_info(AWS_CLOUD_SNAPSHOTS_TABLE);
  $result = db_select($ec2_lib_snapshots_table, 'c')
  ->fields('c', array('progress') )
  ->condition('snapshot_id', $snapshot_id, '=')
  ->execute()
  ->fetchField();
  return (($result !== FALSE && trim($result) == '100%') ? TRUE : FALSE);
}
