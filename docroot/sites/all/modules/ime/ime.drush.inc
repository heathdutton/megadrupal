<?php
/**
 * @file
 * Drush integration for the ime module.
 */

/**
 * Implements hook_drush_command().
 */
function ime_drush_command() {
  $items['ime-download'] = array(
    'callback' => 'ime_drush_download',
    'description' => dt('Downloads the required jquery.ime jQuery plugin from Github.'),
    'arguments' => array(
      'path' => dt('Optional. The path to the download folder. If omitted, Drush will use the default location (sites/all/libraries/jquery.ime).'),
    ),
  );

  return $items;
}

/**
 * Callback to the ime-download Drush command.
 */
function ime_drush_download() {
  $args = func_get_args();

  $path = $args[0] ? $args[0] : drush_get_context('DRUSH_DRUPAL_ROOT') . '/sites/all/libraries/jquery.ime';
  $cmd = 'git clone https://github.com/wikimedia/jquery.ime.git ' . $path;
  if (drush_shell_exec($cmd)) {
    drush_log(dt('jqury.ime was download to !path.', array('!path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download jquery.ime to !path. Download jquery.ime library manually from the site https://github.com/wikimedia/jquery.ime. Extract the jquery.ime files inside the "/sites/all/libraries/" directory or check help page of IME module', array('!path' => $path)) . "\n" . dt('Attempted command: !cmd.', array('!cmd' => $cmd)), 'error');
  }
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_ime_post_pm_enable() {
  $modules = func_get_args();

  if (in_array('ime', $modules)) {
    ime_drush_download();
  }
}
