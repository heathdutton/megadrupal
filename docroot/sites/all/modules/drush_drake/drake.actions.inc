<?php

/**
 * @file
 * Standard actions for Drake.
 */

/**
 * Returns the build-in standard actions.
 */
function drake_standard_actions() {
  return array(
    'shell' => array(
      'callback' => 'drake_action_shell',
      'multiple' => 'command',
      'parameters' => array(
        'command' => 'Shell command to run.',
        'args' => array(
          'description' => 'Arguments to command (optional).',
          'default' => array(),
        ),
      ),
    ),
    'drush' => array(
      'callback' => 'drake_action_drush',
      'multiple' => 'command',
      'parameters' => array(
        'command' => array(
          'description' => 'The drush command to run.',
        ),
        'args' => array(
          'description' => 'Arguments to Drush command. Keyed values are switches, numerically keyed values are arguments.',
          'default' => array(),
        ),
        'target' => array(
          'description' => 'Site-alias to run the command on (default: @self)',
          'default' => '@self',
        ),
      ),
      'examples' => array(
        'sql-sync' => array(
          'action' => 'drush',
          'command' => 'sql-sync',
          'args' => array('@prod', '@dev', 'no-cache' => TRUE),
        ),
      ),
    ),
    'drake' => array(
      'callback' => 'drake_action_drake',
      'multiple' => 'file',
      'parameters' => array(
        'dir' => array(
          'description' => 'The directory to run drake in.',
          'default' => '',
        ),
        'task' => array(
          'description' => 'The task to run.',
          'default' => '',
        ),
        'target' => array(
          'description' => 'Site-alias to run the command on (default: @self)',
          'default' => '@self',
        ),
      ),
    ),
  );
}

/**
 * Trigger an error in actions.
 *
 * Actions should use this instead of drush_set_error when encountering an
 * unrecoverable error.
 *
 * It provides context to the error message about which action failed, and
 * ensures a common format to error messages.
 */
function drake_action_error($message) {
  $context = &drush_get_context('DRAKE_RUN');
  $message = dt('Action "@action" failed: ', array('@action' => $context['action']['name'])) . $message;
  return drush_set_error('DRAKE_TASK_ERROR', $message);
}

/**
 * Drake action.
 *
 * Execute a shell command.
 */
function drake_action_shell($context) {
  $command = array_merge(array($context['command']), $context['args']);
  for ($x = 1; $x < sizeof($command); $x++) {
    $command[$x] = drush_escapeshellarg($command[$x]);
  }
  $command = join(' ', $command);

  if (!drush_shell_exec_interactive($command)) {
    return drake_action_error(dt('command failed.'));
  }
}

/**
 * Drake action.
 *
 * Execute a drush command.
 */
function drake_action_drush($context) {
  $args = $options = array();
  foreach ($context['args'] as $key => $arg) {
    if (is_int($key)) {
      $args[] = $arg;
    }
    else {
      $options[$key] = $arg;
    }
  }
  $res = drush_invoke_process($context['target'], $context['command'], $args, $options, TRUE);

  if (!$res || $res['error_status'] != 0) {
    return drake_action_error(dt('command failed.'));
  }
}

/**
 * Drake action.
 *
 * Execute a drake file.
 */
function drake_action_drake($context) {
  $args = $options = array();
  if (!empty($context['task'])) {
    $args[] = $context['task'];
  }
  if (!empty($context['arguments'])) {
    $args = array_merge($args, $context['arguments']);
  }
  if (!empty($context['context'])) {
    foreach ($context['context'] as $name => $val) {
      $args[] = $name . '=' . $val;
    }
  }
  $old_dir = NULL;
  if ($context['dir']) {
    $old_dir = getcwd();
    chdir($context['dir']);
  }

  $res = drush_invoke_process($context['target'], 'drake', $args, $options, TRUE);
  if ($old_dir) {
    chdir($old_dir);
  }

  if (!$res || $res['error_status'] != 0) {
    return drake_action_error(dt('drake failed.'));
  }
}
