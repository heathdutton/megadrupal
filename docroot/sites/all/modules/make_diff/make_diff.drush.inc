<?php

/**
 * @file
 * Allow diff'ing Drush makefiles.
 */

/**
 * Implements hook_drush_command().
 */
function make_diff_drush_command() {
  $items['make-diff'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => 'Show the differences between two makefiles.',
    'arguments' => array(
      'OldMakefile' => 'An outdated Drush makefile.',
      'NewMakefile' => 'An updated Drush makefile.',
    ),
    'options' => array(
      'list' => 'Output a simple list of projects.',
    ),
    'required-arguments' => TRUE,
    'aliases' => array('diff'),
  );

  return $items;
}

/**
 * Command callback. Diff two makefiles.
 */
function drush_make_diff($old_makefile, $new_makefile) {
  // Load, parse and validate our makefiles;
  $old_info = make_parse_info_file($old_makefile);
  $new_info = make_parse_info_file($new_makefile);
  if (!make_validate_info_file($old_info) || !make_validate_info_file($new_info)) {
    return FALSE;
  }

  // Merge our lists
  $list = array_keys(array_merge($old_info['projects'], $new_info['projects']));
  foreach ($list as $key => $name) {
    if (isset($old_info['projects'][$name])) {
      $list[$name]['old'] = $old_info['projects'][$name];
    }
    if (isset($new_info['projects'][$name])) {
      $list[$name]['new'] = $new_info['projects'][$name];
    }
    unset($list[$key]);
  }

  // Check for version and other differences.
  foreach ($list as $name => $info) {
    $list[$name]['notices'] = array();

    // Project add or removed
    if (isset($info['new']) && !isset($info['old'])) {
      $list[$name]['notices'][] = 'new project added';
    }
    elseif (!isset($info['new']) && isset($info['old'])) {
      $list[$name]['notices'][] = 'WARNING! project removed';
    }
    else {
      // Version changes
      if (version_compare($info['new']['version'], $info['old']['version'], '>')) {
        if (substr($info['new']['version'], 0, 1) > substr($info['old']['version'], 0, 1)) {
          $list[$name]['notices']['version'] = 'WARNING! Major version changed';
        }
        else {
          $list[$name]['notices']['version'] = 'version upgraded';
        }
      }
      elseif (version_compare($info['old']['version'], $info['new']['version'], '>')) {
        $list[$name]['notices']['version'] = 'WARNING! version downgraded';
      }
      // Patch changes
      if (isset($info['new']['patch']) || isset($info['old']['patch'])) {
        if ($info['new']['patch'] !== $info['old']['patch']) {
          $list[$name]['notices']['patch'] = 'patches changed';
        }
      }
      // VCS changes
      if (isset($info['new']['download']) || isset($info['old']['download'])) {
        if ($info['new']['download'] !== $info['old']['download']) {
          $list[$name]['notices']['download'] = 'download changed';
        }
      }
    }
  }

  // Filter down to just projects, versions and notices
  $diff = array('header' => array('Project','Old version','New version','Notices'));
  foreach ($list as $name => $info) {
    if (empty($info['notices'])) {
      continue;
    }
    $diff[$name]['name'] = $name;
    $diff[$name]['old'] = $info['old']['version'];
    $diff[$name]['new'] = $info['new']['version'];
    $diff[$name]['notices'] = implode($info['notices'],', ') . '.';
  }

  if (drush_get_option('list', TRUE)) {
    unset($diff['header']);
    $new = array();
    foreach ($diff as $name => $info) {
      if (isset($info['new'])) {
        $new[] = $name . '-' .  $new_info['core'] . '-' . $info['new'];
      }
    }
    return implode($new, ' ');
  }
  else {
    return drush_format_table($diff, TRUE);
  }
}
