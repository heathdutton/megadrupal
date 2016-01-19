<?php

/**
 * @file
 *   drush integration for semantic_ui_api.
 */

/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function semantic_ui_api_drush_command() {
  $items = array();

  $items['semantic-ui-download'] = array(
    'callback' => 'semantic_ui_api_drush_lib_download',
    'description' => dt('Downloads the required Semantic Ui library'),
    'aliases' => array('suidl'),
    'arguments' => array(
      'path' => dt('Optional. A path to the semantic_ui_api module. If omitted Drush will use the default location.'),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
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
function semantic_ui_api_drush_help($section) {
  switch ($section) {
    case 'drush:semantic-ui-download':
      return dt("Downloads the required Semantic Ui library from "
              . SEMANTIC_UI_DOWNLOAD_URL);
  }
}

/**
 * Example drush command callback.
 *
 * This is where the action takes place.
 *
 * In this function, all of Drupals API is (usually) available, including
 * any functions you have added in your own modules/themes.
 *
 * To print something to the terminal window, use drush_print().
 *
 */
function semantic_ui_api_drush_lib_download() {
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    //we have dependencies on libraries module so no need to check for that
    //TODO: any way to get path for libraries directory?
    //Just in case if it is site specific? e.g. sites/domain.com/libraries ?
    $path = drush_get_context('DRUSH_DRUPAL_ROOT') . '/sites/all/libraries/semantic_ui';
  }
  // Create the path if it does not exist yet. Added substr check for preventing 
  // any wrong attempts or hacks !
  if (substr($path, -11) == 'semantic_ui' && !is_dir($path)) {
    drush_mkdir($path);
  }
  if (is_dir($path . '/dist')) {
    drush_log(dt('Semantic Ui already present at @path. No download required.', array('@path' => $path)), 'ok');
  }
  elseif (drush_op('chdir', $path) &&
      drush_shell_exec('wget ' . SEMANTIC_UI_DOWNLOAD_URL . '  -O semantic_ui.zip') &&
      drush_shell_exec('unzip semantic_ui.zip') &&
      drush_shell_exec('rm -rf semantic_ui.zip') &&
      drush_shell_exec('mv Semantic-UI-master/dist/* .') &&
      drush_shell_exec('rm -rf Semantic-UI-master')) {
    drush_log(dt('The latest Semantic Ui library has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the Semantic Ui library to @path', array('@path' => $path)), 'error');
  }
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_semantic_ui_api_post_pm_enable() {
  $extensions = func_get_args();
  // Deal with comma delimited extension list.
  if (strpos($extensions[0], ',') !== FALSE) {
    $extensions = explode(',', $extensions[0]);
  }

  if (in_array('semantic_ui_api', $extensions) && !drush_get_option('skip')) {
    semantic_ui_api_drush_lib_download();
  }
}
