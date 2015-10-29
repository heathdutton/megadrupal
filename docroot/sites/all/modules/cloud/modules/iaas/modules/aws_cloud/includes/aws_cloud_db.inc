<?php

/**
 * @file
 * AWS Cloud database functions
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Gets all the clouds in the database 
 */
function aws_cloud_get_all_clouds($sort_string = NULL) {

  //Only return sub-clouds that are defined by aws_cloud or other clouds
  //using the same mechanism
  return aws_cloud_get_all_clouds_query()->execute();
}

/**
 * Gets all the clouds in the database 
 */
function aws_cloud_get_all_clouds_query() {

  //Only return sub-clouds that are defined by aws_cloud or other clouds
  //using the same mechanism
  $base_clouds = aws_cloud_get_cloud_data();

  $keys = array();
  foreach (array_keys($base_clouds) as $key) {
    $keys[] = $key;
  }

  $query = db_select(CLOUD_CLOUDS_TABLE, 'c')
         ->fields('c',  array('cloud_name',
                              'cloud_id',
                              'start_duration',
                              'cloud_display_name',
                              'start_duration',
                              'scaling_server_url',
                              'last_update_time',
                              'host_entries_refresh_time',
                              'module',
                              'base_cloud'))
         ->fields('cd', array('api_version',
                              'host_uri',
                              'aws_access_key',
                              'aws_secret_key',
                              'user_id',
                              'image_upload_url',
                              'image_register_url',
                              'certificate'));
  $query->leftJoin(AWS_CLOUD_DETAILS_TABLE, 'cd', 'c.cloud_name = cd.cloud_name');

  if (count($keys) > 0) {
    $str_to_search = implode(',', $keys);
    $query->condition('c.base_cloud', $keys, 'IN');
  }

  return $query;
}

/**
 * Get an entire cloud row from the database
 */
function aws_cloud_get_cloud($cloud_name) {
  $cloud = array();
  $query = "SELECT c.base_cloud, c.cloud_name, c.cloud_id, c.start_duration, c.cloud_display_name, c.start_duration, c.scaling_server_url, c.last_update_time, c.host_entries_refresh_time, c.module, c.base_cloud,
        cd.api_version, cd.host_uri, cd.aws_access_key, cd.aws_secret_key, cd.user_id, cd.image_upload_url, cd.image_register_url, cd.certificate,
        c.monitoring_server_url, c.monitoring_server_user_id, c.monitoring_server_password from {" . CLOUD_CLOUDS_TABLE . "} c
        inner join {" . AWS_CLOUD_DETAILS_TABLE . "} cd on cd.cloud_name = c.cloud_name and c.cloud_name = :cloud_name";
  $results =  cloud_get_db_results($query, array(':cloud_name' => $cloud_name));

  //there should only be one
  if (count($results) == 1) {
    $cloud = array_shift($results);
  }
  return $cloud;
}

/**
 * Delete a cloud details data from the aws cloud table
 */
function aws_cloud_delete_cloud($cloud_name) {
  $num_deleted = db_delete(AWS_CLOUD_DETAILS_TABLE)
    ->condition('cloud_name', $cloud_name)
    ->execute();
}

/**
 * Gets Host URI from the database 
 */
function aws_cloud_check_host_uri($host_uri, $user_id) {
  return db_query("SELECT host_uri from {" . AWS_CLOUD_DETAILS_TABLE . "} where host_uri = :host_uri and user_id = :user_id ", array(':host_uri' => $host_uri, ':user_id' => $user_id))->fetchObject();
}

/**
 * Gets details about a particular cloud 
 */
function aws_cloud_get_info($cloud_name, $key) {
  //monitor_url needs refactoring, ignore for now
  if ($key == 'monitor_url') {
    return FALSE;
  }
  return db_query("SELECT $key from {" . AWS_CLOUD_DETAILS_TABLE . "} where cloud_name = :cloud_name ", array(':cloud_name' => $cloud_name))->fetchField();
}

/**
 * Deletes all aws cloud tables in the database 
 */
function aws_cloud_delete_all($cloud_context) {
  db_delete(aws_cloud_get_table_info(AWS_CLOUD_INSTANCES_TABLE))->condition('cloud_type', $cloud_context)->execute();
  db_delete(aws_cloud_get_table_info(AWS_CLOUD_INSTANCES_LOCK_TABLE))->condition('cloud_type', $cloud_context)->execute();
  db_delete(aws_cloud_get_table_info(AWS_CLOUD_IMAGES_TABLE))->condition('cloud_type', $cloud_context)->execute();
  db_delete(aws_cloud_get_table_info(AWS_CLOUD_SSH_KEYS_TABLE))->condition('cloud_type', $cloud_context)->execute();
  db_delete(aws_cloud_get_table_info(AWS_CLOUD_SECURITY_GROUPS_TABLE))->condition('cloud_type', $cloud_context)->execute();
  db_delete(aws_cloud_get_table_info(AWS_CLOUD_SECURITY_GROUPS_DETAILS_TABLE))->condition('cloud_type', $cloud_context)->execute();
  db_delete(aws_cloud_get_table_info(AWS_CLOUD_ZONE_TABLE))->condition('cloud_type', $cloud_context)->execute();
  db_delete(aws_cloud_get_table_info(AWS_CLOUD_ELASTIC_IP_TABLE))->condition('cloud_type', $cloud_context)->execute();
  db_delete(aws_cloud_get_table_info(AWS_CLOUD_EBS_VOLUMES_TABLE))->condition('cloud_type', $cloud_context)->execute();
  db_delete(aws_cloud_get_table_info(AWS_CLOUD_USER_KEYS_TABLE))->condition('cloud_type', $cloud_context)->execute();
  db_delete(aws_cloud_get_table_info(AWS_CLOUD_REGISTER_IMAGE_STATUS_TABLE))->condition('cloud_type', $cloud_context)->execute();
  db_delete(aws_cloud_get_table_info(AWS_CLOUD_SNAPSHOTS_TABLE))->condition('cloud_type', $cloud_context)->execute();
}

/**
 * Retrieve and store information from the server about the sub-cloud.
 *
 * This function will download the avaliable information for the following.
 *     viz. Elastic IP Info,
 *          Instances Info,
 *          SSH Keys Info,
 *          Security Group Info,
 *          Zone Info,
 *          Snapshot Info,
 *          Volume Info
 *
 * In case the expected paramters are not set there will be a errror message displayed asking user to first set the variables.
 *
 * @param $cloud_context
 *         This is the sub-cloud who's information is to be downloaded
 * @return
 *         The information is downloded using REST calls and is stored in the database in the respective cloud_context tables.
 */
function _aws_cloud_download_all($cloud_context) {
  $host_uri = cloud_get_info($cloud_context, 'host_uri');
  $access_key = cloud_get_info($cloud_context, 'aws_access_key');
  $sec_key = cloud_get_info($cloud_context, 'aws_secret_key');

  // Checking if all ec2 variables are set
  if (empty( $host_uri) || empty( $access_key ) || empty($sec_key)) {
    $cloud_list = cloud_get_all_clouds_display_name();
    cloud_set_config_error_message($cloud_list[$cloud_context], $cloud_context);
    return FALSE;
  }

  if (strpos($host_uri, 'http://') !== FALSE) {
    $cloud_list = cloud_get_all_clouds_display_name();
    $admin_url = cloud_get_admin_url($cloud_list[$cloud_context], $cloud_context);
    drupal_set_message(t("'http://' is not required in AWS API. Please remove 'http://' from API URI: @url", array('@url' => $admin_url)), 'error');
    return FALSE;
  }
  if (_aws_cloud_download_describe_elastic_ips($cloud_context) == FALSE) {
    $cloud_list = cloud_get_all_clouds_display_name();
    cloud_set_config_error_message($cloud_list[$cloud_context], $cloud_context);
    return FALSE;
  }
  if (_aws_cloud_download_describe_instances($cloud_context) == FALSE) {
    return FALSE;
  }
  if (_aws_cloud_download_describe_ssh_keys($cloud_context) == FALSE) {
    return FALSE;
  }
  if (_aws_cloud_download_describe_security_groups($cloud_context) == FALSE) {
    return FALSE;
  }
  if (_aws_cloud_download_describe_zones($cloud_context) == FALSE) {
    return FALSE;
  }
  if (_aws_cloud_download_describe_snapshots($cloud_context) == FALSE) {
    return FALSE;
  }
  if (_aws_cloud_download_describe_volumes($cloud_context) == FALSE) {
    return FALSE;
  }
  return TRUE;
}


/**
 * Retrieve the images information and store it in the database of the respective sub-cloud(cloud_context)
 *
 * @param $cloud_context
 *        This is the sub-cloud who's information is to be downloaded
 * @return
 *        The downloaded image information is stored in the database tables of the respective sub-cloud
 */
function _aws_cloud_download_all_images($cloud_context) {
  _aws_cloud_download_describe_images($cloud_context);
}

/**
 * Download Zone related information
 * @param $cloud_context
 * @return
 */
function _aws_cloud_get_describe_zones_query($cloud_context) {
  $ec2_lib_zone_table  = aws_cloud_get_table_info( AWS_CLOUD_ZONE_TABLE );
  return $query = 'SELECT c.zone_name,  c.zone_state, c.region FROM {' . $ec2_lib_zone_table . '} c  where c.cloud_type = :cloud_type ';
}


/**
 * Describe the zones 
 */
function _aws_cloud_download_describe_zones($cloud_context) {
  $ec2_lib_zone_table  = aws_cloud_get_table_info( AWS_CLOUD_ZONE_TABLE );
  if (($obj_response = aws_cloud_sdk_describe_zones($cloud_context)) !== FALSE) {
    // delete out the old zones
    db_delete($ec2_lib_zone_table)->condition('cloud_type', $cloud_context)->execute();
    
    $insert_query = db_insert($ec2_lib_zone_table)
      ->fields(array('zone_name', 'zone_state', 'cloud_type', 'region'));
    $count = 0;
    foreach ($obj_response->availabilityZoneInfo->item as $item) {
      $insert_query->values(
        array(
          'zone_name' => $item->zoneName,
          'zone_state' => $item->zoneState,
          'cloud_type' => $cloud_context,
          'region' => isset( $item->regionName ) ? $item->regionName : '',
        )
      );
      $count++;
    }
    if ($count) {
      $insert_query->execute();
      cloud_audit_user_activity(
        array(
          'type' => 'user_activity',
          'message' => 'Updated: Availability Zone(s)',
          'link' => '',
        )
      );
    }
  }
  return TRUE;
}

/**
 * Function for bundling instances 
 */
function aws_cloud_db_bundle_instance($cloud_context, $instance_id, $nickname, $bucketname, $foldername, $volume_size, $status) {
  $aws_cloud_bundle_instances_table  = AWS_CLOUD_BUNDLE_INSTANCES_TABLE;
  $insert_query = db_insert($aws_cloud_bundle_instances_table)
    ->fields(array('cloud_context', 'instance_id', 'nickname', 'bucketname', 'foldername', 'volume_size', 'status', 'created'));

  $insert_query->values(
    array(
        'cloud_context' => $cloud_context,
        'instance_id' => $instance_id,
        'nickname' => $nickname,
        'bucketname' => $bucketname,
        'foldername' => $foldername,
        'volume_size' => $volume_size,
        'status' => $status,
        'created' => time(),
    )
  );
  $insert_query->execute();
  return TRUE;
}

/**
 * Function for getting bundles 
 */
function aws_cloud_db_bundle_instance_get($status) {
  $return_result = array();
  $result = db_select(AWS_CLOUD_BUNDLE_INSTANCES_TABLE, 'c')
    ->fields('c')
    ->condition('status', $status, '=')
    ->execute();

  foreach ($result as $details) {
    $values = array();
    foreach ($details as $key => $val) {
      $values[$key] = $val;
    }
    $return_result[] = $values;
  }
  return $return_result;
}

/**
 * Function to find the last bundle status of an instance
 */
function aws_cloud_db_bundle_instance_get_status($instance_id) {
  return db_select(AWS_CLOUD_BUNDLE_INSTANCES_TABLE, 'c')
    ->fields('c', array('status'))
    ->condition('instance_id', $instance_id)
    ->orderBy('created', 'DESC')
    ->range(0, 1)
    ->execute()
    ->fetchField();
}

/**
 * Function for bundling instances 
 */
function aws_cloud_db_bundle_instance_update_status($bundle_id, $instance_id, $status) {
  $num_updated = db_update(AWS_CLOUD_BUNDLE_INSTANCES_TABLE)
    ->fields(array(
      'status' => $status,
    ))
    ->condition('instance_id', $instance_id, '=')
    ->condition('bundle_id', $bundle_id)
    ->execute();
  return $num_updated;
}

