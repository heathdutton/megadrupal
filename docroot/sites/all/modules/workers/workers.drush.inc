<?php
/**
 * @file
 */

/**
 * Implements hook_drush_command().
 */
function workers_drush_command() {
  return array(
    'workers-list' => array(
      'description' => 'Show list of declared workers.',
      'aliases' => ['wl'],
      'callback' => 'drush_workers_list',
    ),
    'workers-execute' => array(
      'description' => 'Execute worker interactively.',
      'aliases' => ['wx', 'workers-exec'],
      'callback' => 'drush_workers_execute',
    ),
  );
}

function drush_workers_list() {
  foreach (array_keys(workers_info()) as $worker) {
    drush_print($worker);
  }
}

function drush_workers_execute() {
  $args = drush_get_arguments();
  array_shift($args);
  foreach ($args as $name) {
    drush_print("Execute $name");
    workers_execute($name);
  }
}
