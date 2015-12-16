<?php

/**
 * @file
 * Cron Key Change - Drush support.
 */

/**
 * Implements hook_drush_command().
 */
function cronkeychange_drush_command() {
  $commands = array();
  $commands['cronkeychange'] = array(
    'bootstrap' => DRUPAL_BOOTSTRAP_VARIABLES,
    'description' => dt('Generate new cron key.'),
    'callback' => 'drush_cronkeychange',
    'core' => array('7'),
  );
  return $commands;
}

/**
 * Generate new cron key.
 */
function drush_cronkeychange() {
  cronkeychange_generate_submit();
  drush_print(dt('New cron key generated.'));
}
