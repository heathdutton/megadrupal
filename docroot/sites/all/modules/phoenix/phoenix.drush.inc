<?php

/**
 * @file
 * Drush support for the Phoenix module. Implements phase 1 - saving users.
 */

/**
 * Implementats hook_drush_command().
 */
function phoenix_drush_command() {
  $items = array();
  $items['phoenix-save'] = array(
    'description' => 'Make snapshot of user logins in the /data/ subdirectory of the module.',
    'aliases' => array('phoenix'),
  );
  return $items;
}

/**
 * Implementats of hook_drush_help().
 */
function phoenix_drush_help($section) {
  switch ($section) {
    case 'drush:phoenix-save':
      return dt("Make snapshot of user logins in the /data/ subdirectory of the module. Later this info can be recovered by re-enabling the module 'phoenix'.");
  }
}

/**
 * Drush command callback.
 */
function drush_phoenix_save() {
  // Do the save and pray for nice permissions.
  $result = _phoenix_save();

  $result === FALSE ?
    drush_log(dt('Saving users failed!'), 'error') :
    drush_log(dt('!count users saved, will wait for resurrection.', array('!count' => $result)), 'ok') ;
}
