<?php
/**
 * @file
 * Git Clone API.
 */

/**
 * Allow modules to execute code before a GitClone is about to be dequeued.
 *
 * @param \Drupal\GitClone\GitClone $clone
 *   The GitClone instance.
 */
function hook_git_clone_pre_dequeue(\Drupal\GitClone\GitClone $clone) {
  // Execute your module code here.
}

/**
 * Allow modules to execute code after a GitClone has successfully dequeued.
 *
 * @param \Drupal\GitClone\GitClone $clone
 *   The GitClone instance.
 */
function hook_git_clone_post_dequeue(\Drupal\GitClone\GitClone $clone) {
  // Execute your module code here.
}

/**
 * Allows modules to alter the arguments passed to a command before execution.
 *
 * Where COMMAND is like: 'checkout', 'fetch', 'merge', 'reset', etc.
 *
 * @param array $args
 *   An array of arguments, passed by reference.
 *   Toggle determining whether or not the command will output content.
 * @param \Drupal\GitClone\GitClone $clone
 *   The current GitClone entity.
 * @param array $context
 *   An associative array containing the following:
 *   - output: (bool) Status of whether or not the method that invoked the
 *     command expects output. This usually is almost always FALSE.
 */
function hook_git_clone_pre_COMMAND_alter(array &$args, \Drupal\GitClone\GitClone $clone, array $context) {
  $args[] = '--force';
}

/**
 * Allows modules to alter the arguments passed to a command before execution.
 *
 * Where COMMAND is like: 'checkout', 'fetch', 'merge', 'reset', etc.
 *
 * @param array $args
 *   An array of arguments, passed by reference.
 * @param \Drupal\GitClone\GitClone $clone
 *   The current GitClone entity.
 * @param array $context
 *   An associative array containing the following:
 *   - output: (bool) Status of whether or not the method that invoked the
 *     command expects output. This usually is almost always FALSE.
 */
function hook_git_clone_post_COMMAND_alter(array &$args, \Drupal\GitClone\GitClone $clone, array $context) {
  $args[] = '--force';
}
