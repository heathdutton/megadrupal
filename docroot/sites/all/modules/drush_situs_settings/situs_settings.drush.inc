<?php
/**
 * @file
 * Code for the Situs plugin named "Situs Settings".
 */

/**
 * Implments hook_situs_plugin().
 */
function situs_settings_situs_plugin() {
  return array(
    'situs-settings' => array(
      'name' => 'Situs settings.php',
      'description' => 'Situs settings.php deploy',
      'post_build' => 'situs_settings_post_build',
    ),
  );
}

/**
 * Implements hook_drush_help_alter().
 *
 * In Drush 5 we must define the options.
 */
function situs_settings_drush_help_alter(&$command) {
  if ($command['command'] == 'situs-build') {
    $command['options']['settings-file'] = 'The settings.php file (of default site) to copy to the build.';
  }
}
/**
 * Situs post_build() callback.
 *
 * Copies the specified settings.php to the build.
 */
function situs_settings_post_build($build_path) {
  $settingsfile = drush_get_option('settings-file', FALSE);
  if (is_file($settingsfile)) {
    // Copy the settings file to sites subdir folder if specified and to
    // 'default' subdir otherwise.
    if ($sites_subdir = drush_get_option('sites-subdir')) {
      // Create sites subdir folder if it doesn't exists already.
      @mkdir($build_path . '/sites/' . $sites_subdir, 0777, TRUE);
      copy($settingsfile, $build_path . '/sites/' . $sites_subdir . '/settings.php');
    }
    else {
      copy($settingsfile, $build_path . '/sites/default/settings.php');
    }
  }
}
