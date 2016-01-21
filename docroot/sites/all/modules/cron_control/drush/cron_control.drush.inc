<?php

/**
 * @file
 *   drush integration for cron_control.
 *
 * @see cron_control.module
 *
 * @author
 *   Markus Kalkbrenner | bio.logis GmbH
 */

/**
 * Implements hook_drush_command().
 *
 * @return
 *   An associative array describing your command(s).
 */
function cron_control_drush_command() {
  $items = array();

  $items['cc-temp-disable-global'] = array(
    'callback' => 'cron_control_global_disable',
    'description' => dt('Disable all cron jobs on every server temporarily.'),
    'aliases' => array('cc-disgl'),
    'arguments' => array(
      'disable' => '1 to disable, 0 to re-enable',
    ),
    'examples' => array(
      'drush cc-disgl 1' =>
        dt('Disable all cron jobs on every server temporarily.'),
      'drush cc-disgl 0' =>
        dt('Re-enable all cron jobs on every server.'),
    ),
  );

  $items['cc-enable-all-on-server'] = array(
    'callback' => 'cron_control_enable_all_on_server',
    'description' => dt('Enable all cron jobs on a server.'),
    'aliases' => array('cc-enall'),
    'arguments' => array(
      'server' => 'server IP address',
    ),
    'examples' => array(
      'drush cc-enall --server 127.0.0.1' =>
        dt('Enable all cron jobs on server 127.0.0.1.'),
    ),
  );

  $items['cc-disable-all-on-server'] = array(
    'callback' => 'cron_control_disable_all_on_server',
    'description' => dt('Disable all cron jobs on a server.'),
    'aliases' => array('cc-disall'),
    'arguments' => array(
      'server' => 'server IP address',
    ),
    'examples' => array(
      'drush cc-disall --server 127.0.0.1' =>
        dt('Disable all cron jobs on server 127.0.0.1.'),
    ),
  );

  $items['cc-enable-on-server'] = array(
    'callback' => 'cron_control_enable_on_server',
    'description' => dt('Enable a cron job on a server.'),
    'aliases' => array('cc-en'),
    'arguments' => array(
      'server' => 'server IP address',
      'module' => 'module',
    ),
    'examples' => array(
      'drush cc-en --server 127.0.0.1 ---module node' =>
        dt('Enable cron job provided by module node on server 127.0.0.1.'),
    ),
  );

  $items['cc-disable-on-server'] = array(
    'callback' => 'cron_control_disable_on_server',
    'description' => dt('Disable a cron job on a server.'),
    'aliases' => array('cc-dis'),
    'arguments' => array(
      'server' => 'server IP address',
      'module' => 'module',
    ),
    'examples' => array(
      'drush cc-dis --server 127.0.0.1 ---module node' =>
        dt('Disable cron job provided by module node on server 127.0.0.1.'),
    ),
  );

  $items['cc-show-all-configuration'] = array(
    'callback' => 'cron_control_show_all_configuration',
    'description' => dt('Show configuration for all cron jobs on all server.'),
    'aliases' => array('cc-allconf'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function cron_control_drush_help($section) {
  switch ($section) {
    case 'drush:cc-temp-disable-global':
      return dt('Disable all cron jobs on every server temporarily. The cron job configuration per server will not be modified.');
    case 'drush:cc-enable-all-on-server':
      return dt('Modifies the configuration of all cron jobs on a given server. All cron jobs will be enabled on that server.');
    case 'drush:cc-disable-all-on-server':
      return dt('Modifies the configuration of all cron jobs on a given server. All cron jobs will be disabled on that server.');
    case 'drush:cc-enable-on-server':
      return dt('Modifies the configuration of a cron jobs on a given server. The cron job will be enabled on that server.');
    case 'drush:cc-disable-on-server':
      return dt('Modifies the configuration of a cron jobs on a given server. The cron job will be disabled on that server.');
    case 'drush:cc-show-all-configuration':
      return dt('Show configuration of all cron jobs on all servers.');
  }
}


function cron_control_global_disable($disable) {
  variable_set('cron_control_temp_disable_global', (bool) $disable);
}

function cron_control_enable_all_on_server($server_addr) {
  db_update('cron_control_jobs')
    ->fields(array('active' => 1))
    ->condition('server_addr', $server_addr)
    ->execute();
}

function cron_control_disable_all_on_server($server_addr) {
  db_update('cron_control_jobs')
    ->fields(array('active' => 0))
    ->condition('server_addr', $server_addr)
    ->execute();
}

function cron_control_enable_on_server($server_addr, $module) {
  db_update('cron_control_jobs')
    ->fields(array('active' => 1))
    ->condition('server_addr', $server_addr, '=')
    ->condition('module', $module)
    ->execute();
}

function cron_control_disable_on_server($server_addr, $module) {
  db_update('cron_control_jobs')
    ->fields(array('active' => 0))
    ->condition('server_addr', $server_addr, '=')
    ->condition('module', $module)
    ->execute();
}

function cron_control_show_all_configuration() {
  if ($result = db_select('cron_control_jobs', 'ccj')
                  ->fields('ccj')
                  ->orderBy('server_addr', 'DESC')
                  ->execute()) {
    while ($row = $result->fetchObject()) {
      print str_pad($row->server_addr, 15, ' ', STR_PAD_LEFT) . '   ' . str_pad($row->module, 20, ' ', STR_PAD_RIGHT) . '   ' . ($row->active ? dt('enabled') : dt('disabled')) . "\n";
    }
  }
}
