<?php

/**
 * @file
 * Provides usage estimate table based on user's activities.
 * Works with Cloud module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Generate billing report
 */
function _cloud_billing_generate_report($start, $end, $cloud_context = '', $instance_type = '') {
  $clouds     = array();
  $cloud_list = cloud_get_all_clouds();
  foreach ($cloud_list as $cloud) {
    $clouds[] = $cloud;
  }
  $search_where = '';
  if ($cloud_context) {
    $search_where = " and b.cloud_type='" . $cloud_context . "' ";
  }

  $search_instance_type = '';
  if ($instance_type) {
    $search_instance_type = " and a.instance_type='" . $instance_type . "' ";
  }

  if ($start && $end) {
    if ($end) {
      list($yr, $mo, $da) = explode('-', $end);
      $end = date('c',   mktime(0,   0,   0, $mo, $da + 1, $yr)); //changed it to start of next month
      $now = date('c'); //date('Y-m-dcH:i:s');
      $end = ($end < $now) ? $end : $now;
    }

    $start_search = "if (launch_time < '" . $start . "', '" . $start . "', launch_time) ";
    $end_search = "if (instance_state_name != 'terminated', '" . $end . "', if (terminated_date < '" . $end . "' and terminated_date > '" . $start . "', terminated_date, '" . $end . "')) ";

    $search_where_sub = " and launch_time < " . $end_search . " and (terminated_date > '" . $start . "' or terminated_date = '0000-00-00 00:00:00' or terminated_date IS null) and instance_state_name != 'pending' ";
  }

  $query = "SELECT a.instance_id, a.instance_type, a.instance_nickname, a.instance_state_name, a.cloud_type, 
    a.launch_time, a.terminated_date, b.linux_or_unix_usage, b.windows_usage 
    FROM  {" . CLOUD_BILLING_INSTANCES_DETAILS_TABLE . "} a 
    LEFT JOIN {" . CLOUD_PRICING_INSTANCES_TABLE . "} b 
    ON a.instance_type = b.instance_type where a.launch_time " . $search_instance_type . " IS NOT NULL 
    AND a.launch_time != '0000-00-00 00:00:00' 
    AND a.launch_time != '' AND (a.instance_state_name = 'running' 
    OR a.instance_state_name = 'terminated') 
    AND a.cloud_type = b.cloud_type " . $search_where;

  return $query;
}

/**
 * Query all the Instance Types details
 */
function _cloud_billing_get_instance_types($cloud_context = '') {
  $search_where = '';
  if ($cloud_context) {
    $search_where = " and a.cloud_type='" . $cloud_context . "' ";
  }

  $query =  "SELECT a.instance_type, b.id, b.description, b.linux_or_unix_usage, b.windows_usage,  " .
        "(select count(instance_id) from {" . CLOUD_BILLING_INSTANCES_DETAILS_TABLE . "} e where a.instance_type=e.instance_type " . $search_where . " group by e.instance_type) as tInstances,  " .
        "(select min(launch_time) from {" . CLOUD_BILLING_INSTANCES_DETAILS_TABLE . "} d where a.instance_type=d.instance_type " . $search_where . " group by d.instance_type) as start_date FROM {" . CLOUD_INSTANCE_TYPE_TABLE . "} a " . 
        "left join {" . CLOUD_PRICING_INSTANCES_TABLE . "} b on a.instance_type=b.instance_type " . $search_where . 
        " WHERE 1 " . $search_where . " group by a.instance_type ";

  return $query;
}

/**
 * Update the Cloud Billing details
 * @param $cloud_context
 *        This is cloud name
 * @return returns a sql-query
 */
function _cloud_billing_update_instance_query($cloud_context, $instance_id, $nickname) {
  $billing_instances_table = CLOUD_BILLING_INSTANCES_DETAILS_TABLE;

  $num_updated = db_update($billing_instances_table) 
    ->fields(array(
    'instance_nickname' => $nickname,
  ))
    ->condition('instance_id', $instance_id, '=')
    ->condition('cloud_type', $cloud_context, '=')
    ->execute();


  return $num_updated;
}

/**
 * Delete the Cloud Billing details on Cloud Delete
 * @param $cloud_context
 *        This is cloud name
 * @return returs and sql-query
 */
function _cloud_billing_delete_data($cloud_context) {
  $billing_instances_table = CLOUD_BILLING_INSTANCES_DETAILS_TABLE;
  $billing_instance_delete = "DELETE FROM {" . $billing_instances_table . "} where and cloud_type = '$cloud_context'";
  return $billing_instance_delete;
}

/**
 * Function to get the details of Individual Instances of Cloud
 */
function _cloud_billing_get_all_instances() {
  $billing_instances_table = CLOUD_BILLING_INSTANCES_DETAILS_TABLE;
  $billing_instance_select = "SELECT * FROM {" . $billing_instances_table . "} ";

  $query_result = cloud_get_db_results($billing_instance_select);

  $instances_details = array();
  foreach ($query_result as $values) {
    $instances_details[$values['instance_id']] = $values;
  }

  return $instances_details;
}

/**
 * Function to check count of Instances
 * @param string $instance_id
 * @param string $cloud_context
 * @return returns a number
 */

function _cloud_billing_check_for_instance($instance_id, $cloud_context = '') {
  $check_query = "SELECT count(*) as tCount FROM {"
               . CLOUD_BILLING_INSTANCES_DETAILS_TABLE
               . "} where instance_id='" . $instance_id . "' and cloud_type='" . $cloud_context . "' ";

  $check_result = db_query( $check_query );
  if ($check_result) {
    $count = $check_result->fetchField();
    return $count;
  }

  return 0;
}

/**
 * Function to update the Instances details where Cloud Instances are fetched.
 * @param $cloud_context
 *        This is cloud name
 * @param $instance_id
 *        This is instance id
 * @param $instance_state
 *        This is instance status
 * @param $launch_time
 *        This is instance launched date
 * @param $instance_type
 *        This is instance type
 *
 */
function _cloud_billing_update( $cloud_context, $instance_id = '', $instance_state = '', $launch_time = '', $instance_type = '') {
  if (empty($instance_id)) {
    return;
  }

  if ( _cloud_billing_check_for_instance($instance_id, $cloud_context) == 0) {
    $id = db_insert(CLOUD_BILLING_INSTANCES_DETAILS_TABLE)
      ->fields(array(
        'instance_id' => $instance_id,
        'instance_state_name' => $instance_state,
        'launch_time' => $launch_time,
        'instance_type' => $instance_type,
        'cloud_type' => $cloud_context,
      ))
      ->execute();
  }

  $update_query  =  "update {" . CLOUD_BILLING_INSTANCES_DETAILS_TABLE
                   . "} set instance_state_name='"
                   . $instance_state
                   . "' where instance_id='"
                   . $instance_id . "' ";

  $update_result = db_query( $update_query );


  $update_query  = "update {"
                 . CLOUD_BILLING_INSTANCES_DETAILS_TABLE
                 . "} set terminated_date='" . date('c')
                 . "' where instance_state_name='terminated' and terminated_date IS NULL";
  $update_result = db_query( $update_query );

  return;
}

/**
 * Function to update Cloud Instance Details table on cron run for Mapped Instances
 */
function _cloud_billing_syncronize_instances($cloud_context, $instance_id, $query_args) {
  $count_args = count($query_args);

  for ($i = 0; $i < $count_args; ) {
    $field_name  = $query_args[$i];
    $field_value = $query_args[$i + 1];

    $billing_instance_update = "update {" . CLOUD_BILLING_INSTANCES_DETAILS_TABLE . "} set $field_name='$field_value' where instance_id= '$instance_id' and cloud_type = '$cloud_context'";
    //Update the Instances details table
    $update_query = db_query($billing_instance_update);
    $i = $i + 2;
  }
}

/**
 * Function to update Cloud Instance Details table on cron run for Oprhaned Instances
 */

function _cloud_billing_update_instances($instance_id, $query_args) {
  $billing_instances_table = CLOUD_BILLING_INSTANCES_DETAILS_TABLE;
  $billing_status_update = "update {" . $billing_instances_table . "} set instance_state_name='terminated' where terminated_date !=''";
  $billing_nickname_update = "update {" . $billing_instances_table . "} set instance_nickname='$instance_id' where instance_nickname IS NULL AND instance_id= '$instance_id'";

  $count_args = count($query_args);

  for ($i = 0; $i < $count_args; ) {
    $field_name  = $query_args[$i];
    $field_value = $query_args[$i + 1];

    $billing_instance_update = "update {" . $billing_instances_table . "} set $field_name='$field_value' where terminated_date IS NULL AND instance_id= '$instance_id'";

    //Update the Instances details table
    $update_query = db_query($billing_instance_update);
    $update_status = db_query($billing_status_update);

    $i = $i + 2;
  }

  $update_nickname = db_query($billing_nickname_update);
}

/**
 * Function to check for Duplicate Instance for same Cloud Type
 */
function _cloud_billing_check_duplicate($cloud_context) {
  $billing_instances_table = CLOUD_BILLING_INSTANCES_DETAILS_TABLE;
  $billing_instance_query = "SELECT instance_id, cloud_type, COUNT( instance_id ) AS numberCount FROM {" . $billing_instances_table . "} WHERE cloud_type = '$cloud_context' GROUP BY instance_id HAVING (COUNT( instance_id ) >1) ";

  $query_result = db_query($billing_instance_query);

  $instances_details = array();
  foreach ($query_result as $rows) {
    $instances_details[$rows->instance_id] = $rows->cloud_type;
  }
  return $instances_details;
}

/**
 * Function to remove duplicate Cloud Instance Details on cron run
 */
function _cloud_billing_remove_instances($instance_id, $cloud_context) {
  $billing_instances_table = CLOUD_BILLING_INSTANCES_DETAILS_TABLE;
  $billing_instance_delete = "delete from {" . $billing_instances_table . "} where instance_id= '$instance_id' and cloud_type = '$cloud_context'";
  $update_query = db_query($billing_instance_delete);
}

/**
 * Function to update running instances 
 * @param unknown_type $instance_id
 * @param unknown_type $cloud_context
 */
function _cloud_billing_update_running($instance_id, $cloud_context) {
  $billing_instances_table = CLOUD_BILLING_INSTANCES_DETAILS_TABLE;
  $billing_instance_update = "update {" . $billing_instances_table . "} set terminated_date=NULL WHERE instance_id= '$instance_id' AND cloud_type = '$cloud_context'";

  $query_result = db_query($billing_instance_update);
}
