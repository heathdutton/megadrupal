<?php

/**
 * Implementation of hook_post_hosting_TASK_TYPE_task() for test tasks.
 */
function hosting_drulenium_post_hosting_drulenium_pre_code_deploy_task($task, $data) {
  // Get test results
  $results = "Results from drulenium hosting_drulenium_post_hosting_drulenium_pre_code_deploy_task .";
  // Save test run results as a task arg
  $task->task_args['test_results'] = $results;
  node_save($task);
}

/**
 * @param $task
 * @param $data
 */
function hosting_drulenium_hosting_drulenium_pre_code_deploy_task_rollback($task, $data) {
  hosting_drulenium_post_hosting_drulenium_pre_code_deploy_task($task, $data);
}

/**
 * Implementation of hook_post_hosting_TASK_TYPE_task() for test tasks.
 */
function hosting_drulenium_post_hosting_drulenium_post_code_deploy_task($task, $data) {
  // Get test results
  $results = "Results from drulenium hosting_drulenium_post_hosting_drulenium_post_code_deploy_task .";
  // Save test run results as a task arg
  $task->task_args['test_results'] = $results;
  node_save($task);
}

/**
 * @param $task
 * @param $data
 */
function hosting_drulenium_hosting_drulenium_post_code_deploy_task_rollback($task, $data) {
  hosting_drulenium_post_hosting_drulenium_post_code_deploy_task($task, $data);
}

/**
 * Implementation of hook_post_hosting_TASK_TYPE_task() for devshop deploy tasks.
 *
 * Saves a deploy task if the environment is set to do so.
 */
function hosting_drulenium_post_hosting_devshop_deploy_task($task, $data) {
  hosting_add_task($task->ref->nid, 'dr_post_code_deploy');
}

/**
 * Implements drush_HOOK_pre_COMMAND()
 *
 * This runs for each task during the command
 *    drush @hostmaster hosting-tasks
 *
 * NOTE: This ONLY runs when being called from a hostmaster task.
 * This hook should ONLY be used to pass options from a hostmaster task form to
 * the $task object, or if you don't need this functionality from the command
 * line.
 */
function drush_hosting_drulenium_pre_hosting_task($task) {
  drush_log(dt("[Drulenium] in drush_hosting_drulenium_pre_hosting_task"));
  $task = &drush_get_context('HOSTING_TASK');

  if ($task->ref->type == 'site' && $task->task_type == 'dr_run_compare') {
    $task->args[] = '@' . ltrim($task->task_args['reference'], '@');
  }
}
