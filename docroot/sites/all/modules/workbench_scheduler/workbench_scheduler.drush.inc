<?php
/**
 * @file
 * Drush utility commands for workbench_scheduler.
 */

/**
 * Implements hook_drush_command().
 */
function workbench_scheduler_drush_command() {
  $items['workbench-scheduler-run'] = array(
    'description' => 'Run workbench scheduler schedules.',
    'callback' => 'workbench_scheduler_drush_run',
    'aliases' => array('wbs-run', 'wbsr'),
  );

  return $items;
}

/**
 * Callback function.
 */
function workbench_scheduler_drush_run() {
  // Call the run function, to process schedules.
  $count = _workbench_scheduler_run();
  drupal_set_message(t('@count Workbench Schedules were processed.', array(
    '@count' => $count,
  )));
}
