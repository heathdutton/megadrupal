<?php

/**
 * @file Provides the basic developer documentation for working with preview
 * sync.
 *
 * Preview sync provides a way to alter the default tasks to either inject your
 * own tasks, or to remove default tasks.
 *
 * @see preview_sync.module
 */

/**
 * Allows modules to add and remove items into the task list.
 *
 * @see preview_sync_tasks().
 */
function hook_preview_sync_tasks_alter(&$tasks) {
  list($from, $to) = preview_sync_get_aliases();
  list($from_key) = array_keys($from);
  $from_alias = 'preview.' . $from_key;
  list($to_key) = array_keys($to);
  $to_alias = 'preview.' . $to_key;

  // Example 1 - removal of an existing task.
  unset($tasks['cache_clear']);

  // Example 2 - insertion of feature revert before the `drush cc all` (before
  // the 6th item by default).
  $task['feature_revert'] = array(
    'title' => t('Feature revert'),
    'command' => 'fr',
    'site' => "@$to_alias",
  );

  // I wish this was easier.
  $tasks = array_slice($tasks, 0, 5, TRUE) + $task +  array_slice($tasks, 5, count($tasks) - 5, TRUE);
}
