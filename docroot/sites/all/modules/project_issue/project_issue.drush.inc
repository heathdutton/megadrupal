<?php

/**
 * @file
 * Drush command for processing the queue of issue notification emails.
 *
 * @author Derek Wright (http://drupal.org/user/46549)
 */

function project_issue_drush_command() {
  return array(
    'issue-send-notifications' => array(
      'description' => 'Send all queued issue notification messages.',
      'options' => array(
        'timeout' => array(
          'description' => 'The maximum amount of time to spend sending emails, defaults to 0, which means: keep going until all queued notifications are sent.',
          'example_value' => 60,
          'value' => 'optional',
        ),
      ),
      'drupal dependencies' => array(
        'project_issue',
      ),
    ),
  );
}

function drush_project_issue_issue_send_notifications() {
  // Prevent session information from being saved while queue is running.
  drupal_save_session(FALSE);

  $queue = DrupalQueue::get('project_issue_notifications');
  drush_log(dt('@number items in queue.', array('@number' => $queue->numberOfItems())), 'ok');

  // Force the current user to the configured followup user to ensure
  // consistent permissions on queue runs.
  $original_user = $GLOBALS['user'];
  $GLOBALS['user'] = user_load(variable_get('project_issue_followup_user', 0));

  // Load the include file for issue notification email functions.
  module_load_include('inc', 'project_issue', 'includes/mail');

  $notification_count = 0;
  $timeout = drush_get_option('timeout', 0);
  $end = time() + $timeout;
  while ((($timeout == 0) || time() < $end) && ($item = $queue->claimItem())) {
    drush_log(dt('Preparing notification for nid: @nid cid: @cid', array('@nid' => $item->data['nid'], '@cid' => $item->data['cid'])), 'ok');
    _project_issue_email_notify($item->data['nid'], $item->data['cid']);
    $queue->deleteItem($item);
    $notification_count++;
  }

  // Restore the user.
  $GLOBALS['user'] = $original_user;
  drupal_save_session(TRUE);

  drush_log(dt('Processed @count queued notifications.', array('@count' => $notification_count)), 'ok');
}
