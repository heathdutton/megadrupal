<?php

/**
 * @file
 * Provision/Drush hooks for the provision-git_pull command.
 *
 * These are the hooks that will be executed by the drush_invoke function
 * when asking for a git pull in the site specific directory.
 */

/**
 * Map values of site node into command line arguments.
 */
function drush_hosting_git_pre_hosting_task($task) {
  $task = &drush_get_context('HOSTING_TASK');

  // Only work on selected task types.
  if (!in_array($task->task_type, array('git-pull', 'git-checkout'))) {
    return;
  }

  if (!empty($task->task_args)) {
    if ($task->task_args['reset_hard'] == TRUE) {
      // Pass the option provision_git expects.
      $task->options['reset'] = TRUE;
    }
    if (!empty($task->task_args['checkout_git_ref'])) {
      // Pass the option provision_git expects.
      $task->options['git_ref'] = $task->task_args['checkout_git_ref'];
    }
  }

  // Force the repository path.
  // Otherwise git climbs the tree to find a platform dir under git control.
  $task->options['force_repo_path_as_toplevel'] = TRUE;
}

/**
 * Implements hook_hosting_TASK_OBJECT_context_options().
 */
function hosting_git_hosting_platform_context_options(&$task) {
  // Set some defaults.
  $task->context_options['deploy_from_git'] = FALSE;
  $task->context_options['git_ref'] = '';

  // If we're actually provisioning from Git, populate real data.
  if (!empty($task->ref->git['repo_url'])) {
    $task->context_options['repo_url'] = $task->ref->git['repo_url'];
    $task->context_options['deploy_from_git'] = TRUE;
    $task->context_options['git_ref'] = $task->ref->git['git_ref'];
  }
}

/**
 * Implements hook_hosting_TASK_OBJECT_context_options().
 */
function hosting_git_hosting_site_context_options(&$task) {
  hosting_git_hosting_platform_context_options($task);
}

/**
 * Implements hook_drush_context_import().
 */
function hosting_git_drush_context_import($context, &$node) {
  if (in_array($context->type, array('site', 'platform')) && $context->deploy_from_git) {
    $node->git['repo_url'] = $context->repo_url;
    $node->git['git_ref'] = $context->git_ref;
  }
}


/**
 * Implements hook_post_hosting_TASK_TYPE_task().
 */
function hosting_git_post_hosting_git_checkout_task($task, $data) {
  hosting_git_post_hosting_verify_task($task, $data);

  // Save the node, hosting_site_post_hosting_verify_task does that
  // for a verify task but not for checkout.
  node_save($task->ref);
}

/**
 * Implements hook_post_hosting_TASK_TYPE_task().
 */
function hosting_git_post_hosting_verify_task($task, $data) {
  $node = $task->ref;
  $context = $data['context'];

  $node->git['repo_url'] = $context['repo_url'];
  $node->git['git_ref'] = $context['git_ref'];
}
