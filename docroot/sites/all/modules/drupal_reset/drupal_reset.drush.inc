<?php

/**
 * @file
 * Define the site-reset Drush command.
 */

/**
 * Implements hook_drush_help().
 */
function drupal_reset_drush_help($command) {
  switch ($command) {
    case 'drush:site-reset':
      return dt('  Reset the site installation, start fresh.
  Drops all database tables and deletes all the files.');
  }
}

/**
 * Implements hook_drush_command().
 */
function drupal_reset_drush_command() {
  $items = array();
  $items['site-reset'] = array(
    'description' => dt('Reset the site installation, start fresh.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
    'arguments' => array(),
    'examples' => array(),
    'aliases' => array(),
  );
  return $items;
}

/**
 * Callback function for drush site-reset.
 *
 * @param $arg1
 *   An optional argument
 */
function drush_drupal_reset_site_reset($arg1 = NULL) {
  if (!drush_confirm(dt('You are about to drop all database tables and delete all the files for this site. Do you want to continue?'))) {
    return drush_user_abort();
  }

  require_once( dirname(__FILE__) . '/drupal_reset.lib.inc');

  if (!_drupal_reset_is_supported_database_configuration()) {
    drush_set_error('DRUPAL_RESET_DATABASE_CONFIGURATION_NOT_SUPPORTED', 'Your database configuration is not supported by the Drupal Reset module.
There must be one database (no master/slave) and the table prefix must be set to a string (not an array); use the empty string if you do not want a prefix.
See your settings.php file.');
  }

  _drupal_reset_delete_files();
  drush_log('Deleting the files', 'ok');

  _drupal_reset_drop_database();
  drush_log('Dropping the database', 'ok');
}
