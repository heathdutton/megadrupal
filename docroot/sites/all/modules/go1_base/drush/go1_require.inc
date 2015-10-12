<?php

/**
 * @file ./drush/go1_require.inc
 */

use \Drupal\go1_base\Drush\Command\Go1Require as Go1RequireCommand;
use \Drupal\go1_base\Drush\Command\Go1RequireSpyc as Go1RequireSpycCommand;
use \Drupal\go1_base\Drush\Command\Go1Require\DependencyFetcher;

class DrushMakeProject_Go1Require_Library extends \DrushMakeProject_Library {
  /**
   * Override default value of parent.
   */
  protected function generatePath($base = TRUE) {
    return parent::generatePath($base = FALSE);
  }
}

/**
 * Callback for go1_require command.
 */
function drush_go1_require($module = 'all') {
  go1_id(new Go1RequireCommand($module))->execute();
}

/**
 * Implements drush_hook_COMMAND_pre_validate()
 */
function drush_go1_require_pm_enable_pre_validate($module) {
  _drush_go1_check_yml_reader();

  // Module was already processed, no need redo
  if (!module_exists($module)) {
    drush_go1_require($module);
  }
}

# ---------------------
# Hack to download required depedencies
# ---------------------

// YAML reader must be available
if (!function_exists('yaml_parse') && !is_file(DRUPAL_ROOT . '/sites/all/libraries/spyc/Spyc.php')) {
  $name = 'spyc';
  $info = array(
    'type' => 'library',
    'download' => array(
      'type' => 'git',
      'url' => 'https://github.com/mustangostang/spyc.git',
      'revision' => 'eeb9f7c47e1457770d64c7fb1f3d4aabff52de4b',
    ),
  );

  go1_id(new DependencyFetcher($name, $info))->fetch();
}

// Required projects must be available
$fn = 'yaml_parse';
if (!function_exists('yaml_parse')) {
  require_once DRUPAL_ROOT . '/sites/all/libraries/spyc/Spyc.php';
  $fn = 'spyc_load_file';
}

if (function_exists('drupal_get_path')) {
  $projects = drupal_get_path('module', 'go1_base') . '/config/require.yml';
  $projects = $fn($projects);
  unset($projects['spyc']);

  // Backup global working copy option.
  $working_copy = drush_get_option('working-copy');
  foreach ($projects['projects'] as $name => $info) {
    try {
      go1_library($name);
    }
    catch (\Exception $e) {
      if (empty($working_copy) && !empty($info['options']['working-copy'])) {
        // Apply working copy option for each library.
        drush_set_option('working-copy', TRUE);
      }

      go1_id(new DependencyFetcher($name, $info))->fetch();

      // Restore global working copy option.
      if (empty($working_copy) && !empty($info['options']['working-copy'])) {
        drush_set_option('working-copy', $working_copy);
      }
    }
  }
}
