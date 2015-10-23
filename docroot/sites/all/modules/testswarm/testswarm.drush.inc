<?php

/**
 * @file
 * Drush commands for the testswarm module
 */

/**
 * Implements hook_drush_command().
 */
function testswarm_drush_command() {
  $items['testswarm-set-githash'] = array(
    'description'=> dt('Sets the new githash and runs the tests for all browsers if necessary'),
    'arguments' => array(
      'githash' => dt('The hash of the commit.'),
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function testswarm_drush_help() {
  switch($section) {
    case 'drush:testswarm-set-githash':
      return dt('Sets the new githash and runs the tests for all browsers if necessary');
      break;
  }
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_testswarm_set_githash_validate() {
  if (func_num_args () < 1) {
    return drush_set_error('DRUSH_ARGUMENT_ERROR', dt('The hash argument is required.'));
  }
}

/**
 * Implements drush_COMMAND().
 */
function drush_testswarm_set_githash($hash) {
  $old_hash = variable_get('testswarm_githash', '');
  $run_tests = ($old_hash != $hash);
  variable_set('testswarm_githash', $hash);
  if ($run_tests) {
    testswarm_run_browserstack_tests();
  }
}