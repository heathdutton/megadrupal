<?php
/**
 * @file
 * Drush commands for Ultimate Cron!
 */

/**
 * Implements hook_drush_command().
 */
function mob_queue_drush_command() {
  $items = array();

  $items['mob-exe-queue'] = array(
    'description' => "Execute mob_queue queued tasks.",
    'examples' => array(
      'drush mob-queue' => 'Go a sprint to finish the tasks in the queues for mob-queue',
    ),
    'arguments' => array(
      'time' => dt('Total execution time for this command.'),
    ),
    'options' => array(
      'no-reset-expired' => "Do not reset expired items from the queue table.",
    ),
    'aliases' => array('meq'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function mob_queue_drush_help($section) {
  switch ($section) {
    case 'drush:mob-queue':
      return dt("Execute queue tasks.");
  }
}

/**
 * Run the queued job.
 */
function drush_mob_queue_mob_exe_queue($time = 900) {
  // Allow execution to continue even if the request gets canceled.
  @ignore_user_abort(TRUE);

  if (!drush_get_option('no-reset-expired', FALSE)) {
    // Reset expired items in the default queue implementation table.
    $updated = db_update('queue')
      ->fields(array(
        'expire' => 0,
      ))
      ->condition('expire', 0, '<>')
      ->condition('expire', REQUEST_TIME, '<')
      ->execute();
    drush_log(dt('!updated expired items reset.', array('!updated' => $updated)));
  }

  // Prevent session information from being saved while cron is running.
  $original_session_saving = drupal_save_session();
  drupal_save_session(FALSE);

  // Force the current user to anonymous to ensure consistent permissions on
  // cron runs.
  $original_user = $GLOBALS['user'];
  $GLOBALS['user'] = drupal_anonymous_user();

  // Try to allocate enough time to run all the hook_cron implementations.
  drupal_set_time_limit($time);

  $return = FALSE;
  // Grab the defined cron queues.
  $queues = module_invoke_all('cron_queue_info');
  foreach ($queues as $name => $queue) {
    if (variable_get('mob_queue_' . $name, 0)) {
      $queues[$name]['mob_queue'] = TRUE;
    }
  }

  // Make sure every queue exists. There is no harm in trying to recreate an
  // existing queue.
  foreach ($queues as $queue_name => $info) {
    DrupalQueue::get($queue_name)->createQueue();
  }

  foreach ($queues as $queue_name => $info) {
    if (empty($info['mob_queue'])) {
      continue;
    }
    $function = $info['worker callback'];
    $end = time() + (isset($info['time']) ? $info['time'] : $time);
    $leasetime = (isset($info['leasetime']) ? $info['leasetime'] : 30);
    $queue = DrupalQueue::get($queue_name);
    while (time() < $end && ($item = $queue->claimItem($leasetime))) {
      drush_log(dt("Processing '!queue-name' queue, item id: !item-id.",
        array('!item-id' => $item->item_id, '!queue-name' => $queue_name)));
      try {
        $function($item->data);
        $queue->deleteItem($item);
      }
      catch (Exception $e) {
        // In case of exception log it and leave the item in the queue
        // to be processed again later.
        watchdog_exception('mob_queue', $e);
      }
    }
  }
  // Restore the user.
  $GLOBALS['user'] = $original_user;
  drupal_save_session($original_session_saving);
  $return = TRUE;

  return $return;
}
