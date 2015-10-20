<?php

/**
 * @file
 * Drush integration for the maps_import module.
 */

use Drupal\maps_suite\Log\Logger;
use Drupal\maps_import\Log\Observer\Mail as MailObserver;
use Drupal\maps_suite\Log\Context\Context;
use Drupal\maps_import\Cache\Object\Profile as CacheProfile;

/**
 * Implements hook_drush_command().
 */
function maps_import_drush_command() {
  $items = array();

  $items['maps-import'] = array(
    'description' => 'Process the given operation.',
    'arguments' => array(
      'pid' => 'The MaPS Import profile id.',
    ),
    'options' => array(
      'op' => 'Specify the operations to process (separated by commas)',
      'list' => 'List the available operations',
      'diff' => 'Differential import',
    ),
    'examples' => array(
      'maps-import 1' => 'Process all available operations for the profile with the pid 1.',
      'maps-import 1 --op=library_mapping,objects_fetch' => 'Process the "objects_fetch" operation, then the "library_mapping" operation, in the right order.',
      'maps-import 1 --list' => 'List all the available operations for the profile with the pid 1',
    ),
  );

  $items['maps-profile'] = array(
    'description' => 'Manage the MaPS profiles.',
    'options' => array(
      'list' => 'List the availables profiles.',
    ),
  );

  $items['maps-unlock'] = array(
    'description' => 'Manage the MaPS operation lock.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

/**
 * Command handler. Process the operation.
 *
 * @param int $pid
 *   The MaPS Import profile id.
 */
function drush_maps_import($pid = NULL) {
  if (is_null($pid)) {
    echo "You must specify a profile id as first parameter.\n";
    return FALSE;
  }

  if (drush_get_option('list')) {
    drush_maps_import_list($pid);
    return;
  }

  // Check if there is already operations processing.
  if (variable_get('maps_import_lock', 0)) {
    echo "There is already a running operation.\n";
    return;
  }

  $profile = maps_import_profile_load($pid);
  $operations = maps_import_get_import_operations($profile);

  // Are the operations specified in the parameters?
  if ($ops = drush_get_option('op')) {
    $ops = array_map('trim', explode(',', $ops));

    // Check if all the given operations exist.
    if ($invalid = array_diff($ops, array_keys($operations))) {
      echo 'The following operations are not defined: ' . implode(', ', $invalid) . "\n";
      return FALSE;
    }

    // Update the $operations variables with the specified operations.
    $operations = array_intersect_key($operations, array_flip($ops));
  }

  // Set up lock.
  variable_set('maps_import_lock', 1);

  // Differential call.
  if (drush_get_option('diff')) {
    // Check if we can process the differential call.
    if ($profile->getOptionsItem('differential') &&
      variable_get('maps_import:configuration_full:' . $profile->getPid(), 0) &&
      variable_get('maps_import:objects_full:' . $profile->getPid(), 0)) {
      variable_set('maps_import_differential', 1);
    }
  }
  
  // Create a global log object?
  if (variable_get('maps_import_log_drush', 0) && variable_get('maps_import_log_drush_global_notifications', 0)) {
    $summary_log = Logger::attachLog('drush_summary')->addObserver(new MailObserver());
    $summary_log->setRelatedToken('maps_import_profile', $profile);
  }

  // Process chosen operations.
  $batch_operations = array();
  foreach ($operations as $name => $operation) {
    printf($operation['title'] . "\n");
    $op = new $operation['class']($profile);
    $batch_operations = array_merge($batch_operations, $op->batchOperations());
  }

  $batch = array(
    'operations' => $batch_operations,
    'title' => t('Update for the profile %title', array('%title' => $profile->getTitle())),
    'file' => drupal_get_path('module', 'maps_import') . '/admin/maps_import.admin.inc',
    'finished' => 'maps_import_import_finished',
  );

  batch_set($batch);
  drush_backend_batch_process();

  if (isset($summary_log)) {
    $summary_log
      ->addContext(new Context($operation['title']), 'child')
      ->addMessage(file_create_url(Logger::getLog($name)->getPath()))
      ->moveToParent();
  }

  if (isset($summary_log)) {
    $summary_log
      ->setCurrentOperation(1)
      ->save();
  }

  // Remove the lock.
  variable_set('maps_import_lock', 0);
}

/**
 * List the available operations.
 *
 * @param int $pid
 *   The MaPS Import profile id.
 */
function drush_maps_import_list($pid) {
  $profile = maps_import_profile_load($pid);
  $operations = maps_import_get_import_operations($profile);

  $mask = $mask = "| %20.20s | %20.20s | %100.100s | \n";
  $row_delimiter = str_pad('', 150, '-') . "\n";

  printf($row_delimiter);
  printf($mask, "Name", "Title", "Description");
  printf($row_delimiter);

  foreach ($operations as $name => $operation) {
    printf($mask, $name, $operation['title'], strip_tags($operation['description']));
  }

  printf($row_delimiter);
}

/**
 * Command handler. Profile management.
 */
function drush_maps_import_maps_profile() {
  // Display available profiles.
  if ($list = drush_get_option('list')) {
    $mask = $mask = "| %5.5s | %20.20s | \n";
    $row_delimiter = str_pad('', 32, '-') . "\n";

    printf($row_delimiter);
    printf($mask, "Pid", "Title");
    printf($row_delimiter);

    foreach (CacheProfile::getInstance()->loadAll() as $pid => $profile) {
      printf($mask, $profile->getPid(), $profile->getTitle());
    }

    printf($row_delimiter);
  }
}

/**
 * Command handler. Manage the MaPS operation lock.
 */
function drush_maps_import_maps_unlock() {
  variable_set('maps_import_lock', 0);
  echo "The lock has been succesfully removed.\n";
}
