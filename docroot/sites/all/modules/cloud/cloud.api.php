<?php

/**
 * @file
 * Hooks provided by Cloud.
 * This is a documentation file
 * 
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 * 
 */

/**
 * This hook lets subclouds implement cloud actions.
 * @param string $op
 *  The operation to be executed: launch, launch_using params, 
 *  terminate, check_key_sg_data, check_key_data, check_sg_data, get_images_count,
 *  backup, detach_volume, check_snapshot_completion, check_volume_attached_status, check_instance_terminated
 *  get_instance_lock_status
 *  NOTE: for get_instance_lock_status, the params array should contain the instance/vm id against the instance_id key, for all clouds
 * @param array $params
 *  An array contain all the required information.  The cloud_context is passed
 *  as one of the array elements. 
 */
function hook_cloud_action($op, $params = array()) {
   switch ($op) {
      case 'launch':
         break;
      case 'launch_using_params':
         break;
      case 'post_launch':
        break;
      case 'terminate':
         break;
      case 'check_key_sg_data':
         break;
      case 'check_key_data':
         break;
      case 'check_sg_data':
         break;
      case 'get_images_count':
         break;
      case 'backup':
         break;
      case 'detach_volume':
         break;
      case 'check_snapshot_completion':
         break;
      case 'check_volume_attached_status':
         break;
      case 'check_instance_terminated':
         break;
      case 'get_instance_lock_status':
         break;
   }
}


/**
 * This hook returns the list of instances presented in an enabled
 * sub-cloud.
 * @param string $cloud_context
 *  The sub-cloud to return information from
 * @param array $filter
 *  An array of filters passed from the UI
 */
function hook_cloud_get_all_instances($cloud_context, $filter = array()) {
  //return your instance information from the database 
}

/**
 * This hook returns data about a particular sub-cloud
 * @param string $cloud_context
 *  The sub-cloud to query and return data
 * @param array $filter
 *  An array of filters passed from the UI
 */
function hook_cloud_get_instance($cloud_context, $filter = array()) {
  //return your sub-cloud information
}

/**
 * This hook returns the ssh key for a particular user
 * @param array $params
 *  The param array contains the key_name, cloud_context
 */
function hook_cloud_get_ssh_key($params) {
  //return your ssh key information from the database
}

/**
 * This hook is called when data about a particular sub-cloud
 * needs to be downloaded.
 * @param string $cloud_context
 *  The sub-cloud for which the information is to be retrieved
 */
function hook_cloud_update_data($cloud_context) {
  //download your cloud data here  
}

/**
 * This hook sets some initialization information about a 
 * particular sub-cloud.  This hook isn't really utilized
 * with the new cloud administration ui. Sub-clouds that 
 * have not been moved to the new ui still uses this hook
 * @param string $cloud_context
 *  Cloud context is passed if your module implements 
 *  more than one variant of your cloud.  For example: the 
 *  OpenStack, Eucalyptus and Amazon EC2 implementations are
 *  handled by one module.  cloud_context is used to distingush 
 *  the different sub-clouds.
 */
function hook_cloud_set_info($cloud_context = '') {
  return array(
    'cloud_display_name' => DISPLAY_NAME,
    'cloud_name' => CONTEXT,
    'module' => 'xcp',
    'base_cloud' => 'xcp',
    'instance_types' => array( 
      XCP_DEFAULT_INSTANCE_TYPE => XCP_DEFAULT_INSTANCE_TYPE,
    ),
    'cloud_pricing_data' => array(
        XCP_DEFAULT_INSTANCE_TYPE => array(
        'instance_type' =>  XCP_DEFAULT_INSTANCE_TYPE,
        'description' =>  t('Default'),
        'linux_or_unix_cost' => '0.085',
        'windows_cost' => '0.089',
      ),
    ),
  );
}

/***** New hooks added to cloud module******/

/**
 * This hook is called when a sub-cloud row is saved
 * into the table cloud_clouds.  This allows sub-cloud modules to 
 * take the data and store it in different places or 
 * do something else with it. The cloud_pricing and cloud_server_template
 * modules have implemented this hook
 * @param string $op
 *  The op can be 'create' or 'edit'
 * @param object $cloud
 */
function hook_cloud_save($op, $cloud) {
  //do something with the data
}

/**
 * This hook is called when a sub-cloud row is 
 * deleted from the table cloud_clouds.  The cloud_pricing 
 * and cloud_server_template modules have implemented this hook.
 * @param unknown_type $cloud_name
 */
function hook_cloud_delete($cloud_name) {
  //do something to the sub-cloud
}

/**
 * Thsi hook is called to return a specific piece
 * of information about a sub-cloud.  For example, the
 * cloud module will need to retrieve the host_uri, or 
 * user/password.  the aws_cloud module implements this hook.
 * @param string $cloud_name
 * @param string $key
 */
function hook_cloud_get_info($cloud_name, $key) {
  //return any name/value pairs
}


