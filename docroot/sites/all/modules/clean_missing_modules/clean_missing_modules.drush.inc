<?php

/**
 * @file
 *  Drush command that finds and removes missing modules.
 */

/**
 * Implements hook_drush_command().
 */
function clean_missing_modules_drush_command() {
  $items = array();
  $items['clean-modules'] = array(
    'callback' => 'clean_missing_modules',
    'description' => dt('Find and remove enabled modules that do not exist from the database.'),
  );
  return $items;
}

/**
 * Find and remove missing modules from the database.
 */
function clean_missing_modules() {
  $modules = db_query("SELECT name, filename, info FROM {system} WHERE type = 'module' AND status = 1 ORDER BY filename");

  $to_disable = array();

  foreach ($modules as $module) {
    $info = unserialize($module->info);

    $filepath = DRUPAL_ROOT . '/' . $module->filename;

    if (!file_exists($filepath)) {
      drush_print($info['name']);
      $to_disable[] = $module->name;
    }
  }
  if ($to_disable) {
   if (!drush_confirm(dt('Are you sure you want to disable the modules above?'))) {
     return drush_user_abort();
   }
    db_update('system')
      ->fields(array('status' => 0))
      ->condition('name', $to_disable)
      ->condition('type', 'module')
      ->execute();
  }
  else {
    drush_print(dt('Nothing to disable.'));
  }
}
