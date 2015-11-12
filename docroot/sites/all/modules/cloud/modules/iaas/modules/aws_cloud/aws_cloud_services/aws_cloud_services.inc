<?php
// aws_cloud_services.inc

/**
 * @file
 * AWS Cloud Services module implementation.
 */

// updated by yas 2015/05/06
// created by yas 2015/08/25

/**
* Callback for listing all instances.
*
* @return object
*/
function _aws_cloud_services_index() {

  $filter_params      = array();
  $all_instances_list = array();
  $cloud_list         = cloud_get_all_clouds();

  foreach ($cloud_list as $cloud_context) {

    if (user_access($cloud_context . ' ' . 'list instances') === FALSE) continue;

    $instances = cloud_get_instances($cloud_context, $filter_params );

    if (empty($instances) === FALSE) {

      $instances_list = array();

      foreach ($instances as $instance) {

        unset($instance['action_data']); // remove $instance['action_data'] since it is redundant
        $instances_list[] = $instance;
      }

      $all_instances_list = array_merge( $all_instances_list, $instances_list );
    }
  }

  return array('servers', $all_instances_list);
}

/**
* Callback for launching instances.
*
* @param object $instance
* @return object
*/
function _aws_cloud_services_launch($cloud_context, $instance) {

  // Call launch instances function
 
  return (object) array(
    'instance_id' => $instance->instance_id,
    'uri' => services_resource_uri(array('instance', $instance->instance_id)),
  );
}

/**
* Callback for retrieving instances.
*
* @param string $cloud_context
* @param string $instance_id
* @return object
*/
function _aws_cloud_services_retrieve($cloud_context, $instance_id) {

  $instance_info = _aws_cloud_get_instance_details_by_id($cloud_context, $instance_id);

  return array('instance', $instance_info);
}

/**
* Callback for deleting instances.
*
* @param string $instance_id
* @return object
*/
function _aws_cloud_services_terminate($cloud_context, $instance_id) {

  // needs to implement

  return (object) array(
    'instance_id' => $instance_id,
    'uri'         => services_resource_uri(array('instance', $instance_id)),
  );
}

/**
* Access callback for the instances.
*
* @param string $op
*  The operation that's going to be performed.
* @param array $args
*  The arguments that will be passed to the callback.
* @return bool
*  Whether access is given or not.
*/
function _aws_cloud_services_access($op, $args) {

  $access        = FALSE;
  $cloud_context = $args[0];
  $instance_id   = $args[1];

  switch ($op) {

    case 'view':
      $access = user_access($cloud_context . ' list instances');
      break;

    case 'launch':
      // call launch instance function here
      $access = user_access($cloud_context . ' launch instance');
      break;

    case 'terminate':
      // call terminate instance function here
      $access = user_access($cloud_context . ' terminate instance');
      break;
  }

  return $access;
}
