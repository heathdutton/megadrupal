<?php

/**
 * @file
 * Provides Drush tasks for de/provisioning/deprovisioning Shibboleth access.
 */

/**
 * Implements hook_drush_help().
 */
function shib_provision_drush_help($command) {
  switch ($command) {
    case 'drush:my-command':
      return dt('Run my command');
  }
}

/**
 * Implements hook_drush_command().
 */
function shib_provision_drush_command() {
  $items = array();
  $items['user-shib-status'] = array(
    'description' => dt('Display Shibboleth provisioning status for the specified user accounts.'),
    'aliases' => array('ushs'),
    'arguments'   => array(
      'users' => 'A comma delimited list of uids, user names, or email addresses.',
    ),
    'required-arguments' => 1,
    'examples' => array(
      'drush user-shib-status 5,user3' =>
        'Display Shibboleth provisioning status for the accounts with name, id, or email 5 or user3',
    ),
  );
  $items['user-shib-provision'] = array(
    'description' => dt('Provision Shibboleth authentication for the specified user accounts.'),
    'aliases' => array('ushp'),
    'arguments'   => array(
      'users' => 'A comma delimited list of uids, user names, or email addresses.',
    ),
    'required-arguments' => 1,
    'examples' => array(
      'drush user-shib-provision 5,user3' =>
        'Provision Shibboleth authentication for the accounts with name, id, or email 5 or user3',
    ),
  );
  $items['user-shib-deprovision'] = array(
    'description' => dt('De-provision Shibboleth authentication for the specified user accounts.'),
    'aliases' => array('ushd'),
    'arguments'   => array(
      'users' => 'A comma delimited list of uids, user names, or email addresses.',
    ),
    'required-arguments' => 1,
    'examples' => array(
      'drush user-shib-deprovision 5,user3' =>
        'De-provision Shibboleth authentication for the accounts with name, id, or email 5 or user3',
    ),
  );
  return $items;
}

function drush_shib_provision_user_shib_status($user_args) {
  $users = _drush_shib_provision_get_users_from_arguments($user_args);
  foreach ($users as $user) {
    $status = (shib_provision_is_provisioned($user[0]) ? dt("provisioned") : dt('not provisioned'));
    drush_print(dt("Current Shibboleth authentication status for uid !uid: !status",
                array('!uid' => $user[0], '!status' => $status)));
  }
}

function drush_shib_provision_user_shib_provision($user_args) {
  $users = _drush_shib_provision_get_users_from_arguments($user_args);
  foreach ($users as $user) {
    shib_provision_provision($user[0], $user[1], $user[2]);
    drush_log(dt("Provisioned uid !uid for Shibboleth authentication", array('!uid' => $user[0])), 'success');
  }
}

function drush_shib_provision_user_shib_deprovision($user_args) {
  $users = _drush_shib_provision_get_users_from_arguments($user_args);
  foreach ($users as $user) {
    shib_provision_deprovision($user[0], $user[1], $user[2]);
    drush_log(dt("De-provisioned uid !uid for Shibboleth authentication", array('!uid' => $user[0])), 'success');
  }
}

/**
 * Given a comma-separated list of users, return uid/name pairs
 * for users that match either by uid or email address.
 *
 * Adapted from _drush_user_get_users_from_arguments() in Drush 5.7
 */
function _drush_shib_provision_get_users_from_arguments($users) {
  $uids = array();
  if ($users !== '') {
    $users = explode(',', $users);
    foreach ($users as $user) {
      $uid = _drush_shib_provision_get_uid($user);
      if ($uid !== FALSE) {
        $uids[] = $uid;
      }
    }
  }
  return $uids;
}

/**
 * Get uid/name pair(s) from a uid, user name, or email address.
 * Returns a uid, or FALSE if none found.
 *
 * Adapted from _drush_user_get_uid() in Drush 5.7
 */
function _drush_shib_provision_get_uid($search) {
  // We use a DB query while looking for the uid to keep things speedy.
  $uids = array();
  $mail = array();
  if (is_numeric($search)) {
    if (drush_drupal_major_version() >= 7) {
      $uid_query = db_query("SELECT uid, name, mail FROM {users} WHERE uid = :uid OR name = :name", array(':uid' => $search, ':name' => $search));
    }
    else {
      $uid_query = db_query("SELECT uid, name, mail FROM {users} WHERE uid = %d OR name = '%d'", $search, $search);
    }
  }
  else {
    if (drush_drupal_major_version() >= 7) {
      $uid_query = db_query("SELECT uid, name, mail FROM {users} WHERE mail = :mail OR name = :name", array(':mail' => $search, ':name' => $search));
    }
    else {
      $uid_query = db_query("SELECT uid, name, mail FROM {users} WHERE mail = '%s' OR name = '%s'", $search, $search);
    }
  }
  while ($uid = drush_db_fetch_object($uid_query)) {
    $uids[$uid->uid] = $uid->name;
    $mail[$uid->uid] = $uid->mail;
  }
  switch (count($uids)) {
    case 0:
      return drush_set_error("Could not find a uid for the search term '" . $search . "'!");
      break;
    case 1:
      $uid = array_pop(array_keys($uids));
      $name = $uids[$uid];
      $mail = $mail[$uid];
      return array($uid, $name, $mail);
      break;
    default:
      drush_print('More than one user account was found for the search string "' . $search . '".');
      $uid = drush_choice($uids, 'Please choose a name:', '!value (uid=!key)');
      $mail = $mail[$uid];
      return array($uid, $name, $mail);
  }
}
