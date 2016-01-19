<?php

/**
 * @file
 * Drush commands.
 */

/**
 * Implements hook_drush_command().
 */
function dcq_drush_command() {
  $items = array();

  $items['dcq-install'] = array(
    'description' => 'Install DCQ (Drupal Code Quality) git hooks.',
    'aliases' => array('dcq'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(
      'force' => array(
        'short-form' => 'f',
        'description' => 'Overwrite existing pre-commit hook files.',
      ),
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function dcq_drush_help($section) {
  switch ($section) {
    case 'meta:dcq:title':
      return dt('Drupal Code Quality commands');

    case 'drush:dcq-install':
      return dt("Install the dcq (Drupal Code Quality) hooks in a git project. This command can be run from within any git directory.");
  }
}

/**
 * Drush command; Setup DCQ git commit hooks.
 */
function drush_dcq_install() {
  $source_dir = dirname(realpath(__FILE__));
  $dest_dir = exec('git rev-parse --show-toplevel');

  if (empty($dest_dir)) {
    return drush_set_error('dcq-no-git', 'No git directory was found.');
  }

  if ($dest_dir) {
    $dest_dir .= '/.git/hooks';
    $files = array('pre-commit', 'pre-commit_dcq');

    // Check for existing pre-commit hook files.
    $file_exists = FALSE;
    foreach ($files as $file) {
      $dest_file = "{$dest_dir}/{$file}";
      if (file_exists($dest_file)) {
        drush_log(dt('File @file already exists.', array('@file' => $dest_file)), 'warning');
        $file_exists = TRUE;
      }
    }

    // Exit or notify the user if there are existing pre-commit hook files.
    if ($file_exists) {
      if (!drush_get_option('force')) {
        drush_log(dt('Aborting. Use --force to overwrite existing pre-commit hooks.'), 'error');
        return FALSE;
      }
      else {
        drush_log(dt('Overwriting existing pre-commit hooks.'), 'warning');
      }
    }

    // Copy the files from dcq into the .git/hooks directory.
    foreach ($files as $file) {
      copy("{$source_dir}/{$file}", "{$dest_dir}/{$file}");
      chmod("{$dest_dir}/{$file}", 0755);
    }
    drush_log(dt('Installed pre-commit hooks in @dir', array('@dir' => $dest_dir)), 'success');
  }
}
