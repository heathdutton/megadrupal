<?php

/**
 * @file
 *
 * Drush integration for elasticsearch.
 */

/**
 * Implements hook_drush_command().
 */
function elasticsearch_drush_command() {
  $items['download-elastica'] = array(
    'description' => dt("Downloads the Elastica PHP Library."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'callback' => 'drush_download_elastica',
  );
  return $items;
}

/**
 * Command to download the Elastica PHP LIbrary.
 */
function drush_download_elastica() {
  $path = 'sites/all/libraries';

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  $path .= '/Elastica';

  if (is_dir($path)) {
    drush_log('Elastica already present. No download required.', 'ok');
  }
  elseif (drush_shell_cd_and_exec(dirname($path), 'git clone https://github.com/ruflin/Elastica.git')) {
    drush_log(dt('Elastica has been cloned via git to @path.', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to clone Elastica to @path.', array('@path' => $path)), 'error');
  }
}

/**
* Implements drush_MODULE_post_COMMAND().
 */
function drush_elasticsearch_post_pm_enable() {
  $modules = func_get_args();
  if (in_array('elasticsearch', $modules)) {
    drush_download_elastica();
  }
}
