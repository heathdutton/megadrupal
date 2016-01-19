<?php

/**
 * @file
 *   MAMP commands for Drush.
 */


/**
 * Implements hook_drush_init().
 */
function mamp_drush_init() {
  $mamp_dir = '/Applications/MAMP/';
  if (file_exists($mamp_dir) && is_dir($mamp_dir)) {
    define('MAMP_DIRECTORY', $mamp_dir);
    define('MAMP_BIN_DIRECTORY', MAMP_DIRECTORY.'bin/');
    define('MAMP_HTDOCS_DIRECTORY', MAMP_DIRECTORY.'htdocs/');
  }
}


/**
* Implements hook_drush_command().
*
* @See drush_parse_command() for a list of recognized keys.
*
* @return
*   An associative array describing each command.
*/
function mamp_drush_command() {
  $items = array();

  $items['mamp'] = array(
    'description' => "Control MAMP using Drush.",
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'restart' => 'Restart MAMP services',
      'start'   => 'Start MAMP services',
      'stop'    => 'Stop MAMP services',
    ),
    'options' => array(
      'apache' => 'Control only the Apache service.',
      'mysql'  => 'Control only the MySQL service.',
    ),
    'examples' => array(
      'drush mamp restart'          => 'Restart both Apache & MySQL services.',
      'drush mamp start'            => 'Start both Apache & MySQL services.',
      'drush mamp stop'             => 'Stop both Apache & MySQL services.',
      'drush mamp --apache=start'   => 'Start only Apache.',
      'drush mamp --mysql=stop'     => 'Stop only MySQL.',
      'drush mamp --memcache=start' => 'Custom shell scripts in /MAMP/bin folder can be triggered, for example `startMemcache.sh`, `stopMemcache.sh`.',
    ),
  );

  return $items;
}


/**
 * Implements hook_drush_help().
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function mamp_drush_help($section) {
  switch ($section) {
    case 'drush:mamp':
      return dt("Control MAMP via Drush; start, stop, or restart Apache/MySQL services.");
  }
}


/**
 * Implements drush_COMMANDNAME().
 */
function drush_mamp() {
  // All services
  $arguments = drush_get_arguments();
  if (in_array($arguments[1], array('restart', 'start', 'stop'))) {
    mamp_service_control('', $arguments[1]);
    return;
  }
  
  // Specific services
  $service_commands = array();
  static $services = array('apache', 'mysql');
  foreach ($services as $service) {
    $service_command = drush_get_option($service);
    if ($service_command) {
      $service_commands[$service] = $service_command;
    }
  }
  // Run MAMP commands
  foreach ($service_commands as $service => $command) {
    mamp_service_control($service, $command);
  }
}


/**
 * Implements drush_COMMANDNAME_validate().
 */
function drush_mamp_validate() {
  // PHP Safe Mode
  if (getenv('safe_mode')) {
    return drush_set_error('MAMP_ERROR_PHP_SAFE_MODE', dt('PHP Safe Mode is enabled. Cannot execute MAMP shell commands via Drush with Safe Mode on.'));
  }
  
  // Check for MAMP
  if (!defined('MAMP_DIRECTORY')) {
    return drush_set_error('MAMP_ERROR_INSTALL', dt('Make sure MAMP is installed.'));
  }
}


/**
 * Return schell script for a service command.
 * @param String $service
 * @param String $command
 */
function mamp_service_command_script($service, $command, $full_path = FALSE) {
  $script = strtolower($command) . ucfirst($service) .'.sh';
  if (!$full_path) {
    return $script;
  }
  else {
    return MAMP_BIN_DIRECTORY . $script;
  }
}


/**
 * Execute MAMP shell command.
 * @param Array $commands
 */
function mamp_service_command_script_execute($service, $command, $script) {
  if (file_exists($script)) {
    if (drush_shell_exec($script)) {
      drush_log(dt('MAMP !service !command', array(
        '!service' => $service,
        '!command' => $command,
        )), 
        'success'
      );
    }
    else {
      drush_set_error('MAMP_ERROR_COMMAND_EXECUTE', dt('Failed to !command !service. Make sure MAMP is running.', array(
        '!command' => $command,
        '!service' => ucfirst($service),
        )
      ));
    }
  }
}


/**
 * Control a MAMP service.
 * @param Array $service
 */
function mamp_service_control($service, $command) {
  if ($command == 'restart') {
    mamp_service_control($service, 'stop');
    mamp_service_control($service, 'start');
    return;
  }
  
  $script = mamp_service_command_script($service, $command, TRUE);
  mamp_service_command_script_execute($service, $command, $script);
}
