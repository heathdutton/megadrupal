<?php

/**
 * @file
 * Provides failover feature.
 * Works with Cloud, Cluster and Scripting module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

module_load_include('inc', 'cloud_failover', 'cloud_failover_nagios_json');

/**
 * Get running instances
 * @param type $cloud
 * @return type
 */
function _cloud_failover_failover_enabled_running_instances($cloud_context, $running_instances, $failover_flag, $cloud = '') {
  $instance_array = array();
  foreach ($running_instances as $instance) {

    $instance_array[] = $instance['instance_id'];
  }
  $result = db_select(CLOUD_FAILOVER_INFO_TABLE, 'c')
    ->fields('c')
    ->condition('c.failover', $failover_flag, '=')
    ->condition('c.instance_id', $instance_array, 'IN')
    ->execute();

  $fo_instance_list = array();

  foreach ($result as $instance) {

    $fo_instance_list[$instance->instance_id] = array();
    $fo_instance_list[$instance->instance_id]['instance_id'] = $instance->instance_id;
    $fo_instance_list[$instance->instance_id]['template_id'] = _cloud_failover_instance_info($cloud, $instance->instance_id);
  }

  return $fo_instance_list;
}


/**
 *
 * @param type $cloud_context
 * @param type $instance_id
 * @return type
 */
function _cloud_failover_instance_info($cloud_context, $instance_id) {

  return _cloud_server_templates_get_instance_info($cloud_context, $instance_id);
}


/**
 *
 * @param type $cloud
 * @return type
 */
function _cloud_failover_start_monitoring_instances($cloud_context, $cloud, $url) {

  $running_instances            = cloud_get_instances($cloud);

  $fo_instance_list             = _cloud_failover_failover_enabled_running_instances($cloud_context, $running_instances, CLOUD_FAILOVER_FLAG_READY_FOR_CHECK, $cloud);


  $all_failover_enabled_dns_arr = array();

  // For service the request has to be of the form:
  // http://ec2-75-101-141-33.compute-1.amazonaws.com/failover_monitor.php?service=
  // host:::ec2-174-129-169-94.compute-1.amazonaws.com:,:description:::cloud_PING:,:command:::check_ping!100.0, 20%!500.0, 60%
  // @host:::ec2-174-129-169-94.compute-1.amazonaws.com:,:port:::80:,:description:::cloud_HTTP:, :command:::check_http_port!!_PORT
  // @host:::ec2-174-129-169-94.compute-1.amazonaws.com:,:port:::82:,:description:::cloud_HTTP:, :command:::check_http_port!!_PORT
  $service_param = '';
  foreach ($fo_instance_list as $instance) {

    $instance_id = $instance['instance_id'];
    $template_id = $instance['template_id'];
    $dns         = $running_instances[$instance_id]['dns_name']; //RAMP

    $fo_sc_for_instance_list = _cloud_failover_get_scenarios_for_instance($template_id);

    foreach ($fo_sc_for_instance_list as $fo_sce) {
      $host_param_arr = array();

      $port               = empty($fo_sce['port']) ? $fo_sce['default_port'] : $fo_sce['port'];
      $command            = $fo_sce['command'];
      $service            = $fo_sce['service'];
      $check_interval     = $fo_sce['check_interval'];
      $retry_interval     = $fo_sce['retry_interval'];
      $max_check_attempts = $fo_sce['max_check_attempts'];

      $host_param_arr[] = "description:::$service";
      $host_param_arr[] = "host:::$dns";
      $host_param_arr[] = "port:::$port";
      $host_param_arr[] = "command:::$command";
      $host_param_arr[] = "check_interval:::$check_interval";
      $host_param_arr[] = "retry_interval:::$retry_interval";
      $host_param_arr[] = "max_check_attempts:::$max_check_attempts";

      $service_param .= implode(":,:", $host_param_arr) . "@";
    }
    $all_failover_enabled_dns_arr[] = $dns;
  }

  if (! empty($service_param)) {
    $service_param = substr( $service_param,   0,   -1 );
  }

  $all_failover_enabled_str = implode( ',', $all_failover_enabled_dns_arr );

  $request = array(
    'dns' => $all_failover_enabled_str   ,
    'service' => $service_param,
    'check_interval' => '1',
    'retry_interval' => '1',
    'max_check_attempts' => '5',
  );

//   $url_array = parse_url($url);

//   $host = $url_array['host'];
//   $path = $url_array['path'];

  //$host = $host . '/failover_request_handler.php';

  //aws_query_request($cloud_context, $host, $request);
  //drupal_htt_request($host, array($request))
  $endpoint = $url . '/?' . drupal_http_build_query($request);
  drupal_http_request($endpoint);
}


/**
 *
 * @param type $cloud
 * @return type
 */
function _cloud_failover_failover_enabled_instances($cloud) {

  $query        = "SELECT * from {" . CLOUD_FAILOVER_INFO_TABLE . "} WHERE failover = 'Enabled' AND cloud_context='$cloud' ";
  
  $result       = db_query($query);

  $fo_enabled_instance_list = array();

  foreach ($result as $instance) {
    
    $fo_enabled_instance_list[$instance->instance_id] = array();
    $fo_enabled_instance_list[$instance->instance_id]['instance_id'] = $instance->instance_id;
    $fo_enabled_instance_list[$instance->instance_id]['template_id'] = _cloud_failover_instance_info($cloud, $instance->instance_id);
  }

  return $fo_enabled_instance_list;
}


/**
 *
 * @param type $server_id
 * @return type
 */
function _cloud_failover_get_scenarios_for_instance($template_id) {
  if (empty($template_id)) {
    return array();
  }

  $query = "select * from {" . CLOUD_FAILOVER_SCENARIO_TABLE . "} as fs left join {" .
            CLOUD_FAILOVER_PROTOCOL_TABLE . "} fp on fs.csid = fp.fpid left join {" .
            CLOUD_SERVER_TEMPLATES_FAILOVER_SCENARIOS_TABLE . "} tfs on tfs.fsid = fs.fsid
            where tfs.server_template_id = '$template_id' ";

  $result = db_query($query);

  $fo_sc_list = array();
  foreach ($result as $row) {  
    //print $row->nickname. "<br/>";
    $fo_sc_list[$row->fsid] = array();
    $fo_sc_list[$row->fsid]['fsid']                 = $row->fsid;
    $fo_sc_list[$row->fsid]['nickname']             = $row->nickname;
    $fo_sc_list[$row->fsid]['check_interval']       = $row->check_interval;
    $fo_sc_list[$row->fsid]['retry_interval']       = $row->retry_interval;
    $fo_sc_list[$row->fsid]['max_check_attempts']   = $row->max_check_attempts;
    $fo_sc_list[$row->fsid]['faid']                 = $row->faid;
    $fo_sc_list[$row->fsid]['script_id']            = $row->script_id;
    $fo_sc_list[$row->fsid]['csid']                 = $row->csid;
    $fo_sc_list[$row->fsid]['port']                 = $row->port;
    $fo_sc_list[$row->fsid]['uid']                  = $row->uid;
    $fo_sc_list[$row->fsid]['fpid']                 = $row->fpid;
    $fo_sc_list[$row->fsid]['description']          = $row->description;
    $fo_sc_list[$row->fsid]['default_port']         = $row->default_port;
    $fo_sc_list[$row->fsid]['command']              = $row->command;
    $fo_sc_list[$row->fsid]['service']              = $row->service;

  }

  return $fo_sc_list;

}


/**
 *
 * @param unknown_type $cloud
 * @return unknown_type      foreach($fo_scenario_list )
 */
function _cloud_failover_launch_failed_server_using_nagios($cloud_context, $cloud, $url) { //$cloud indicates complete name

  $running_instances        = cloud_get_instances($cloud);
  $fo_enabled_instance_list = _cloud_failover_failover_enabled_instances($cloud);
  //print var_dump($fo_enabled_instance_list);die;
  foreach ($fo_enabled_instance_list as $instance) {

    if (array_key_exists($instance['instance_id'], $running_instances)) {

      //print "Host is running";
      //Server is running, check for services.
      $fo_scenario_list =  _cloud_failover_get_cluster_scenario($instance['template_id']);
      foreach ($fo_scenario_list as $scenario) {

        $service        = $scenario['service'];
        $dns            = $running_instances[$instance['instance_id']]['dns_name'];

        $service_status = _cloud_failover_get_service_details_for_scenario($service, $dns);

        if ($service_status == CLOUD_FAILOVER_SERVICE_FAILURE) {

          if ($scenario['faid']  == '1' ) {
            //Add new Script ready for execution.
            _cloud_scripting_register_script_save($instance['instance_id'], $instance['script_id'], '0', $cloud, $instance['template_id']);

            //Update flag as Script Under Execution
            _cloud_failover_update_failover_flag($cloud, $instance['instance_id'], CLOUD_FAILOVER_FLAG_SCRIPT_EXEC_IN_PROGRESS);
          }
          else {
            //Means terminate Old One and launch New Server.

            $filter_params['instance_id'] = $instance['instance_id'];
            $terminate_result = cloud_action_notify('terminate_using_params', $filter_params);
            if ($terminate_result->code == 200) {

              _cloud_failover_launch_new_instance($cloud_context, $cloud, $instance['instance_id'], $instance['template_id']);
            }
          }
        }
        elseif ($service_status === '0') {
          _cloud_failover_start_monitoring_instances($cloud_context, $cloud, $url);
        }
      }
    }
    else {

      _cloud_failover_launch_new_instance($cloud_context, $cloud, $instance['instance_id'], $instance['template_id']);
    }
  }
}


/**
 *
 * @param type $cloud_context
 * @param type $cloud
 * @param type $old_instance_id
 * @param type $template_id
 * @return type
 */
function _cloud_failover_launch_new_instance($cloud_context, $cloud, $old_instance_id,   $template_id) {

  $launch_params['template_id'] = $template_id;
  $launch_params['redirect']    = 'no_redirection';

  $instance_ids_array = cloud_action_notify('launch_using_params', $launch_params);

  foreach ($instance_ids_array as $key => $new_instance_id) {

    //Update Instance Id for newly launched instance.
    _cloud_failover_update_failover_instance_id($cloud, $old_instance_id, $new_instance_id);

    _cloud_failover_update_failover_flag($cloud, $new_instance_id, CLOUD_FAILOVER_FLAG_INITIATED);

    $params['old_instance_id'] = $instance_id;
    $params['new_instance_id'] = $new_instance_id;

    //Update instanceId in Static Ip tables.

    cloud_update_instance_staticip_mapping($cloud_context, $params);

  }

  return;
}


/**
 * function for fetching failover scenario associated with Instance.
 * @param $server_id
 * @return unknown_type
 */
function _cloud_failover_get_cluster_scenario($template_id) {

  $query = "SELECT * FROM {" . CLOUD_SERVER_TEMPLATES_FAILOVER_SCENARIOS_TABLE . "} as  stfs,  {" .
         CLOUD_FAILOVER_SCENARIO_TABLE . "} as fs, {" . CLOUD_FAILOVER_PROTOCOL_TABLE . "} as fp WHERE stfs.fsid = fs.fsid
          AND fs.csid = fp.fpid AND stfs.server_template_id = '$template_id' ";

  $result = db_query($query);

  $fo_scenario_list = array();

  foreach ($result as $row) {

    $fo_scenario_list[$row->fsid] = array();
    $fo_scenario_list[$row->fsid]['check_interval']     = $row->check_interval;
    $fo_scenario_list[$row->fsid]['retry_interval']     = $row->retry_interval;
    $fo_scenario_list[$row->fsid]['max_check_attempts'] = $row->max_check_attempts;
    $fo_scenario_list[$row->fsid]['faid']               = $row->faid;
    $fo_scenario_list[$row->fsid]['script_id']          = $row->script_id;
    $fo_scenario_list[$row->fsid]['service']            = $row->service;

  }

  return $fo_scenario_list;
}


/**
 *
 * @param type $service
 * @param type $dns
 * @return type
 */
function _cloud_failover_get_service_details_for_scenario($service, $dns) {

  $query      = "SELECT status, current_attempt, max_attempts  from {" . CLOUD_FAILOVER_SERVICE_MONITORING_NAGIOS_LOGS . " } where alias='$dns' and service='$service' ";

  $result       = db_query($query);
  $status       = '';

  foreach ($result as $row) {
    //print    $row->status . '  ' . $row->current_attempt . '  ' .$row->max_attempts;

    $status = $row->status;

    if ($row->current_attempt === $row->max_attempts) {
      return $row->status;
    }
    else {
      return '';
    }
  }

  return '0';
}

/**
 *
 * @param type $cloud_context
 * @param type $cloud
 * @param type $url
 * @return type
 */
//Lets leave the idea of start monitoring severs at this stage.
function _cloud_failover_check_newly_launched_servers_cron($cloud_context, $cloud, $url) {

  $fo_instance_list = _cloud_failover_failover_instances($cloud, CLOUD_FAILOVER_FLAG_INITIATED);

  //$update_monitoring_serverlist_flag = FALSE;

  foreach ($fo_instance_list as $instance) {
    $filter_params['column']        = 'instance_id';
    $filter_params['filter_value']  = $instance['instance_id'];

    $cloud_instance_list = cloud_get_instances($cloud, $filter_params); //Single server list.
    foreach ($cloud_instance_list as $cloud_instance) {

      $cloud_instance_state = $cloud_instance['state'];

      if ($cloud_instance_state == 'running') {

        //$update_monitoring_serverlist_flag = TRUE;
        _cloud_failover_update_failover_flag($cloud, $cloud_instance['instance_id'], CLOUD_FAILOVER_FLAG_SCRIPT_EXEC_IN_PROGRESS);
      }
    }
  }

  //if ($update_monitoring_serverlist_flag) {
//  print "Came Inside";
//  _cloud_failover_start_monitoring_instances($cloud_context, $cloud, $url);
  //}

  return;
}


/**
 *
 * @param type $cloud_context
 * @return type
 */
function _cloud_failover_check_script_executing_servers_cron($cloud_context, $cloud) {

  $fo_instance_list = _cloud_failover_failover_instances($cloud, CLOUD_FAILOVER_FLAG_SCRIPT_EXEC_IN_PROGRESS);

  foreach ($fo_instance_list as $instance) {

    $instance_id            = $instance['instance_id'];
    $params['instance_id']  = $instance_id;
    $instance_scripts       = _cloud_scripting_under_progress_scripts_for_instance($params);

    if (count($instance_scripts) == 0) { //Means all script execution is Done
      _cloud_failover_update_failover_flag($cloud, $instance_id, CLOUD_FAILOVER_FLAG_ELASTIC_IP_ASSIGNMENT);
    }
    else {
      //Skip this Server
    }
  }
  return;
}


/**
 *
 * @param type $cloud_context
 * @param type $cloud
 * @return type
 */
function _cloud_failover_check_instance_elastic_ip_cron($cloud_context, $cloud, $url) {

  $fo_instance_list = _cloud_failover_failover_instances($cloud, CLOUD_FAILOVER_FLAG_ELASTIC_IP_ASSIGNMENT);

  $update_monitoring_serverlist_flag = FALSE;


  foreach ($fo_instance_list as $instance) {

    $params['instance_id']  = $instance['instance_id'];
    $pub_ip                 = $instance['public_ip'];


    if (empty($pub_ip)) {

      $update_monitoring_serverlist_flag = TRUE;
      _cloud_failover_update_failover_flag($cloud, $instance['instance_id'], CLOUD_FAILOVER_FLAG_READY_FOR_CHECK);
    }
    else {

      $filter_params['column']        = 'instance_id';
      $filter_params['filter_value']  = $instance['instance_id'];

      $cloud_instance_list = cloud_get_instances($cloud, $filter_params); //Single server list.


      foreach ($cloud_instance_list as $instance) {
        $dns_name = $instance['dns_name'];
        $str      = str_replace(".", "-", $pub_ip);

        if (strpos($dns_name, $str)) {

          $update_monitoring_serverlist_flag = TRUE;
          _cloud_failover_update_failover_flag($cloud, $instance['instance_id'], CLOUD_FAILOVER_FLAG_READY_FOR_CHECK);


        }
        else {
          $associate_params['instance_id'] = $instance['instance_id'];
          $associate_params['pub_ip']      = $pub_ip;

          $result = cloud_associate_instance_staticip_mapping($cloud_context, $cloud, $associate_params);
          if ($result) {

            $update_monitoring_serverlist_flag = TRUE;
            _cloud_failover_update_failover_flag($cloud, $instance['instance_id'], CLOUD_FAILOVER_FLAG_ELASTIC_IP_ASSIGNMENT);

          }
        }
      }
    }

  }

  if ($update_monitoring_serverlist_flag) {
    _cloud_failover_start_monitoring_instances($cloud_context, $cloud, $url);
  }

  return;
}


/**
 *
 * @param type $failover_flag
 * @return type
 */
function _cloud_failover_failover_instances($cloud, $failover_flag) {

  $query        = "SELECT * from {" . CLOUD_FAILOVER_INFO_TABLE . "} WHERE failover = '$failover_flag' ";
  


  $result       = db_query($query);

  $fo_instance_list = array();


  foreach ($result as $instance) {

    $fo_instance_list[$instance->instance_id] = array();
    $fo_instance_list[$instance->instance_id]['instance_id'] = $instance->instance_id;
    $fo_instance_list[$instance->instance_id]['public_ip']   = $instance->public_ip;
    $fo_instance_list[$instance->instance_id]['template_id'] = _cloud_failover_instance_info($cloud, $instance->instance_id);
  }

  return $fo_instance_list;
}

/**
 * function will fetch the details of monitored hosts from Failover Monitoring Server.
 * details will be pulled from Nagios.
 * Dump the details ro DB for further analysis.
 * @return unknown_type
 */
function _cloud_failover_dump_host_status_from_nagios($cloud, $url, $userid, $pwd) {

  //Clean old data.

  db_query('TRUNCATE TABLE {' . CLOUD_FAILOVER_HOST_MONITORING_NAGIOS_LOGS . '}');


  $json = new NagiosJSON('url', $url);
  $json->setAuthentification( $userid, $pwd );

  $result = $json->getCompleteJson();

  if (is_array($result["hosts"]) && $result["hosts"] > 0) {
    foreach ($result["hosts"] as $hostname => $host) {
      $insert_query = "insert into { " . CLOUD_FAILOVER_HOST_MONITORING_NAGIOS_LOGS . "} (
                                    'address',   'alias',  'plugin_output',  'performance_data',  'status',
                                    'check_command',  'event_handler',  'has_been_checked',  'should_be_scheduled',  'check_execution_time',
                                    'check_latency',  'check_type',  'last_hard_state',  'last_check',  'next_check',
                                    'current_attempt', 'max_attempts', 'state_type', 'last_state_change', 'last_hard_state_change',
                                    'last_time_up',  'last_time_down',  'last_time_unreachable',  'last_notification',   'next_notification',
                                    'no_more_notifications',  'current_notification_number',  'notifications_enabled',  'problem_has_been_acknowledged',  'acknowledgement_type',
                                    'active_checks_enabled',  'passive_checks_enabled',  'event_handler_enabled',  'flap_detection_enabled',  'failure_prediction_enabled',
                                    'process_performance_data',  'obsess_over_host',  'last_update',  'is_flapping',  'percent_state_change',
                                    'scheduled_downtime_depth'
                                    )
                                    values( ";
      
      $insert_query = $insert_query . "'" . $host["address"] . "', " ;
      $insert_query = $insert_query . "'" . $host["alias"] . "', " ;
      $insert_query = $insert_query . "'" . $host["plugin_output"] . "', " ;
      $insert_query = $insert_query . "'" . $host["performance_data"] . "', " ;
      $insert_query = $insert_query . "'" . $host["status"] . "', " ;
      $insert_query = $insert_query . "'" . $host["check_command"] . "', " ;
      $insert_query = $insert_query . "'" . $host["event_handler"] . "', " ;
      $insert_query = $insert_query . "'" . $host["has_been_checked"] . "', " ;
      $insert_query = $insert_query . "'" . $host["should_be_scheduled"] . "', " ;
      $insert_query = $insert_query . "'" . $host["check_execution_time"] . "', " ;
      $insert_query = $insert_query . "'" . $host["check_latency"] . "', " ;
      $insert_query = $insert_query . "'" . $host["check_type"] . "', " ;
      $insert_query = $insert_query . "'" . $host["last_hard_state"] . "', " ;
      $insert_query = $insert_query . "'" . $host["last_check"] . "', " ;
      $insert_query = $insert_query . "'" . $host["next_check"] . "', " ;
      $insert_query = $insert_query . "'" . $host["current_attempt"] . "', " ;
      $insert_query = $insert_query . "'" . $host["max_attempts"] . "', " ;
      $insert_query = $insert_query . "'" . $host["state_type"] . "', " ;
      $insert_query = $insert_query . "'" . $host["last_state_change"] . "', " ;
      $insert_query = $insert_query . "'" . $host["last_hard_state_change"] . "', " ;
      $insert_query = $insert_query . "'" . $host["last_time_up"] . "', " ;
      $insert_query = $insert_query . "'" . $host["last_time_down"] . "', " ;
      $insert_query = $insert_query . "'" . $host["last_time_unreachable"] . "', " ;
      $insert_query = $insert_query . "'" . $host["last_notification"] . "', " ;
      $insert_query = $insert_query . "'" . $host["next_notification"] . "', " ;
      $insert_query = $insert_query . "'" . $host["no_more_notifications"] . "', " ;
      $insert_query = $insert_query . "'" . $host["current_notification_number"] . "', " ;
      $insert_query = $insert_query . "'" . $host["notifications_enabled"] . "', " ;
      $insert_query = $insert_query . "'" . $host["problem_has_been_acknowledged"] . "', " ;
      $insert_query = $insert_query . "'" . $host["acknowledgement_type"] . "', " ;
      $insert_query = $insert_query . "'" . $host["active_checks_enabled"] . "', " ;
      $insert_query = $insert_query . "'" . $host["passive_checks_enabled"] . "', " ;
      $insert_query = $insert_query . "'" . $host["event_handler_enabled"] . "', " ;
      $insert_query = $insert_query . "'" . $host["flap_detection_enabled"] . "', " ;
      $insert_query = $insert_query . "'" . $host["failure_prediction_enabled"] . "', " ;
      $insert_query = $insert_query . "'" . $host["process_performance_data"] . "', " ;
      $insert_query = $insert_query . "'" . $host["obsess_over_host"] . "', " ;
      $insert_query = $insert_query . "'" . $host["last_update"] . "', " ;
      $insert_query = $insert_query . "'" . $host["is_flapping"] . "', " ;
      $insert_query = $insert_query . "'" . $host["percent_state_change"] . "', " ;
      $insert_query = $insert_query . "'" . $host["scheduled_downtime_depth"] . "' ) " ;
      db_query( $insert_query);
    }
  }

}

/**
 * function for dumping Instance Serice details to DB.
 * Details will be pulled from Nagios.
 * @return unknown_type
 */
function _cloud_failover_dump_service_status_from_nagios($cloud, $url, $userid, $pwd) {

  db_query('TRUNCATE TABLE {' . CLOUD_FAILOVER_SERVICE_MONITORING_NAGIOS_LOGS . '}');

  $json = new NagiosJSON('url', $url);
  $json->setAuthentification( $userid, $pwd );
  $result = $json->getCompleteJson();

  if (is_array($result["services"]) && $result["services"] > 0) {
    foreach ($result["services"] as $host => $entry) {
      foreach ($entry as $service => $service_detail) {

        $insert_query = 'INSERT INTO {' . CLOUD_FAILOVER_SERVICE_MONITORING_NAGIOS_LOGS . "} (
                                    'alias', 'service',  'status' , 'plugin_output' , 'performance_data', 'check_command', 'event_handler', 'has_been_checked',
                                    'state_type', 'should_be_scheduled', 'check_execution_time', 'check_latency', 'check_type', 'last_hard_state',
                                    'current_attempt', 'max_attempts', 'last_state_change', 'last_hard_state_change', 'last_time_ok', 'last_time_warning',
                                    'last_time_unknown', 'last_time_critical', 'last_check', 'next_check', 'current_notification_number', 'last_notification',
                                    'next_notification', 'no_more_notifications', 'notifications_enabled', 'active_checks_enabled', 'passive_checks_enabled',
                                    'event_handler_enabled', 'problem_has_been_acknowledged', 'acknowledgement_type', 'flap_detection_enabled', 'failure_prediction_enabled',
                                    'process_performance_data', 'obsess_over_service', 'last_update', 'is_flapping', 'percent_state_change', 'scheduled_downtime_depth'
                                    )
                                    values  ( ";
       
        $insert_query = $insert_query . "'" . $host . "', ";
        $insert_query = $insert_query . "'" . $service;
        $insert_query = $insert_query . "'" . $service_detail["status"];
        $insert_query = $insert_query . "'" . $service_detail["plugin_output"];
        $insert_query = $insert_query . "'" . $service_detail["performance_data"];
        $insert_query = $insert_query . "'" . $service_detail["check_command"];
        $insert_query = $insert_query . "'" . $service_detail["event_handler"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["has_been_checked"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["state_type"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["should_be_scheduled"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["check_execution_time"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["check_latency"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["check_type"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["last_hard_state"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["current_attempt"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["max_attempts"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["last_state_change"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["last_hard_state_change"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["last_time_ok"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["last_time_warning"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["last_time_unknown"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["last_time_critical"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["last_check"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["next_check"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["current_notification_number"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["last_notification"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["next_notification"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["no_more_notifications"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["notifications_enabled"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["active_checks_enabled"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["passive_checks_enabled"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["event_handler_enabled"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["problem_has_been_acknowledged"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["acknowledgement_type"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["flap_detection_enabled"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["failure_prediction_enabled"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["process_performance_data"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["obsess_over_service"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["last_update"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["is_flapping"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["percent_state_change"] . "', ";
        $insert_query = $insert_query . "'" . $service_detail["scheduled_downtime_depth"] . "' ) ";

        db_query( $insert_query);
      }
    }
  }
  //}

}


#######################################################
#######################################################
#######################################################
//After Successful testing functions below can be deleted.





















/**
 *
 * @param unknown_type $cloud
 * @return unknown_type
 */
function _cloud_failover_start_monitoring_by_nagios($cloud) {

  $failover_instances_ready_for_monitoring = _cloud_failover_get_instances_ready_for_monitoring($cloud);
  for ($row_counter = 0; $row_counter < count($failover_instances_ready_for_monitoring); $row_counter++) {
    $server_id = $failover_instances_ready_for_monitoring[$row_counter]['server_id'];
    $instance_id = $failover_instances_ready_for_monitoring[$row_counter]['instance_id'];

    $dns =  cloud_get_dns_name($instance_id, $cloud);


//    Update monitoring flag in cloud_instance table.
    aws_cloud_instance_monitoring_status_update($cloud, $instance_id,   2);

//    Update SNMP entry
//    cloud_snmp_update_entry("add", $dns);

    //Update the last check time to current time
    _cloud_failover_update_lastchecktime($server_id);

//    Update the failover flag to 4
    _cloud_failover_update_failover_flag(CLOUD_FAILOVER_FLAG_READY_FOR_CHECK, $server_id);

    // set the instance state to be operational
    $result_script_status = aws_cloud_instance_status_update($instance_id, CLOUD_INSTANCE_STATUS_OPERATIONAL );

    //Now update the host entry in Nagios Configuration file.

    _cloud_failover_update_host_Entires($cloud);

    //Update the last check time to current time
    //Now not needed.
//    _cloud_failover_update_lastchecktime($server_id);
  }
  return;
}


/**
 * function for assigning elastic_ip to servers.
 * servers with cloud_failover_flag = 2
 * @return unknown_type
 */
function _cloud_failover_assign_elasticip_to_servers_using_nagios($cloud_context) {
  $failover_enabled_servers_ready_for_elasticip_assignment = _cloud_failover_get_servers_ready_for_elasticip_assignment_using_nagios();
  for ($row_counter = 0; $row_counter < count($failover_enabled_servers_ready_for_elasticip_assignment); $row_counter++) {
    $instance_id = $failover_enabled_servers_ready_for_elasticip_assignment[$row_counter]['instance_id'];

    //removing snmp entry for older instance(if there is any)
    $hostname = cloud_get_dns_name($instance_id, $cloud_context);
    cloud_snmp_update_entry('remove', $hostname);

    $elastic_ip = $failover_enabled_servers_ready_for_elasticip_assignment[$row_counter]["elastic_ip"];
    $server_id = $failover_enabled_servers_ready_for_elasticip_assignment[$row_counter]['server_id'];
        
    $result = aws_cloud_associate_ip($cloud_context, $instance_id, $elastic_ip);
    
    if ($result) {
      cloud_log_to_db('', 'Assigned the IP to the Instance->' . $instance_id);
      _cloud_failover_update_last_check_time($server_id,   '0');
      _cloud_failover_update_failover_flag(CLOUD_FAILOVER_FLAG_ELASTIC_IP_ASSIGNMENT_IN_PROGRESS, $server_id);
    }
    else {
      cloud_log_to_db('', "There was a problem while assigning the IP to the instance->" . $instance_id);
    }
  }
  return;
}


/**
 * function for executing scripts on servers which are needs to be checked for failover.
 * function will check if there is any server which needs to be checked for failover.(servers with cloud_failover_flag = 1)
 *   a) first it will check that server is in running state
 *  b) it will add script in ready for execution stage.
 *  c) then it will update the status for server to next stage(i.e. SCRIPT_EXECUTION_IN_PROGRESS).
 * Cron will take care of executing the script in instance.
 * @return unknown_type
 */
function _cloud_failover_exec_monitoring_scripts_on_enabled_server_using_nagios($cloud) {

  $failover_server_list_ready_for_script_exec = _cloud_failover_get_server_list_ready_for_script_execution();
  for ($row_counter = 0; $row_counter < count($failover_server_list_ready_for_script_exec); $row_counter++) {
    $server_id = $failover_server_list_ready_for_script_exec[$row_counter]['server_id'];
    $instance_id = $failover_server_list_ready_for_script_exec[$row_counter]['instance_id'];
    //Do this only when all scripts have been executed.
    if (!_failover_is_script_pending_for_execution($instance_id)) {
      _cloud_failover_update_failover_flag(CLOUD_FAILOVER_FLAG_SCRIPT_EXEC_IN_PROGRESS, $server_id);
    }
  }
  return;
}


function _cloud_failover_is_script_pending_for_execution($instance_id) {
  $query = "select * from { cloud_instance_script } where instance_id = '" . $instance_id . "' and script_executed = 0";
  $row_count = count(cloud_get_db_results($query));
  if ($row_count > 0 ) {
    cloud_log_to_db('There is a pending script for execution on Instance ID..->' . $instance_id,   '');
    return TRUE;
  }
  else {
    cloud_log_to_db('There is a no more pending scripts for execution on Instance Id..-> ' . $instance_id,   '');
    return FALSE;
  }
}


/**
 *
 * @param unknown_type $server_id
 * @param unknown_type $last_failover_check_time
 * @param unknown_type $failover_status
 * @return unknown_type
 */
function _cloud_failover_update_last_check_time_status($server_id, $last_failover_check_time, $failover_status) {
  if (empty($failover_status)) {
    $query = "update { cloud_cluster_servers } set last_failover_check_time = '" . $last_failover_check_time . "' where serverid = " . $server_id . "";
  }
  else {
    $query = "update { cloud_cluster_servers } set last_failover_check_time = '" . $last_failover_check_time . "',  failover_status = '" . $failover_status . "' where serverid = " . $server_id . "";
  }

  $query = db_query( $query );
}


/**
 *
 * @param $server_id
 * @param $failover_status
 * @return unknown_type
 */
function _cloud_failover_update_last_check_time($server_id,    $failover_status) {

  $query = "update { cloud_cluster_servers } set failover_status = '" . $failover_status . "' where serverid = " . $server_id . "";
  $query = db_query( $query );
}


/**
 *
 * @param unknown_type $dns
 * @param unknown_type $last_failover_checktime
 * @return unknown_type
 */
function _cloud_failover_get_latest_log_details_for_instance($dns, $last_failover_checktime) {

  $query = "select status,  record_timestamp,  current_attempt ,  max_attempts from { cloud_failover_host_monitoring_logs }
      where alias = '" . $dns . "'
      and record_timestaCLOUD_FAILOVER_HOST_MONITORING_NAGIOS_LOGSmp > '" . $last_failover_checktime . "' order by record_timestamp desc";

  return cloud_get_db_results($query);
}


/**
 *
 * @return unknown_type
 */
function _cloud_failover_get_servers_ready_for_elasticip_assignment_using_nagios() {

  // TODO: DEPENDENCY: AWS_CLOUD_* (by Ram) / ELIMINATE $_INSTANCES_TABLE
  $_INSTANCES_TABLE = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_TABLE );

  $query = "select ds.instance_id,  ds.elastic_ip,  ds.serverid from { " . CLOUD_CLUSTER_SERVER_TABLE . " } ds,  { " . $_INSTANCES_TABLE . " } inst
             where ds.instance_id = inst.instance_id and
              inst.instance_state_name = 'running' and ds.failover_flag = '" . CLOUD_FAILOVER_FLAG_SCRIPT_EXEC_IN_PROGRESS . "'";
  $failover_serverlist_ready_for_elasticip_change = cloud_get_db_results($query);
  return $failover_serverlist_ready_for_elasticip_change;
}


#######################Common Function to both Collectd & Nagios#################################


/**
 *
 * @param type $server_id
 * @return type
 */
function _cloud_failover_update_lastchecktime($server_id) {

  $query = "update " . CLOUD_CLUSTER_SERVER_TABLE . " set last_failover_check_time = CURRENT_TIMESTAMP where serverid = '" . $server_id . "'";
  db_query( $query );
  return;
}


/**
 *
 * @param String
 * @return Array
 */
function _cloud_failover_get_instances_ready_for_monitoring($cloud) {

  global $_ELASTIC_IP_TABLE;

  $query = 'select ds.serverid , ds.instance_id from { ' . CLOUD_CLUSTER_SERVER_TABLE . ' } ds, { ' . $_ELASTIC_IP_TABLE . ' } eip where cloud_failover_flag = '
         . CLOUD_FAILOVER_FLAG_ELASTIC_IP_ASSIGNMENT_IN_PROGRESS . ' and ds.instance_id = eip.instance_id';

  return cloud_get_db_results($query);

}


/**
 *
 * @param int $flag
 * @param int $server_id
 * @return
 */
//Commented by RAMP
//function _cloud_failover_update_failover_flag($flag, $server_id) {
//
//  $query = "update { ". CLOUD_CLUSTER_SERVER_TABLE . " } set cloud_failover_flag = '" . $flag . "' , failover_status = 2,  last_failover_check_time = CURRENT_TIMESTAMP where serverid = " . $server_id;
//  db_query( $query );
//  return;
//}


/**
 *
 * @return Array
 */
function _cloud_failover_get_server_list_ready_for_script_execution() {

  $_INSTANCES_TABLE    = aws_cloud_get_table_info( AWS_CLOUD_INSTANCES_TABLE );
  $query = "select ds.instance_id,  ds.serverid, ds.cluster_id,  ds.template_id, inst.dns_name from { " . CLOUD_CLUSTER_SERVER_TABLE . " } ds,  { " . $_INSTANCES_TABLE . " } inst where
            inst.instance_id = ds.instance_id and inst.instance_state_name = 'running' and ds.failover_flag = '" . CLOUD_FAILOVER_FLAG_INITIATED . "'";
  $failover_server_list_ready_for_script_exec = cloud_get_db_results($query);
  return $failover_server_list_ready_for_script_exec;
}


/*
 function _cloud_failover_stop_for_instance() {
 $instance_id = $_GET['id'] ;
 $server_id   = $_GET['server_id'] ;
 $cloud      = $_GET['cloud'] ;

 AWS_CLOUD_instance_monitoring_status_update( $cloud , $instance_id,  0) ;
 _cloud_failover_update_failover_flag(CLOUD_FAILOVER_FLAG_NO_FAILOVER, $server_id);

 drupal_set_message(check_plain("Stopped failover for instance $instance_id")) ;
 drupal_goto( $_GET['src']) ;
 return;
 }*/




































































#######################Code for monitoring using Collectd########################################
/**
 * function for checking that all servers are running with failover enabled.
 * if not then launch a new server.
 * @param unknown_type $cloud
 * @return unknown_type
 */
/*function _cloud_failover_check_failed_servers_using_collectd($cloud) {


 $failed_servers_enabled_for_failover = _cloud_failover_get_failed_servers_enabled($cloud);


 for ($row_counter = 0; $row_counter < count($failed_servers_enabled_for_failover); $row_counter++) {
 $server_id = $failed_servers_enabled_for_failover[$row_counter]['server_id'];
 $instance_id = $failed_servers_enabled_for_failover[$row_counter]['instance_id'];

 cloud_log_to_db('Checking details for InstanceID->>>>>>>>>>>>>>>>>', $instance_id);

 $hostname = cloud_get_dns_name($instance_id, $cloud);

 cloud_log_to_db('Server and Instance ID ->', $server_id . '---' . $instance_id . '---' . $hostname);

 if (trim($hostname) == '') {//Sever is terminated.Need to launch a new server.
 cloud_log_to_db("NO data for instance->", $instance_id);
 _cloud_failover_launch_failed_server($cloud, $server_id);
 _cloud_failover_update_failover_flag(CLOUD_FAILOVER_FLAG_INITIATED , $server_id);
 }
 else {
 $collectd_reading = cloud_snmp_get_readings($cloud , $hostname, CLOUD_FAILOVER_TIME_CHECK);
 if ($collectd_reading > 0) {
 cloud_log_to_db('', 'Instance -> ' . $instance_id . ' is working');
 }
 else { //Server is not responding. Need to launch a new server.
 cloud_log_to_db('Reading from Collectd is not proper -> ', $instance_id);
 _cloud_failover_launch_failed_server($cloud, $server_id);
 _cloud_failover_update_failover_flag(CLOUD_FAILOVER_FLAG_INITIATED, $server_id);
 }
 }
 }
 return;
 }
 */
/**
 *
 * @param $cloud
 * @param $server_id
 * @return
 */
/*function _cloud_failover_launch_failed_server($cloud, $server_id) {

 $query = _cloud_cluster_get_server_details_db() ;
 $query_args[] = $server_id;

 $result = db_query( $query, $query_args );

 $template_options = array() ;
 while ($depl = db_fetch_object($result)) {
 $grp_id  = explode(',', $depl->group_id) ;
 $result_launch = launch_new_instance(
 $depl->server_nickname,
 $depl->image_id ,
 1 ,
 $depl->key_name ,
 $grp_id  ,
 $depl->user_data ,
 $depl->instance_type ,
 $depl->placement ,
 $depl->kernel_id ,
 $depl->ramdisk_id
 ) ;
 }

 if ( $result_launch) {
 $instance_id = $_GET['new_instance_ids'][0] ;
 $update_query = _cloud_cluster_update_cluster_server_query() ;
 $query_args = array(
 'instance_id',
 $instance_id ,
 $server_id   ,
 ) ;
 $result = db_query( $update_query, $query_args );
 }
 else {
 cloud_log_to_db('Failed To Launch Failover Instance with Server ID', $server_id);
 }
 return;
 }
 */
/**
 *
 * @param unknown_type $cloud
 * @return unknown_type
 */
/*function _cloud_failover_start_monitoring_by_collectd($cloud) {

 $failover_instances_ready_for_monitoring = _cloud_failover_get_instances_ready_for_monitoring($cloud);
 for ($row_counter = 0; $row_counter < count($failover_instances_ready_for_monitoring); $row_counter++) {
 $server_id = $failover_instances_ready_for_monitoring[$row_counter]['server_id'];
 $instance_id = $failover_instances_ready_for_monitoring[$row_counter]['instance_id'];

 cloud_log_to_db('Server and Instance ID ->' , $server_id . '---' . $instance_id);

 //Update SNMP entry
 cloud_snmp_update_entry("add", $dns);
 //Update the failover flag to 4
 _cloud_failover_update_failover_flag(CLOUD_FAILOVER_FLAG_READY_FOR_CHECK , $server_id);

 // set the instance state to be operational
 $result_script_status = AWS_CLOUD_instance_status_update($instance_id, CLOUD_INSTANCE_STATUS_OPERATIONAL ) ;

 //Update monitoring flag in cloud_instance table.
 AWS_CLOUD_instance_monitoring_status_update($cloud , $instance_id,  2) ;
 //Update the last check time to current time
 _cloud_failover_update_lastchecktime($server_id);
 }
 return;
 }
 */

/**
 * function for executing scripts on servers which are needs to be checked for failover.
 * function will check if there is any server which needs to be checked for failover.(servers with cloud_failover_flag = 1)
 *   a) first it will check that server is in running state
 *  b) it will add script in ready for execution stage.
 *  c) then it will update the status for server to next stage(i.e. SCRIPT_EXECUTION_IN_PROGRESS).
 * Cron will take care of executing the script in instance.
 * @return unknown_type
 */
/*function _cloud_failover_exec_monitoring_scripts_on_enabled_server_using_collectd($cloud) {

 $monitor_script_id = '';
 $failover_server_list_ready_for_script_exec = _cloud_failover_get_server_list_ready_for_script_execution();

 if (count($failover_server_list_ready_for_script_exec) > 0 ) {
 $monitor_script_id = _cloud_scripting_get_id_from_master('MONITOR_SCRIPT');
 }

 print "\nMonitor Script ID->> " . $monitor_script_id . "\n";

 for ($row_counter = 0; $row_counter < count($failover_server_list_ready_for_script_exec); $row_counter++) {

 $instance_id   = $failover_server_list_ready_for_script_exec[$row_counter]['instance_id'  ];
 $server_id     = $failover_server_list_ready_for_script_exec[$row_counter]['server_id'    ];
 $cluster_id = $failover_server_list_ready_for_script_exec[$row_counter]['cluster_id'];
 $template_id   = $failover_server_list_ready_for_script_exec[$row_counter]['template_id'  ];
 $dns           = $failover_server_list_ready_for_script_exec[$row_counter]['dns_name'     ];
 $insert_flag   = _cloud_scripting_add_ready_for_exec_on_instance($instance_id, $monitor_script_id,  0, $cloud, $cluster_id,   $template_id);

 if ($insert_flag == 0) {
 _cloud_failover_update_failover_flag(CLOUD_FAILOVER_FLAG_SCRIPT_EXEC_IN_PROGRESS, $server_id);
 //cloud_snmp_update_entry($cloud , "add", $dns);
 }
 }
 return;
 }*/

/**
 *
 * @param unknown_type $cloud
 * @return unknown_type
 */
/*function _cloud_failover_assign_elasticip_to_servers_using_collectd($cloud_context) {
 $failover_enabled_servers_ready_for_elasticip_assignment = _cloud_failover_get_servers_ready_for_elasticip_assignment_using_collectd();
 for ($row_counter = 0; $row_counter < count($failover_enabled_servers_ready_for_elasticip_assignment); $row_counter++) {
 $instance_id = $failover_enabled_servers_ready_for_elasticip_assignment[$row_counter]['instance_id'];

 //removing snmp entry for older instance(if there is any)
 $hostname = cloud_get_dns_name($instance_id, $cloud_context);
 cloud_snmp_update_entry($cloud_context , 'remove', $hostname);

 $elastic_ip = $failover_enabled_servers_ready_for_elasticip_assignment[$row_counter]["elastic_ip"];
 $server_id = $failover_enabled_servers_ready_for_elasticip_assignment[$row_counter]['server_id'];
 $response = aws_ec2_api_associate_address( $cloud_context , $instance_id, $elastic_ip);

 $result = _AWS_CLOUD_save_associate_address( $cloud_context , $response , $instance_id , $elastic_ip);

 if ($result) {
 cloud_log_to_db('', 'Assigned the IP to the Instance->' . $instance_id);
 _cloud_failover_update_failover_flag(CLOUD_FAILOVER_FLAG_ELASTIC_IP_ASSIGNMENT_IN_PROGRESS, $server_id);
 }
 else
 cloud_log_to_db('', "There was a problem while assigning the IP to the instance->" . $instance_id);
 }
 return;
 }*/

/**
 *
 * @return unknown_type
 */
/*function _cloud_failover_get_servers_ready_for_elasticip_assignment_using_collectd() {

 // TODO: THIS SHOULD BE CALLED HERE. (by Ram)
 $_INSTANCES_TABLE    = AWS_CLOUD_get_table_info( AWS_CLOUD_INSTANCES_TABLE );

 $monitor_script_id = _cloud_scripting_get_id_from_master('MONITOR_SCRIPT');

 $query = "select ds.instance_id,  ds.elastic_ip,  ds.serverid from
 ". CLOUD_CLUSTER_SERVER_TABLE . " ds,  ". $_INSTANCES_TABLE . " inst ,  " . INSTANCES_CLOUD_SCRIPTING_TABLE . " script
 where ds.instance_id = inst.instance_id and
 inst.instance_state_name = 'running' and ds.failover_flag = '" . CLOUD_FAILOVER_FLAG_SCRIPT_EXEC_IN_PROGRESS . "' and
 script.instance_id = ds.instance_id and script.script_executed = 2
 and script.script_id = '" . $monitor_script_id . "'";

 $failover_serverlist_ready_for_elasticip_change = cloud_get_db_results($query);
 return $failover_serverlist_ready_for_elasticip_change;
 }*/
