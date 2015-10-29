<?php

/**
 * @file
 * DB and API functions for Elastic IP
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Complete function for updating Amazon with ip address
 * and updating the database.
 */
function aws_cloud_associate_ip($cloud_context, $instance_id, $ip) {
  $result = FALSE;
  if (($response = aws_cloud_sdk_associate_address($cloud_context, $instance_id, $ip)) !== FALSE) {
    if ($response->return == 'true') {
      $result = _aws_cloud_save_associate_address($cloud_context, $instance_id, $ip);
    }
  }
  return $result;
}

/**
 * Complete function for disassociating ip address. Updates the local
 * database.
 */
function aws_cloud_disassociate_ip($cloud_context, $ip) {
  $result = FALSE;
  if (($response = aws_cloud_sdk_disassociate_address($cloud_context, $ip)) !== FALSE) {
    if ($response->return == 'true') {
      $result  = _aws_cloud_save_associate_address($cloud_context, '', $ip);
    }
  }
  return $result;
}

/**
 * Complete function for allocating IP address 
 */
function aws_cloud_allocate_ip($cloud_context) {
  $result = FALSE;
  if (($response = aws_cloud_sdk_allocate_address($cloud_context)) !== FALSE) {
    $result = _aws_cloud_save_allocate_address($cloud_context, $response);
  }
  return $result;  
}

/**
 * Complete function for releasing Ip address 
 */
function aws_cloud_release_ip($cloud_context, $ip) {
  $result = FALSE;
  if (($response = aws_cloud_sdk_release_address($cloud_context, $ip)) !== FALSE) {
    if ($response->return == 'true') {
      $result = _aws_cloud_release_address_save($cloud_context, $ip );
    }
  }
  return $result;
}

/**
 * Saves a new Elastic IP addres into the database
 * @param   $cloud_context
 * @param   $myclient
 * @param   $instance_id
 * @param   $public_ip
 * @return
 */
function _aws_cloud_save_associate_address($cloud_context, $instance_id, $public_ip) {

  $ec2_lib_elastic_ip_table    = aws_cloud_get_table_info( AWS_CLOUD_ELASTIC_IP_TABLE );

  db_update($ec2_lib_elastic_ip_table)
    ->fields(array(
      'instance_id' => $instance_id,
    ))
    ->condition('public_ip', $public_ip, '=')
    ->condition('cloud_type', $cloud_context, '=')
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('Elastic IP "@public_id" has been assigned to instance "@instance_id".', array(
        '@public_id' => $public_ip,
        '@instance_id' => $instance_id,
      )),
      'link' => '',
    )
  );

  return TRUE;
}

/**
 * Saves a new IP address into the database
 * @param   $cloud_context
 * @param   $myclient
 * @return
 */
function _aws_cloud_save_allocate_address($cloud_context, $obj_response) {

  $ec2_lib_elastic_ip_table = aws_cloud_get_table_info( AWS_CLOUD_ELASTIC_IP_TABLE );
  if (isset($obj_response->publicIp)) {
    $insert_query = db_insert($ec2_lib_elastic_ip_table)
      ->fields(array('public_ip', 'cloud_type', 'public_ip_name'))
      ->values(
        array(
          'public_ip' => $obj_response->publicIp,
          'cloud_type' => $cloud_context,
          'public_ip_name' => '- changeme -',
        )
      )
      ->execute();

    // User Activity Log
    cloud_audit_user_activity( 
      array(
        'type' => 'user_activity',
        'message' => t('Elastic IP has been added: @public_ip', array('@public_ip' => $obj_response->publicIp)),
        'link' => '',
      )
    );
    return TRUE;
  }
  return FALSE;
}

/**
 * Delete an IP address from the database
 * @param   $cloud_context
 * @param   $myclient
 * @param   $ip
 * @return
 */
function _aws_cloud_release_address_save($cloud_context, $ip) {
  $ec2_lib_elastic_ip_table    = aws_cloud_get_table_info( AWS_CLOUD_ELASTIC_IP_TABLE );
  
  db_delete($ec2_lib_elastic_ip_table)
    ->condition('public_ip', $ip)
    ->condition('cloud_type', $cloud_context)
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('Elastic IP has been deleted: @ip', array('@ip' => $ip)),
      'link' => '',
    )
  );
  return TRUE;
}


/**
 * Download all ip addresses from Amazon and store them in the database
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_download_describe_elastic_ips($cloud_context) {

  $ec2_lib_elastic_ip_table = aws_cloud_get_table_info( AWS_CLOUD_ELASTIC_IP_TABLE );

  if (($obj_response = aws_cloud_sdk_describe_addresses($cloud_context)) !== FALSE) {
    $id_nick_arr = cloud_get_all_nickname('public_ip', 'public_ip_name', $ec2_lib_elastic_ip_table );

    // delete elastic ip table
    db_delete($ec2_lib_elastic_ip_table)
      ->condition('cloud_type', $cloud_context)
      ->execute();
  
    $insert_query = db_insert($ec2_lib_elastic_ip_table)
      ->fields(
        array(
          'public_ip',
          'cloud_type',
          'public_ip_name',
          'instance_id',
        )
     );
  
    $count = 0;
    
    foreach ($obj_response->addressesSet->item as $item) {
      $instance_id  = '';
      if (empty($item->instanceId) || substr_count( $item->instanceId, 'available') > 0) {
        $instance_id  = '';
      }
      else {
        $instance_id  = $item->instanceId;
        $instance_id  = trim(str_replace('(eucalyptus)', '', $instance_id));
        $instance_id  = trim(str_replace('(admin)',    '', $instance_id));
      }

      $tmp_nickname = cloud_check_in_array($id_nick_arr, $item->publicIp);
      $insert_query->values( 
        array(
          'public_ip' => $item->publicIp,
          'cloud_type' => $cloud_context,
          'public_ip_name' => $tmp_nickname != NULL ? $tmp_nickname : $item->publicIp,
          'instance_id' => $instance_id,
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
          'message' => 'Updated: Elastic IP(s)',
          'link' => '',
        )
      );
    }
  }
  return TRUE;
}

/**
 * Get all elastic IP addresses
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_get_describe_elastic_ips_query($cloud_context) {

  $ec2_lib_elastic_ip_table = aws_cloud_get_table_info( AWS_CLOUD_ELASTIC_IP_TABLE );
  $ec2_lib_instances_table  = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_TABLE  );

  $query = db_select($ec2_lib_elastic_ip_table, 'c');
  $query
    ->leftjoin($ec2_lib_instances_table, 'd', 'c.instance_id = d.instance_id');

  $query->fields('c');
  $query->fields('d', array('instance_nickname') );

  return $query;
}


/**
 * Get elastic ip addresses
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_get_elastic_ips_query($cloud_context, $ip) {

  $ec2_lib_elastic_ip_table = aws_cloud_get_table_info( AWS_CLOUD_ELASTIC_IP_TABLE );
  $ec2_lib_instances_table  = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_TABLE  );

  $query = 'SELECT c.public_ip, c.public_ip_name, c.instance_id, d.instance_nickname FROM {' . $ec2_lib_elastic_ip_table . '} c 
  LEFT JOIN {' . $ec2_lib_instances_table . '} d on c.instance_id=d.instance_id where c.public_ip=:public_ip and c.cloud_type=:cloud_type '  ;
  return db_query($query, array(':public_ip' => $ip, ':cloud_type' => $cloud_context));
}

/**
 * Get count for elastic ips
 * @param   $cloud_context
 * @return
 */
function _aws_cloud_get_elastic_ips_list_count($cloud_context) {

  $ec2_lib_elastic_ip_table = aws_cloud_get_table_info( AWS_CLOUD_ELASTIC_IP_TABLE );
  $ec2_lib_instances_table  = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_TABLE  );
  
  $query = 'SELECT COUNT(*) as item_count FROM {' . $ec2_lib_elastic_ip_table . '} c 
  LEFT JOIN {' . $ec2_lib_instances_table . '} d on c.instance_id=d.instance_id and c.cloud_type =:cloud_type '  ;
  
  $result = db_query($query, array(':cloud_type' => $cloud_context));
  return $result->fetchField();
}

/**
 * Update the nickname for an elastic IP
 * @param type $cloud_context
 * @return string
 */
function _aws_cloud_update_ip_query($cloud_context, $ip_name, $ip) {
  $ec2_lib_elastic_ip_table    = aws_cloud_get_table_info( AWS_CLOUD_ELASTIC_IP_TABLE );
  $result = db_update($ec2_lib_elastic_ip_table)
    ->fields(array(
      'public_ip_name' => $ip_name
    ))
    ->condition('public_ip', $ip, '=')
    ->condition('cloud_type', $cloud_context, '=')
    ->execute();
  return $result;
}

