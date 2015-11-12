<?php

/**
 * @file
 *
 * Provides drush commands to retrieve repository permissions.
 */

/**
 * Implements hook_drush_command().
 */
function versioncontrol_project_drush_command() {
  $commands = array();
  $commands['vcs-auth-data'] = array(
    'aliases' => array('vad'),
    'description' => 'Retrieve a json encoded string of authorization data for a given project\'s repository.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'arguments' => array(
      'project uri' => 'The uri of the project for which to retrieve auth data.',
    ),
  );
  $commands['vcs-auth-check-user-pass'] = array(
    'aliases' => array('vcup'),
    'description' => 'Check to see if a password matches a username.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'arguments' => array(
      'username' => 'The user to check.',
      'password' => 'The password to match.',
    ),
  );
  return $commands;
}

/**
 * Returns a json encoded string of authorization data.
 */
function drush_versioncontrol_project_vcs_auth_data($project_uri = '') {
  drush_print(json_encode(versioncontrol_project_get_auth_data($project_uri)));
}

/**
 * Returns 'true' if a the username and passowrd match
 *
 * @todo: We could do this just via DRUPAL_BOOTSTRAP_DATABASE and a direct
 * query on {users} instead of a full bootstrap and user_load().
 */
function drush_versioncontrol_project_vcs_auth_check_user_pass($username = '', $password = '') {
  $account = array_shift(user_load_multiple(array(), array('name' => $username)));
  if (!empty($account) && md5($password) == $account->pass) {
    drush_print('true');
  }
  drush_print('false');
}
