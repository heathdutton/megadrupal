<?php

/**
 * @file
 * Drush integration for state_flow_schedule
 */

/**
 * Implements hook_drush_help().
 */
function state_flow_schedule_drush_help($command) {
  switch ($command) {
    case 'drush:state_flow_schedule':
      return dt('Process nodes that have been scheduled to publish in state_machine.');
  }
}

/**
 * Implements hook_drush_command().
 */
function state_flow_schedule_drush_command() {
  $items = array();
  $items['process-items'] = array(
    'description' => dt('Process nodes that have been scheduled to publish in state_machine.'),
    /*@TODO
    'arguments'   => array(
      'nid'    => dt('An optional node id'),
      'vid'    => dt('An optional node revision id'),
      'time'    => dt('An optional strtotime() string in a valid format'),
    ),
    */
    'examples' => array(
      'Standard example' => 'drush state_flow_schedule',
    ),
    'aliases' => array('smsfs'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  return $items;
}

/**
 * Publish scheduled nodes via drush
 *
 */
function drush_state_flow_schedule_process_items() {
  global $user;
  $user = user_load(1);
  $results = state_flow_schedule_get_scheduled_nodes(NULL, NULL, time());

  foreach ($results as $data) {
    $node = node_load($data->nid, $data->vid);
    $msg = dt('Attempting to publish nid:!nid, vid:!vid', array('!nid' => $node->nid, '!vid' => $node->vid));
    drush_print("\n" . $msg . "\n");
    $state_flow = state_flow_load_state_machine($node, TRUE);
    $state_flow->fire_event('publish', $user->uid, 'Published via drush.');
  }
}