<?php

/**
 * @file ./drush/go.require.inc
 *
 * Functions/Classes for drush go-require command.
 *
 * @author  Sang Le
 * @author  Andy Truong <thehongtt@gmail.com>
 *
 * - If not found .gorequire in ./sites/example.com/.gorequire, throw exception.
 * - If .gorequire is not in valid format, thow exception.
 * - Parse to get dependencies — modules/themes/libraries
 * - Install dependency:
 *   - If the resource is already available, no need to download again.
 *   - If an resource is available with different version to expected,
 *     - If the alternative directory is available, download dep. there
 *     - Else, confirm user to override it or not. Allow user go to next dep.
 *         without resolving issue.
 */

use Drupal\go\Drush\Command\GoRequire\ParseInfo;

/**
 * Callback for drush go-require command.
 *
 */
function drush_go_require() {
  $make_file   = drush_get_option('file', '.gorequire');
  $gitignore   = '.gitignore';
  $site_root   = drush_get_context(DRUSH_DRUPAL_SITE_ROOT);
  $build_path  = drush_get_context(DRUSH_DRUPAL_SITE_ROOT);
  $drupal_root = drush_get_context(DRUSH_DRUPAL_ROOT);
  $full_info   = $info = go_require_get_projects_info($drupal_root, $site_root, $make_file);

  // drush_print_r($info);

  go_require_exlude_already_available_projects($drupal_root, $site_root, $info);
  go_require_exlude_already_available_libraries($drupal_root, $site_root, $info);

  if (!empty($info['projects'])) {
    _drush_go_require_projects($make_file, $gitignore, $site_root, $build_path, $drupal_root, $info['projects']);
  }

  if (!empty($info['libraries'])) {
    _drush_go_require_libraries($make_file, $gitignore, $current_dir, $build_path, $drupal_root, $info['libraries']);
  }
}

/**
 * Parse projects info from .gorequire file.
 *
 * @param  string $drupal_root Path to Drupal root dir
 * @param  string $site_root   Path to Site Root dir
 * @param  string $make_file   Path to
 * @return array               Parsed projects info.
 */
function go_require_get_projects_info($drupal_root, $site_root, $make_file) {
  // Drush >=5, using native function make_parse_info_file
  $make_parse_info_file = 'make_parse_info_file';
  $make_validate_info_file = 'make_validate_info_file';
  $make_version = MAKE_API;
  if (version_compare(DRUSH_MAJOR_VERSION, 5) < 0) {
    // We are in drush 4, using drush_make_parse_info_file
    $make_parse_info_file = 'drush_make_parse_info_file';
    $make_validate_info_file = 'drush_make_validate_info_file';
    $make_version = DRUSH_MAKE_API;
  }

  drush_print_r("{$site_root}/{$make_file}");

  if ($info = $make_parse_info_file("{$site_root}/{$make_file}")) {
    $info += array('core' => '7.x', 'api' => $make_version);
    if ($make_validate_info_file($info)) {
      return $info;
    }
  }

  return FALSE;
}

/**
 * Exclude already available Drupal projects
 */
function go_require_exlude_already_available_projects($drupal_root, $site_root, &$info) {
  $extension_info = version_compare(DRUSH_MAJOR_VERSION, 5) < 0 ? drush_pm_get_extensions() : drush_get_extensions();
  uasort($extension_info, '_drush_pm_sort_extensions');
  foreach ($extension_info as $key => $extension) {
    if (isset($info['projects'][$key])) {
      if (version_compare("7.x-{$info['projects'][$key]['version']}", $extension->info['version']) == 0) {
        unset($info['projects'][$key]);
      }
      else {
        $info['projects'][$key]['confict'] = TRUE;
      }
    }
  }
}

/**
 * Exclude already available libraries
 */
function go_require_exlude_already_available_libraries($drupal_root, $site_root, &$info) {
  $libraries = array();
  foreach (array($drupal_root . '/sites/all/libraries', $drupal_root . '/sites/' . $site_root . '/libraries') as $scan_dir) {
    if (is_dir($scan_dir)) {
      $libraries = array_merge(array_diff(scandir($scan_dir), array('..', '.')), $libraries);
    }
  }

  foreach ($libraries as $library) {
    unset($info['libraries'][$key]);
  }
}

/**
 * [_drush_go_require description]
 *
 * @param  string $make_file   Path to .gorequire
 * @param  string $gitignore   Path to .gitignore
 * @param  string $current_dir [description]
 * @param  string $build_path  [description]
 * @param  string $drupal_root Drupal Root directory
 */
function _drush_go_require_projects($make_file, $gitignore, $current_dir, $build_path, $drupal_root, $projects) {
  $commands = drush_get_commands();
  $dl_cmd = $commands['pm-download'];
  drush_set_option('no-core', 1);

  foreach ($projects as $name => $project) {
    if ($name !== 'drupal') {
      // drush_print_r("drush pm-download {$name}-{$project['version']}");
      $release = "{$name}-{$info['version']}";
      $dl_cmd['arguments'] = array($release);
      drush_dispatch($dl_cmd, array($release));
    }
  }
}

/**
 * [_drush_go_require_libraries description]
 * @param  [type] $make_file   [description]
 * @param  [type] $gitignore   [description]
 * @param  [type] $current_dir [description]
 * @param  [type] $build_path  [description]
 * @param  [type] $drupal_root [description]
 * @param  [type] $libraries   [description]
 * @return [type]              [description]
 */
function _drush_go_require_libraries($make_file, $gitignore, $current_dir, $build_path, $drupal_root, $libraries) {
  foreach ($libraries as $name => $library) {
    $library += array(
      'type' => 'library',
      'destination' => 'libraries',
      'name' => $name,
      'build_path' => $drupal_root,
      'make_directory' => $drupal_root,
      'contrib_destination' => 'sites/all',
      'directory_name' => $name,
    );

    $destination = drush_get_option('destination', 'sites/all/libraries');
    $class = DrushMakeProject::getInstance('GoLibrary', $library);
    $class->make();
  }
}
