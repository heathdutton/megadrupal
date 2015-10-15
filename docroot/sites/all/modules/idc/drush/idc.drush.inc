<?php
/**
 * @file
 * idc.drush.inc
 * Entry point to tie idc implementations with Drush.
 */

/**
 * Implements hook_drush_command().
 */
function idc_drush_command() {
  // Need to fully bootstrap drupal to fetch all the available IDC commands.
  drush_bootstrap_to_phase(DRUSH_BOOTSTRAP_DRUPAL_FULL);
  $idc_commands = array();

  // Drush will execute this even if the module is not enabled. Just return
  // commands when it *IS* enabled.
  if (function_exists('module_exists')) {
    if (module_exists('idc')) {
      $idc_commands = idc_get_drush_commands();
    }
  }
  return $idc_commands;
}

/**
 * Routes the drush command execution to the appropriate handler class.
 */
function idc_callback() {
   // Find the handler class that should be used for the executed command.
  $current_command = drush_get_command();
  if (isset($current_command['idc']['handler class'])) {
    // Instantiate the command and trigger madness.
    $command_handler = new $current_command['idc']['handler class'];
    $command_handler->feedTheUnicornsLittleTimmy();
  }
}
