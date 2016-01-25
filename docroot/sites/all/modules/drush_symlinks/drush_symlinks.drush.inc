<?php

/**
 * Implements hook_drush_help().
 */
function drush_symlinks_drush_help($section) {
  switch ($section) {
    case 'meta:drush_symlinks:title':
      return dt('Drush Symlinks');
    case 'meta:drush_symlinks:summary':
      return dt('Creates symlinks using array with link definitions stored in alias file.');
  }
}

/**
 * Implements hook_drush_command().
 */
function drush_symlinks_drush_command() {
  $items = array();
  $items['symlinks'] = array(
    'description' => 'Create symlinks.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(),
    'arguments' => array(
      'target' => 'Target site.',
    ),
    'examples' => array(
      'drush sm @alias' => 'Create symlinks.',
    ),
    'aliases' => array('sm'),
  );
  return $items;
}

/**
 * Implements of drush_hook_COMMAND_validate().
 */
function drush_drush_symlinks_symlinks_validate() {
  if (drush_is_windows()) {
    return drush_set_error('DRUSH_IS_WINDOWS', dt("Symlinks command can't be used on Windows platform."));
  }
}

/**
 * Command callback.
 */
function drush_drush_symlinks_symlinks($target = NULL) {
  if ($target && $alias = drush_sitealias_get_record($target)) {
    // check if there 'drush_symlinks' key
    if (!array_key_exists('drush_symlinks', $alias)) {
      return drush_set_error('DRUSH_NO_ARGUMENTS', dt('Symlinks definitions not found.'));
    }
    foreach ($alias['drush_symlinks'] as $item) {
      // check if link definition is correct
      if (!array_key_exists('target_path', $item) ||
          !array_key_exists('link_path', $item)) {
        return drush_set_error('DRUSH_NO_ARGUMENTS', dt('Symlink definition is incorrect.'));
      }
      // prepare space for new link
      if (is_dir($item['target_path'])) {
        drush_op_system("rm -rf {$item[target_path]}");
      }
      elseif (is_link($item['target_path'])) {
        drush_op_system("unlink {$item[target_path]}");
      }
      // let's do it!
      drush_op_system("ln -s {$item[link_path]} {$item[target_path]}");
      if (is_link($item['target_path'])) {
        drush_log(dt("Symlink to '!symlink' has been created.", array('!symlink' => $item['target_path'])), 'success');
      }
    }
  }
  else {
    return drush_set_error('DRUSH_NO_ARGUMENTS', dt('No valid arguments provided. Example: drush sm @alias'));
  }
}