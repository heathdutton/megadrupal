<?php

/**
 * @file
 * Provides failover feature.
 * Works with Cloud, Cluster and Scripting module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

/**
 * Get all failover scenarios 
 */
function _cloud_failover_get_scenarios($sql_col, $filter, $table_sort) {

  $query =
     'SELECT *, a.port as port,  a.fsid as cloud_failover_command_id,  e.name as script_name, d.name as user_name, b.name as command_desc, c.description as action_desc FROM
                {' . CLOUD_FAILOVER_SCENARIO_TABLE . '} a
      LEFT JOIN {' . CLOUD_FAILOVER_COMMANDS_TABLE . '} b on a.script_id=b.script_id
      LEFT JOIN {' . CLOUD_FAILOVER_ACTIONS_TABLE . '} c on a.faid=c.faid
      LEFT JOIN {' . CLOUD_FAILOVER_DRUPAL_USERS_TABLE . '} d on a.uid=d.uid
      LEFT JOIN {' . CLOUD_SCRIPTING_TABLE . '} e on a.script_id=e.script_id ' ;
  
  $query = $query . " WHERE " . $sql_col . " like '%" . $filter . "%'";
  $query = $query . ' group by a.fsid ';
  
  if (isset($table_sort)) {
    
    $query = $query . '  ' . $table_sort ;
  }

  return $query;
}

/**
 * Get failover scenario by id
 */
function _cloud_failover_get_scenarios_fsid($fsid) {

  $query =
     "SELECT *, a.port as port,  a.fsid as cloud_failover_command_id,  e.name as script_name, d.name as user_name, b.name as command_desc, c.description as action_desc FROM
                {" . CLOUD_FAILOVER_SCENARIO_TABLE . "} a
      LEFT JOIN {" . CLOUD_FAILOVER_COMMANDS_TABLE . "} b on a.script_id=b.script_id
      LEFT JOIN {" . CLOUD_FAILOVER_ACTIONS_TABLE . "} c on a.faid=c.faid
      LEFT JOIN {" . CLOUD_FAILOVER_DRUPAL_USERS_TABLE . "} d on a.uid=d.uid
      LEFT JOIN {" . CLOUD_SCRIPTING_TABLE . "} e on a.script_id=e.script_id
    WHERE fsid like '%$fsid%' group by a.fsid ";

  return $query;
}

/**
 * Get all scenarios
 */
function _cloud_failover_get_all_scenarios() {

  $query = 'SELECT * FROM {' . CLOUD_FAILOVER_SCENARIO_TABLE . '} a';

  return $query;
}

/**
 * Get script cound
 * @return type
 */
function _cloud_failover_get_count_db() {
  $script_count = 0;
  $query =  'SELECT count(*) as script_count FROM {' . CLOUD_FAILOVER_SCENARIO_TABLE . '} c ';
  $result = db_query($query);
  return $result->fetchField();
}

/**
 * Get failover command 
 */
function _cloud_failover_get_command_info($fpid) {
  return $query = "SELECT * FROM {" . CLOUD_FAILOVER_PROTOCOL_TABLE . "} c WHERE fpid like '%$fpid%' ";
}

/**
 * Delete scenario
 * @param unknown_type $module
 */
function _cloud_failover_delete_scenario($module) {
  $fsid = $_REQUEST['id'];

  db_delete(CLOUD_FAILOVER_SCENARIO_TABLE)
    ->condition('fsid', $fsid, '=')
    ->execute();
  
  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('Failover Scenario has been deleted: @failover_id', array('@failover_id' => $fsid)),
      'link' => '',
    )
  );

  drupal_set_message(t('Scenario has been deleted successfully.'));
  drupal_goto(CLOUD_FAILOVER_PATH . '/scenarios');
}

/**
 * Save a server scenario
 */
function _cloud_failover_save_server_scenario($server_id, $scenario_arr) {

  $insert_query = db_insert($ec2_lib_instances_table)
    ->fields(
      array(
      'serverid',
      'fsid',
      )
    );

  $scenario_values = array();
  foreach ($scenario_arr as $scenario ) {
    $scenario_values[] =
      array(
      'serverid' => $server_id,
      'fsid' => $scenario,
    );
  }
  
  if (count($scenario_values)) {
    $insert_query->values($scenario_values);
    $insert_query->execute();
  }
}

/**
 * Get failover status 
 */
function _cloud_failover_get_service_status_of_server($dns, $service) {

  $select_query = "SELECT * FROM " . CLOUD_FAILOVER_PREFIX . CLOUD_FAILOVER_MONITORING_LOGS_TABLE
                                   . "} WHERE alias='$dns' and service='$service' and last_update =
            ( SELECT max( last_update ) FROM cloud_failover_nagios_service_logs where alias='$dns' and service='$service' and  TIMESTAMPDIFF(MINUTE,  record_timestamp,  CURRENT_TIMESTAMP ) < 1 ) ";
  
  $failover_monitering_result = db_query( $select_query);

  foreach ($failover_monitering_result as $row) {
    return $row;
  }
  return NULL;
}

/**
 * Insert a failover scenario into the database
 */
function _cloud_failover_insert_scenario($form_values) {
  global $user;

  db_insert(CLOUD_FAILOVER_SCENARIO_TABLE)
    ->fields(array(
      'nickname',
      'check_interval',
      'retry_interval',
      'max_check_attempts',
      'faid',
      'csid',
      'script_id',
      'port',
      'uid' 
    ))
    ->values(
      array(
        $form_values['name_text'],
        $form_values['check_interval_text'],
        $form_values['retry_interval_text'],
        $form_values['max_check_attempts_text'],
        $form_values['cloud_failover_action_select'],
        $form_values['cloud_failover_command_select'],
        $form_values['cloud_failover_script_select'],
        $form_values['port_text'],
        $user->uid,
      )
    )->execute();
  
  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('New Failover Scenario has been added: @failover_scenario', array('@failover_scenario' => $form_values['name_text'])),
      'link' => '',
    )
  );
  return;
}

/**
 * Update a failover scenario 
 */
function _cloud_failover_update_scenario($form_values, $fsid = '') {

  $cmd_select = $form_values['cloud_failover_command_select']
                ? $form_values['cloud_failover_command_select']
                : $form_values['command_id'                   ] ;
  
  db_update(CLOUD_FAILOVER_SCENARIO_TABLE)
    ->fields(
      array(
        'nickname' => $form_values['name_text'],  
        'check_interval' => $form_values['check_interval_text'],
        'retry_interval' => $form_values['retry_interval_text'],
        'max_check_attempts' => $form_values['max_check_attempts_text'],
        'faid' => $form_values['cloud_failover_action_select'],
        'csid' => $cmd_select,
        'script_id' => $form_values['cloud_failover_script_select'],
        'port' => $form_values['port_text'],
      )
    )
    ->condition('fsid', $fsid, '=')
    ->execute();

  // User Activity Log
  cloud_audit_user_activity( 
    array(
      'type' => 'user_activity',
      'message' => t('Failover Scenario has been modified: @failover_scenario', array('@failover_scenario' => $fsid)),
      'link' => '',
    )
  );
  return;
}

/**
 * Get all failover scenarios
 */
function _cloud_failover_scenario_get_all() {
  $query  = _cloud_failover_get_all_scenarios();
  $result = db_query( $query );
  $failover_scenarios = array();
  foreach ($result as $failover) {
    $failover_scenarios[$failover->fsid] = $failover->nickname;
  }
  return $failover_scenarios;
}

/**
 * Get failover commands 
 */
function _cloud_failover_get_commands() {
  $result = db_select(CLOUD_FAILOVER_PROTOCOL_TABLE, 'c')
    ->fields('c')
    ->execute();
  
  $command_list[''] = '- Select -';
  
  while ($command = $result->fetchObject()) {
    $command_list[$command->fpid] = $command->description;
  }
  return $command_list;
}

/**
 * Get failover actions
 */
function _cloud_failover_get_actions() {
  $action_list[''] = '- Select Action -';
  $result = db_select(CLOUD_FAILOVER_ACTIONS_TABLE, 'c')
    ->fields('c')
    ->execute();
  while ($action = $result->fetchObject()) {
    $action_list[$action->faid] = $action->description;
  }
  return $action_list;
}

/**
 * Get all the executeable scripts
 */
function _cloud_failover_get_executable_scripts() { //Check failed servers with fail over Enabled.
  $query =  _cloud_scripting_get_scripts();
  $script_list_result = db_query($query);
  $script_list[''] = '- Select Script -';

  foreach ($script_list_result as $script) {
    $script_list[$script->script_id] = $script->name;
  }
  return $script_list;
}

/**
 * Get failover scenario
 */
function _cloud_failover_get_scenario($nickname) {
  return $query = "SELECT * FROM {" . CLOUD_FAILOVER_SCENARIO_TABLE . "} c WHERE nickname = '$nickname' ";
}

/**
 * Update failover instance id
 * @param type $cloud_context
 * @param type $old_instance_id
 * @param type $new_instance_id
 * @return type
 */
function _cloud_failover_update_failover_instance_id($cloud_context, $old_instance_id, $new_instance_id) {
  db_update(CLOUD_FAILOVER_INFO_TABLE)
    ->fields(array('instance_id' => $new_instance_id))
    ->condition('instance_id', $old_instance_id)
    ->condition('cloud_context', $cloud_context)
    ->execute();
  return;
}


/**
 * Update the failover flag
 * @param type $cloud_context
 * @param type $instance_id
 * @param type $template_id
 * @return type
 */
function _cloud_failover_update_failover_flag($cloud_context, $instance_id, $failover_flag) {

  $query = "select count(*) as count from {" . CLOUD_FAILOVER_INFO_TABLE . "} where instance_id='$instance_id' and cloud_context='$cloud_context' ";
  $result = db_query($query);

  if ($result->fetchField() == 0) {
    db_insert(CLOUD_FAILOVER_INFO_TABLE)
      ->fields(array('cloud_context', 'instance_id', 'failover'))
      ->values(array($cloud_context, $instance_id, $failover_flag))
      ->execute();
  }
  else {
    db_update(CLOUD_FAILOVER_INFO_TABLE)
      ->fields(array('failover' => $failover_flag))
      ->condition('instance_id', $instance_id)
      ->condition('cloud_context', $cloud_context)
      ->execute();
  }
  return;
}

/**
 * Update the EIP info
 * @param type $cloud_context
 * @param type $instance_id
 * @param type $failover_flag
 * @return type
 */
function _cloud_failover_update_eip_info($cloud_context, $instance_id, $pub_ip, $failover_flag) {
  $query = "select count(*) as count from {" . CLOUD_FAILOVER_INFO_TABLE . "} where instance_id='$instance_id' and cloud_context='$cloud_context' ";
  $result = db_query($query);

  if ($result->fetchField() != 0) {
    db_update(CLOUD_FAILOVER_INFO_TABLE)
      ->fields(array('failover' => $failover_flag, 'public_ip' => $pub_ip))
      ->condition('instance_id', $instance_id)
      ->condition('cloud_context', $cloud_context)
      ->execute();
  }
  return;
}

/**
 * Select failover flag from database
 * @param type $cloud_context
 * @param type $instance_id
 * @return type
 */
function _cloud_failover_instance_failover_flag($cloud_context, $instance_id) {
  $result = db_select(CLOUD_FAILOVER_INFO_TABLE, 'c')
    ->fields('c', array('failover'))
    ->condition('instance_id', $instance_id)
    ->condition('cloud_context', $cloud_context)
    ->execute();

  while ($row = $result->fetchObject()) {
    return $row->failover;
  }
  return 'Disabled';

}
