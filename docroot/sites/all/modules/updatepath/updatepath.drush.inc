<?php
/**
 * @file
 * Runs a set of steps to update a database to be in line with code.
 */

/**
 * Implements hook_drush_command().
 */
function updatepath_drush_command() {
  $items = array();
  $items['updatepath'] = array(
    'description' => 'Runs a set of common steps to update a database to be in line with code.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'examples' => array(
      'drush --uri=example.com updatepath' => 'Runs the update path in the current Drupal project.',
      'drush @example.com updatepath' => 'Runs the update path in the Drupal project referenced by @example.com.',
    ),
  );
  return $items;
}

/**
 * Implements drush_hook_command().
 */
function drush_updatepath() {
  // Registry rebuild.
  $return = drush_invoke_process('@self', 'registry-rebuild', array(), array('no-cache-clear' => TRUE));
  if ($return['error_status']) {
    return drush_set_error('UPDATEPATH_RR', dt('registry-rebuild failed.'));
  }

  // Database updates.
  $return = drush_invoke_process('@self', 'updatedb', array(), array('yes' => true));
  if ($return['error_status']) {
    return drush_set_error('UPDATEPATH_UPDB', dt('updatedb failed.'));
  }

  // Clear Drush cache (sometimes needed before reverting Features components).
  $return = drush_invoke_process('@self', 'cache-clear', array('type' => 'drush'));
  if ($return['error_status']) {
    return drush_set_error('UPDATEPATH_CC_DRUSH', dt('cache-clear drush failed.'));
  }

  // Revert all Features components.
  $return = drush_invoke_process('@self', 'features-revert-all', array(), array(
    'yes' => TRUE,
  ));
  if ($return['error_status']) {
    return drush_set_error('UPDATEPATH_FRA', dt('features-revert-all failed.'));
  }

  // Clear all caches.
  $return = drush_invoke_process('@self', 'cache-clear', array('type' => 'all'));
  if ($return['error_status']) {
    return drush_set_error('UPDATEPATH_CC_ALL', dt('cache-clear all failed.'));
  }

  // Run cron.
  $return = drush_invoke_process('@self', 'cron', array('type' => 'all'));
  if ($return['error_status']) {
    return drush_set_error('UPDATEPATH_CRON', dt('cron failed.'));
  }
}
