<?php
/**
 * @file
 * Drush integration.
 */

/**
 * Implements hook_drush_command().
 */
function git_rules_drush_command() {
  $items = array();

  // Execute queued commands.
  $items['git-rules-exec'] = array(
    'callback' => 'git_rules_exec',
    'description' => 'Execute all queued commands.',
  );

  // Register an update to a local repo.
  $items['git-rules-post-receive'] = array(
    'callback' => 'git_rules_post_receive',
    'description' => 'Register a post receive in a repository. Can be used by local repositories to invoke the event.',
    'arguments' => array('repository_path', 'repository_name'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function git_rules_drush_help($section) {
  switch ($section) {
    case 'drush:git-rules-exec':
      return dt("Execute git rules.");
  }
}

/**
 *
 */
function git_rules_post_receive($repository_path, $repository_name) {
  // Trigger the git post receive rule
  rules_invoke_event('git_rules_git_post_receive', array(
    'repository' => $repository_name,
    'repository_path' => $repository_path,
  ));
  $handle = fopen('php://stdin', 'r');
  while (!feof($handle)) {
    $input = fgets($handle);
    list($rev_old, $rev_new, $ref) = explode(' ', $input);
    // Get some info about the commit.
    exec('git --git-dir=' . $repository_path . ' --bare show ' . $rev_new, $output);

    // The second line shows information about the author, which we need
    $author_info = explode(' <', drupal_substr($output[1], drupal_strlen('Author: ')));
    $author = $author_info[0];
    $email = drupal_substr($author_info[1], 0, drupal_strlen($author_info[1])-1);

    // We also need the date.
    $date = drupal_substr($output[2], drupal_strlen('Date: '));
    $message = $output[4];
    $date = strtotime($date);
    if (!empty($rev_new) && !empty($date)) {
      // All right, we're set.
      rules_invoke_event('git_rules_git_post_receive_commit', array(
      'id' => $rev_new,
      'author_email' => $email,
      'author_name' => $author,
      'message' => $message,
      'timestamp' => $date,
      'repository' => $repository_name,
      'repository_path' => $repository_path,
      ));
    }
  }
}

/**
 * Execute all saved commands.
 */
function git_rules_exec() {
  $result = db_query('SELECT * FROM {git_rules_command_queue} WHERE exec_status = 0');
  while ($cmd = db_fetch_object($result)) {
    $status = 1;
    exec($cmd->command, $output, $exec_result);
    if ($exec_result !== 0) {
      watchdog(WATCHDOG_ERROR, 'Something went wrong while executing command @command', array(
        '@command' => $cmd->command
      ));
      $status = 2;
    }
    db_query('UPDATE {git_rules_command_queue} SET exec_status = %d WHERE qid = %d', $status, $cmd->qid);
  }
}
