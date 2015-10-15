<?php

include_once('reset_node_counter.module');

/**
 * @file reset_node_counter drush implementation.
 */

/**
 * Implements hook_drush_command().
 */
function reset_node_counter_drush_command() {
  $items['reset-node-counter'] = array(
    'description' => 'Reset node_counter entry for a given node.',
    'arguments' => array(
      'nid' => 'The node id to reset.',
    ),
    'required-arguments' => TRUE,
  );

  return $items;
}

/**
 * Implements drush_COMMAND_validate().
 */ 
function drush_reset_node_counter_validate() {
  $arg = func_get_args();
  if (!is_numeric($arg[0])) {
    return drush_set_error('NO_INT', dt('!arg is not numeric', array('!arg' => $arg[0])));
  } 
}

/**
 * Implements drush_COMMAND().
 */ 
function drush_reset_node_counter() {
  $arg = func_get_args();
  reset_node_counter_reset($arg[0]);  
}
