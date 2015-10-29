<?php

/**
 * @file
 * Provides failover feature.
 * Works with Cloud, Cluster and Scripting module.
 *
 * Copyright (c) 2010-2011 DOCOMO FInnovations, Inc.
 *
 */


// ############ Code for Displaying Failover #####################

/*
 function cloud_failover_monitor_instance_info($cloud) {

 $dns = $_GET['dns'];

 $indent = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";

 if (empty($dns)
 || strlen($dns) == 0)
 $dns = $_POST['dns'];

 if (empty($dns)
 || strlen($dns) == 0) {
 drupal_goto(CLOUD_FAILOVER_PATH . '/failover') ;
 return ;
 }

 $server_id = $_GET['server_id'];
 if (empty($server_id)
 || strlen($server_id) == 0)
 $server_id = $_POST['server_id'];

 if (empty($server_id)
 || strlen($server_id) == 0) {
 drupal_goto(CLOUD_FAILOVER_PATH . '/failover') ;
 return ;
 }

 $failover_monitor_data = _cloud_cluster_get_failover_status_db($dns) ;

 if (isset($failover_monitor_data) == FALSE || isset($failover_monitor_data -> address) == FALSE) {
 drupal_set_message(check_plain(t('Failover Monitoring data not present for dns:' . $dns)) ,  'error');
 drupal_goto(CLOUD_FAILOVER_PATH . '/failover') ;
 return ;
 }

 if ($failover_monitor_data -> status == 2) {
 $src_img_path = 'images/icon_failover_up.png' ;
 $status_image = _cloud_failover_get_image($src_img_path) ;
 $status = 'Up' ;
 }
 elseif (empty($failover_monitor_data -> status) == FALSE && $failover_monitor_data -> status != 2) {
 $src_img_path = 'images/icon_failover_down.png' ;
 $status_image = _cloud_failover_get_image($src_img_path) ;
 $status = 'Down' ;
 }
 else {
 $status = 'NA' ;
 }

 $status_image .= "&nbsp;";

 $form['Monitor_data']['Host_label'             ] = array('#type' => 'item', '#title' => 'Host');
 $form['Monitor_data']['Host_text'              ] = array(array('#value' => t($dns)));

 $form['Monitor_data']['Host_status_label'      ] = array('#type' => 'item', '#title' => $indent . 'Host Status');
 $form['Monitor_data']['Host_status_text'       ] = array(array('#prefix' => $status_image ,  '#value' => t($status)));

 $form['Monitor_data']['Status_Info_label'      ] = array('#type' => 'item', '#title' => $indent . 'Status Information');
 $form['Monitor_data']['Status_Info_text'       ] = array(array('#value' => t($failover_monitor_data -> plugin_output)));

 $form['Monitor_data']['Performance_Data_label' ] = array('#type' => 'item', '#title' => $indent . 'Performance Data');
 $form['Monitor_data']['Performance_Data_text'  ] = array(array('#value' => t($failover_monitor_data -> performance_data)));

 $form['Monitor_data']['Current_Attempt_label'  ] = array('#type' => 'item', '#title' => $indent . 'Current Attempt');
 $form['Monitor_data']['Current_Attempt_text'   ] = array(array('#value' => t($failover_monitor_data -> current_attempt . '/' . $failover_monitor_data -> max_attempts)));

 $form['Monitor_data']['Last_Check_Time_label'  ] = array('#type' => 'item', '#title' => $indent . 'Last Check Time');
 $form['Monitor_data']['Last_Check_Time_text'   ] = array(array('#value' => t(date('F j, Y, g:i a' , $failover_monitor_data -> last_check))));

 $form['Monitor_data']['Check_Type_label'       ] = array('#type' => 'item', '#title' => $indent . 'Check Type');
 $form['Monitor_data']['Check_Type_text'        ] = array(array('#value' => t($failover_monitor_data -> check_type)));

 $form['Monitor_data']['Check_Latency_label'    ] = array('#type' => 'item', '#title' => $indent . 'Check Latency / Duration');
 $form['Monitor_data']['Check_Latency_text'     ] = array(array('#value' => t($failover_monitor_data -> check_latency . ' / ' . $failover_monitor_data -> check_execution_time)));

 $form['Monitor_data']['Next_Check_label'       ] = array('#type' => 'item', '#title' => $indent . 'Next Check');
 $form['Monitor_data']['Next_Check_text'        ] = array(array('#value' => t(date('F j, Y, g:i a' , $failover_monitor_data -> next_check))));

 $form['Monitor_data']['Last_state_change_label'] = array('#type' => 'item', '#title' => $indent . 'Last State Change');
 $form['Monitor_data']['Last_state_change_text' ] = array(array('#value' => t($failover_monitor_data -> last_state_change)));

 $form['Monitor_data']['Last_Notification_label'] = array('#type' => 'item', '#title' => $indent . 'Last Notification');
 $form['Monitor_data']['Last_Notification_text' ] = array(array('#value' => t($failover_monitor_data -> last_notification)));

 $form['Monitor_data']['Last_Update_label'      ] = array('#type' => 'item', '#title' => $indent . 'Last Update');
 $form['Monitor_data']['Last_Update_text'       ] = array(array('#value' => t(date('F j, Y, g:i a' , $failover_monitor_data -> last_update))));

 $failover_commands_result = _cloud_failover_get_cluster_scenario($server_id) ;

 $service_cnt = 0 ;
 $cnt = count($failover_commands_result) ;

 while ($service_cnt < $cnt) {

 $command_name = $failover_commands_result[$service_cnt]['service'] ;
 $failover_service_data = _cloud_failover_get_service_status_of_server($dns, $command_name) ;
 if ($failover_service_data == NULL
 || empty($failover_service_data)
 || isset($failover_service_data) == FALSE) {
 $service_cnt++ ;
 continue ;
 }

 if ($failover_service_data -> status == 2) {
 $status = 'Up' ;
 $src_img_path = 'images/icon_failover_up.png' ;
 }
 elseif (empty($failover_service_data -> status) == FALSE && $failover_service_data -> status != 2) {
 $status = 'Down' ;
 $src_img_path = 'images/icon_failover_service_down.png' ;
 }
 else {
 $status = 'NA' ;
 }

 $status_image = _cloud_failover_get_image($src_img_path) . "&nbsp;";

 $form['Monitor_data_' . $service_cnt ]['Host_label'             ] = array('#type' => 'item', '#title' => 'Service');
 $form['Monitor_data_' . $service_cnt ]['Host_text'              ] = array(array('#prefix' => '<b>', '#suffix' => '</b>', '#value' => t($failover_service_data -> service)));

 $form['Monitor_data_' . $service_cnt ]['Host_status_label'      ] = array('#type' => 'item', '#title' => $indent . 'Host Status');
 $form['Monitor_data_' . $service_cnt ]['Host_status_text'       ] = array(array('#prefix' => $status_image ,  '#value' => t($status)));

 $form['Monitor_data_' . $service_cnt ]['Status_Info_label'      ] = array('#type' => 'item', '#title' => $indent . 'Status Information');
 $form['Monitor_data_' . $service_cnt ]['Status_Info_text'       ] = array(array('#value' => t($failover_service_data -> plugin_output)));

 $form['Monitor_data_' . $service_cnt ]['Performance_Data_label' ] = array('#type' => 'item', '#title' => $indent . 'Performance Data');
 $form['Monitor_data_' . $service_cnt ]['Performance_Data_text'  ] = array(array('#value' => t($failover_service_data -> performance_data)));

 $form['Monitor_data_' . $service_cnt ]['Current_Attempt_label'  ] = array('#type' => 'item', '#title' => $indent . 'Current Attempt');
 $form['Monitor_data_' . $service_cnt ]['Current_Attempt_text'   ] = array(array('#value' => t($failover_service_data -> current_attempt . '/' . $failover_service_data -> max_attempts)));

 $form['Monitor_data_' . $service_cnt ]['Last_Check_Time_label'  ] = array('#type' => 'item', '#title' => $indent . 'Last Check Time');
 $form['Monitor_data_' . $service_cnt ]['Last_Check_Time_text'   ] = array(array('#value' => t(date('F j, Y,  g:i a' , $failover_service_data -> last_check))));

 $form['Monitor_data_' . $service_cnt ]['Check_Type_label'       ] = array('#type' => 'item', '#title' => $indent . 'Check Type');
 $form['Monitor_data_' . $service_cnt ]['Check_Type_text'        ] = array(array('#value' => t($failover_service_data -> check_type)));

 $form['Monitor_data_' . $service_cnt ]['Check_Latency_label'    ] = array('#type' => 'item', '#title' => $indent . 'Check Latency / Duration');
 $form['Monitor_data_' . $service_cnt ]['Check_Latency_text'     ] = array(array('#value' => t($failover_service_data -> check_latency . ' / ' . $failover_service_data -> check_execution_time)));

 $form['Monitor_data_' . $service_cnt ]['Next_Check_label'       ] = array('#type' => 'item', '#title' => $indent . 'Next Check');
 $form['Monitor_data_' . $service_cnt ]['Next_Check_text'        ] = array(array('#value' => t(date('F j, Y, g:i a' , $failover_service_data -> next_check))));

 $form['Monitor_data_' . $service_cnt ]['Last_state_change_label'] = array('#type' => 'item', '#title' => $indent . 'Last State Change');
 $form['Monitor_data_' . $service_cnt ]['Last_state_change_text' ] = array(array('#value' => t($failover_service_data -> last_state_change)));

 $form['Monitor_data_' . $service_cnt ]['Last_Notification_label'] = array('#type' => 'item', '#title' => $indent . 'Last Notification');
 $form['Monitor_data_' . $service_cnt ]['Last_Notification_text' ] = array(array('#value' => t($failover_service_data -> last_notification)));

 $form['Monitor_data_' . $service_cnt ]['Last_Update_label'      ] = array('#type' => 'item', '#title' => $indent . 'Last Update');
 $form['Monitor_data_' . $service_cnt ]['Last_Update_text'       ] = array(array('#value' => t(date('F j, Y,  g:i a' , $failover_service_data -> last_update))));

 $service_cnt++ ;
 }

 $form['service_cnt'] = array('#type' => 'hidden', '#value' => $service_cnt) ;

 return $form ;
 }
 */
function _cloud_failover_get_image($src_img_path) {
  $img_path = cloud_get_module_base() . $src_img_path;
  $image_info = image_get_info($img_path);
  $attributes['class'] = 'image image-$label ' . (isset($attributes['class']) ? $attributes['class'] : '');
  $attributes['width'] = $image_info['width'];
  $attributes['height'] = $image_info['height'];
  // $status_image = theme('image' , $img_path ,  '', '' , $attributes) ;
  $status_image = theme( 'image', array(
    'path' => $img_path,
    'alt' => '',
    'title' => '',
    'width' => $image_info['width'],
    'height' => $image_info['height'],
    'attributes' => array('class' => $attributes['class']),
  ));

  return $status_image;
}
/*
 function theme_cloud_failover_monitor_instance_info($form) {

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data']['Host_label']);
 $row[]  = drupal_render($form['Monitor_data']['Host_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data']['Host_status_label']);
 $row[]  = drupal_render($form['Monitor_data']['Host_status_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data']['Status_Info_label']);
 $row[]  = drupal_render($form['Monitor_data']['Status_Info_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data']['Performance_Data_label']);
 $row[]  = drupal_render($form['Monitor_data']['Performance_Data_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data']['Current_Attempt_label']);
 $row[]  = drupal_render($form['Monitor_data']['Current_Attempt_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data']['Last_Check_Time_label']);
 $row[]  = drupal_render($form['Monitor_data']['Last_Check_Time_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data']['Check_Type_label']);
 $row[]  = drupal_render($form['Monitor_data']['Check_Type_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data']['Check_Latency_label']);
 $row[]  = drupal_render($form['Monitor_data']['Check_Latency_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data']['Next_Check_label']);
 $row[]  = drupal_render($form['Monitor_data']['Next_Check_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data']['Last_state_change_label']);
 $row[]  = drupal_render($form['Monitor_data']['Last_state_change_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data']['Last_Notification_label']);
 $row[]  = drupal_render($form['Monitor_data']['Last_Notification_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data']['Last_Update_label']);
 $row[]  = drupal_render($form['Monitor_data']['Last_Update_text']);
 $rows[] = $row;

 $service_max_cnt = $form['service_cnt']['#value'] ;

 for ($service_cnt = 0 ; $service_cnt < $service_max_cnt ; $service_cnt++) {
 $row    = array();
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Host_label']);
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Host_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Host_status_label']);
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Host_status_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Status_Info_label']);
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Status_Info_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Performance_Data_label']);
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Performance_Data_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Current_Attempt_label']);
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Current_Attempt_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Last_Check_Time_label']);
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Last_Check_Time_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Check_Type_label']);
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Check_Type_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Check_Latency_label']);
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Check_Latency_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Next_Check_label']);
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Next_Check_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Last_state_change_label']);
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Last_state_change_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Last_Notification_label']);
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Last_Notification_text']);
 $rows[] = $row;

 $row    = array();
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Last_Update_label']);
 $row[]  = drupal_render($form['Monitor_data_' . $service_cnt]['Last_Update_text']);
 $rows[] = $row;
 }

 $output  = theme('table',  NULL , $rows);
 $output .= drupal_render($form);

 return $output ;
 }
 */
