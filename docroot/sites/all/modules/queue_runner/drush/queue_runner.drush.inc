<?php
/**
 * @file
 * Drush commands for managing the processing of queues.
 */

/**
 * Implements hook_drush_command().
 */
function queue_runner_drush_command() {
  $commands = array();
  $commands['queue-runner-process-queue'] = array(
    'examples' => array(
      'drush qr-pq' => 'Process items in the queue',
    ),
    'description' => 'Run a processing job for the queue.',
    'options' => array(
      'timeout' => 'Maximal run time in seconds of processing (defaults to value of variable queue_runner_time)',
      'pause' => 'Time in seconds between claiming items (defaults to value of variable queue_runner_pause_between_tasks)',
      'lock-name' => 'Different lock names allow parallel queue processing',
      'lock-timeout' => 'How long in seconds to retry getting a lock (defaults to 0)',
    ),
    'aliases' => array('qr-pq'),
  );
  $commands['queue-runner-queue-nodes'] = array(
    'examples' => array(
      'drush qr-qn page page_node_worker' => 'Add all page node related tasks to the queue and execute the page_node_worker callback on them',
    ),
    'description' => 'Debugging command - add all tasks related to a certain node type to the queue.',
    'options' => array(
      'retries' => 'Number of retries to allow (default 0)',
    ),
    'aliases' => array('qr-qn'),
  );
  $commands['queue-runner-add-ping-task'] = array(
    'description' => dt('Sends a task to test the task server state.'),
    'aliases' => array('qr-apt'),
  );
  $commands['queue-runner-check-queue-status'] = array(
    'description' => dt('Prints the time of the last known successful task server check.'),
    'aliases' => array('qr-cqs'),
  );
  $commands['queue-runner-check-task-status'] = array(
    'description' => dt('Prints the time of the last known successful task server check.'),
    'aliases' => array('qr-cts'),
    'arguments' => array(
      'task-identifier' => 'The queue runner task identifier.',
    ),
  );
  return $commands;
}


/**
 * Command callback: enqueues a test job on the task server to test task server health.
 */
function drush_queue_runner_add_ping_task() {
  queue_runner_add_ping_task();
  drush_log(dt('Enqueued a test job in the task server.'), 'ok');
}

/**
 * Command callback: prints the date/time of the last succesful task server test.
 */
function drush_queue_runner_check_queue_status() {
  $timestamp = queue_runner_last_check_timestamp();
  $status = queue_runner_check_health_thresholds($timestamp);
  // Always uses a standard date in case the user configured dates lack
  // sufficient granularity and/or timezone data.
  drush_log(gmdate('c', $timestamp), $status);
}

/**
 * Command callback: prints the date/time of the last succesful task server test.
 */
function drush_queue_runner_check_task_status($task_id) {
  if ((int)$task_id == 0) {
    drush_print(dt('Not a valid task id.'));
    return drush_user_abort();
  }

  // Get the database row from queue_runner_in_progress
  $result = db_select('queue_runner_in_progress')
      ->condition('item_id', $task_id)
      ->fields('queue_runner_in_progress')
      ->execute()
      ->fetchAll();

  // Get our result, only 1 task can be shown
  $result = reset($result);
  $state = $result->state;
  switch ($state) {
    case QUEUERUNNER_STATE_WAITING :
      drush_print(dt('Task is waiting'));
      break;
    case QUEUERUNNER_STATE_CLAIMED :
      drush_print(dt('Task has been claimed'));
      break;
    case QUEUERUNNER_STATE_RETRYING :
      drush_print(dt('Task will be retried'));
      break;
    case QUEUERUNNER_STATE_DONE :
      drush_print(dt('Task is done'));
      break;
    case QUEUERUNNER_STATE_DONE :
      drush_print(dt('Task is cancelled'));
      break;
    case QUEUERUNNER_STATE_COLLECTING :
      drush_print(dt('Task is collecting or the collector has failed'));
      break;
    case QUEUERUNNER_STATE_FAILED :
      drush_print(dt('Task has failed'));
      break;
  }

  try {
    $collector = queue_runner_collector_load($task_id);
    if (!empty($collector)) {
      // Generate the filepath to the collector's log file.
      $path = variable_get('queue_runner_log_path', file_directory_temp() . '/queue_runner');
      $filepath = "$path/collectors/$task_id/queue.log";

      if (file_exists($filepath)) {
        module_load_include('inc', 'queue_runner', 'queue_runner.admin');
        $rows = queue_runner_file_log_paged_csv_get($filepath, 100, 1);
        foreach ($rows as $row) {
          drush_print($row[1]);
        }
      }
    }
  }
  catch (Exception $e) {
    // Collector could not be loaded or it is a simple task
  }
}

/**
 * Process items from Queue.
 */
function drush_queue_runner_process_queue() {
  // We don't want to wait for a lock.
  $lock_name = drush_get_option('lock-name', __FUNCTION__);
  $lock_timeout = drush_get_option('lock-timeout', 0);
  _drush_queue_runner_run_with_lock('_drush_queue_runner_process_queue', $lock_name, $lock_timeout);
}

function _drush_queue_runner_process_queue() {
  // Run the worker for a certain period of time before killing it.
  $timeout = drush_get_option('timeout', variable_get('queue_runner_time', '55'));
  $pause = (int) max(0, drush_get_option('pause', variable_get('queue_runner_pause_between_tasks', '5')));

  $end = time() + $timeout;
  drush_print(dt('Running for @time seconds with a pause of @pause.', array('@time' => $timeout, '@pause' => $pause)));
  // Count processed items.
  $items = 0;

  // Get item from queue and process it.
  $queue = queue_runner_queue();
  $use_claim_timeout = method_exists($queue, 'setClaimTimeout');
  $lease_time = variable_get('queue_runner_lease_time', 1200);

  do {
    $claim_next_time = time() + $pause;
    // When supported by the queue class, set how long the call can take at
    // most.
    if ($use_claim_timeout) {
      $remainingTime = $end - time();
      $queue->setClaimTimeout($remainingTime);
    }

    if ($item = $queue->claimItem($lease_time)) {
      $state = queue_runner_do_item($item, $queue);
      if ($state == QUEUERUNNER_STATE_RETRYING) {
        drush_log(dt('Item @item_id was retried', array('@item_id' => $item->data['item_id'])));
      }
      else {
        $queue->deleteItem($item);
        if ($state == QUEUERUNNER_STATE_FAILED) {
          drush_set_error('QUEUE_RUNNER_FAILURE', dt('Item @item_id failed and was logged', array('@item_id' => $item->data['item_id'])));
        }
      }
      $items++;
    }
    drush_print('.', NULL, NULL, FALSE);
    // Sleep for up to $pause seconds.
    $remaining_pause = $claim_next_time - time();
    if ($remaining_pause > 0) {
      sleep($remaining_pause);
    }
  } while (time() < $end);
  // Report how many items we have processed.
  if ($items > 0) {
    drush_print(dt("Total processed records: @items", array('@items' => $items)));
  }
  else {
    drush_print(dt("No records to process"));
  }
}

/**
 * Queue all nodes of a given bundle for processing a given action
 *
 * @param string $bundle
 * @param string $action
 */
function drush_queue_runner_queue_nodes($bundle, $action) {
  $result = db_query("SELECT nid FROM {node} WHERE type = :bundle AND status = 1", array(':bundle' => $bundle))->fetchAll();
  drush_log(dt('Found @count nodes of type @bundle', array('@bundle' => $bundle, '@count' => count($result))));

  $actions = queue_runner_get_entity_workers('node', $bundle);
  drush_log(dt('Found @count actions: @actions', array('@count' => count($actions), '@actions' => print_r($actions, TRUE))));
  if (empty($action) || empty($actions[$action])) {
    $message = dt('Action @action for given bundle @bundle does not exist', array('@action' => $action, '@bundle' => $bundle));
    drush_set_error('QUEUE_RUNNER_FAILURE', $message);
    return;
  }

  $retries = (int) drush_get_option('retries', 0);

  foreach ($result as $n) {
    $data = array(
      'entity_id' => $n->nid,
      'entity_type' => 'node',
      'entity_bundle' => $bundle,
      'retries' => $retries,
    );
    $data['worker'] = $actions[$action];
    queue_runner_add($data);
  }

}

/**
 * Run the given function with a PID lock that is named after the function.
 *
 * If the lock is unavailable within the timeout, log and exit with a soft
 * failure. Other exceptions are logged and re-thrown.
 *
 * @param $function
 *   The function name to call.
 * @param $lock_name
 *   The lock name (defaults to the function name).
 * @param $timeout
 *   The maximum number of seconds to wait for the lock. 0 means to try only
 *   once.
 *
 * @return
 *   $func()'s return value, exit(SOFT_FAILURE_EXIT_CODE), or an exception.
 */
function _drush_queue_runner_run_with_lock($function, $lock_name = NULL, $timeout = 0) {
  if (!isset($lock_name)) {
    $lock_name = $function;
  }
  try {
    // Use the tmp dir as the folder where we save our lock file.
    $tmp_dir = file_directory_temp();
    $lock = QueueRunnerFLock::create($lock_name, $tmp_dir, $timeout);
    $retval = $function($lock);
    $lock->release();
    return $retval;
  }
  catch (QueueRunnerFLockException $e) {
    drush_set_error('QUEUE_RUNNER_CONTENTION', 'alert="lock_contention" ' . $e->getMessage());
  }
  catch (Exception $e) {
    drush_set_error('QUEUE_RUNNER_FATAL', 'alert="fatal_exception" ' . $e->getMessage());
    if (isset($lock)) {
      $lock->release();
    }
    // ??
    //throw $e;
  }
}
