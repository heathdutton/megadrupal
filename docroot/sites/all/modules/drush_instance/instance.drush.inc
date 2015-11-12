<?php

/**
 * @file instance.drush.inc
 */

/**
 * Implements hook_drush_help().
 */
function instance_drush_help($section) {
  switch($section) {
    case 'drush:instance-build':
      return dt("Build a site instance from a site alias");
    break;
  }
}

/**
 * Implements hook_drush_command().
 */
function instance_drush_command() {
  $items = array();
  $items['instance-build'] = array(
    'description' => 'Build a site instance from a site alias',
    'arguments' => array(
      'alias' => 'Site alias for the local build target',
    ),
    'options' => array(
      'rebuild' => 'Build over an existing instance, preserving e.g. sites/files if configured in site alias',
      'revision' => 'Specify a revision of the makefile/composer repository, overriding the site alias',
      'branch' => 'Specify a branch of the makefile/composer repository, overriding the site alias',
      'tag' => 'Specify a tag of the makefile/composer repository, overriding the site alias',
      'templates' => 'Build a comma-separated list of templates, or "all"',
    ),
    'allow-additional-options' => TRUE,

    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

/**
 * Make a unique temporary folder to avoid between-command collisions
 */
function _drush_instance_tmp_unique() {
  // Drush make uses make_tmp() and doesn't play nicely (directory name
  // collisions, and it seems it deletes everything when it's done)
  // So generate our own temporary location
  $site_tmp = drush_find_tmp() . "/drush_instance_tmp_" . time() . '_' . uniqid();
  if (!drush_get_option('no-clean', FALSE)) {
    drush_register_file_for_deletion($site_tmp);
  }
  return $site_tmp;
}

/**
 * Paths-specific validation
 */
function _drush_instance_build_validate_paths(&$alias_info) {
  // Path specifications
  if (!array_key_exists('paths', $alias_info['instance'])) {
    $alias_info['instance']['paths'] = array();
    // If this is a rebuild, we at least try to preserve %files
    if ($alias_info['_is_rebuild']) {
      // Don't have any? Can we at least preserve %files?
      if(array_key_exists('path-aliases', $alias_info)
         && array_key_exists('%files', $alias_info['path-aliases'])) {
        $alias_info['instance']['paths'][$alias_info['path-aliases']['%files']]
          = array('preserve_on_rebuild' => 'keep');
        drush_log("I won't rebuild a site without preserving %files unless you explicitly tell me to, so I've added that to the list now", "ok");
      }
      // Otherwise, just warn the user. This warning can be suppressed
      // by creating an empty array of preserve_on_rebuild files, thus
      // explicitly stating "no preserve_on_rebuild required"
      else {
        drush_log("There are no paths configured to preserve_on_rebuild: any non-Drupal paths INCLUDING FILES will be left behind.", "warning");
      }
    }
  }

  // Replace any path aliases e.g. %files
  foreach($alias_info['instance']['paths'] as $path => $config) {
    if (array_key_exists($path, $alias_info['path-aliases'])) {
      $alias_info['instance']['paths'][$alias_info['path-aliases'][$path]]
        = $config;
      unset($alias_info['instance']['paths'][$path]);
    }
  }

  // And now check those files exist!
  foreach($alias_info['instance']['paths'] as $path => $config) {
    if ($config['preserve_on_rebuild'] && $alias_info['_is_rebuild']
        && !file_exists($alias_info['root'] . "/$path")) {
      drush_log("Cannot preserve on rebuild as does not exist: $path", "warning");
    }
  }
}

/**
 * Helper function to wrap make_download_* for different drush versions
 */
function _drush_instance_make_download_factory($name, $download, $download_location) {
  switch(DRUSH_MAJOR_VERSION) {
    // Drush <5.9 requires only three arguments to make_download_factory().
    case '5':
      if (DRUSH_MINOR_VERSION <= 9) {
        return make_download_factory($name, $download, $download_location);
      }

    // Drush >5.10 6.x 7.x permit "types" of downloading, at the factory level.
    default:
      return make_download_factory($name, 'core', $download, $download_location);
  }
}
