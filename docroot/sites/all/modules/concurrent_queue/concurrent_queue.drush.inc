<?php

/**
 * @file
 * Drush integration for the Concurrent Queue module.
 */

/**
 * Implements hook_drush_command().
 */
function concurrent_queue_drush_command() {
  $items['queue-run-concurrent'] = array(
    'description' => 'Concurrently run a specific queue by name',
    'arguments' => array(
      'queue_name' => 'The name of the queue to run, as defined in either hook_queue_info or hook_cron_queue_info.',
    ),
    'options' => array(
      'concurrency' => 'The number of parallel processes to run. The default concurrency is 2.',
      'override-time-limit' => 'Override the time limit set in hook_cron_queue_info() when processing. Time in seconds.',
      'no-reclaim' => 'Will not attempt to reclaim items from the queue before processing.',
    ),
    'aliases' => array('qrc'),
    'required-arguments' => TRUE,
    'examples' => array(
      'drush queue-run-concurrent example_queue --concurrency=5 --override-time-limit=300' => "Run the 'example_queue' queue with five different processes, each running for up to 5 minutes.",
    ),
  );

  return $items;
}

/**
 * Validation callback for drush queue-run-concurrent.
 */
function drush_concurrent_queue_queue_run_concurrent_validate($queue_name) {
  $info = _drush_concurrent_queue_get_queue_info($queue_name);
  if (!$info) {
    return drush_set_error('DRUSH_QUEUE_ERROR', dt('Could not find the !name queue.', array('!name' => $queue_name)));
  }
  if ($info['subqueue'] && (int) drush_get_option('concurrency') !== 1) {
    return drush_set_error('DRUSH_SUBQUEUE_CONCURRENCY', dt('Cannot use --concurrency argument to run subqueues.'));
  }
  if (!is_callable($info['worker callback'])) {
    return drush_set_error('DRUSH_QUEUE_ERROR', dt('Unable to find worker callback @callback for the @queue queue.', array('@callback' => $info['worker callback'], '@queue' => $queue_name)));
  }
}

/**
 * Fetch a queue's information from drush_queue_get_queues().
 *
 * If the queue name is an auto-generated process queue, this ensures the
 * 'parent' queue is returned, since there is no information defined about
 * the individual process queue.
 *
 * @param $queue_name
 * @return bool
 */
function _drush_concurrent_queue_get_queue_info($queue_name) {
  drush_include_engine('drupal', 'environment');

  $queues = drush_queue_get_queues();
  $data = array('queue_name' => $queue_name, 'subqueue' => FALSE);

  if (!empty($queues[$queue_name])) {
    // Drush 5 and 6 use ['cron']['callback'] but 7/master uses ['worker callback'].
    if (isset($queues[$queue_name]['cron']['callback']) && !isset($queues[$queue_name]['worker callback'])) {
      $data['worker callback'] = $queues[$queue_name]['cron']['callback'];
    }
    return $data + $queues[$queue_name];
  }
  elseif (preg_match('/^([\w-]+):([\w]+)$/', $queue_name, $matches) && !empty($queues[$matches[1]])) {
    $data['subqueue_name'] = $queue_name;
    $data['queue_name'] = $matches[1];
    $data['subqueue'] = TRUE;
    return $data + _drush_concurrent_queue_get_queue_info($matches[1]);
  }
  return FALSE;
}

/**
 * Command callback for drush queue-run-concurrent.
 */
function drush_concurrent_queue_queue_run_concurrent($queue_name) {
  $queue = new DrushConcurrentQueue($queue_name);

  // Reset expired items in process queues.
  if (!$queue->isSubqueue() && !drush_get_option('no-reclaim') && $reclaim_count = $queue->reclaimItems()) {
    drush_log(dt("Reset @count items that had expired in the @queue queue.", array('@count' => $reclaim_count, '@queue' => $queue_name)), 'ok');
  }

  $count_before = $queue->numberOfItems();
  if (!$count_before) {
    drush_log(dt("There are no items to process in the @name queue.", array('@name' => $queue_name)), 'ok');
    return;
  }

  $concurrency = (int) drush_get_option('concurrency', '2');
  if (!$queue->isSubqueue() && $concurrency > 1) {
    return $queue->invokeWorkers($concurrency);
  }
  else {
    return $queue->run();
  }
}

class DrushConcurrentQueue {

  public function __construct($queue_name, $time_limit = NULL) {
    $this->name = $queue_name;
    $this->info = _drush_concurrent_queue_get_queue_info($queue_name);
    $this->queue = drush_queue_get($queue_name);
    $this->startTime = NULL;
    $this->timeLimit = $time_limit;
  }

  public function __call($name, $arguments) {
    return call_user_func_array(array($this->queue, $name), $arguments);
  }

  public function isSubqueue() {
    return !empty($this->info['subqueue']);
  }

  public function reclaimItems() {
    // If a parent queue is being run, reclaim all unrun and expired items
    // assigned to subqueues or the parent queue.
    $query = db_update('queue');
    $query->fields(array('expire' => 0, 'name' => $this->name));
    $expire_condition = db_or();
    $expire_condition->condition('expire', 0);
    $expire_condition->condition('expire', time(), '<');
    $query->condition($expire_condition);
    $name_condition = db_or();
    $name_condition->condition('name', $this->name);
    $name_condition->condition('name', db_like($this->name . ':') . '%', 'LIKE');
    $query->condition($name_condition);
    return $query->execute();
  }

  public function getTimeLimit($set = TRUE) {
    $max_execution_time = ini_get('max_execution_time');
    $time_limit = drush_get_option('override-time-limit');

    if (!isset($time_limit)) {
      if (!$max_execution_time) {
        $time_limit = 86400;
      }
      else {
        $time_limit = (isset($this->info['cron']['time']) ? $this->info['cron']['time'] : 60);
      }
    }

    // Try to increase the maximum execution time if it is too low.
    if ($set && $max_execution_time > 0 && $max_execution_time < ($time_limit + 30) && !ini_get('safe_mode')) {
      drush_log(dt("Calling set_time_limit(@value).", array('@value' => $time_limit + 30)), 'ok');
      @set_time_limit($time_limit + 30);
    }

    return $time_limit;
  }

  public function startTimer() {
    $this->startTime = microtime(TRUE);
  }

  public function readTimer() {
    return (microtime(TRUE) - $this->startTime);
  }

  public function generateSubqueueName() {
    if (strlen($this->name) > 240) {
      throw new Exception("Unable to generate subqueue name since $this->name is longer the 240 characters.");
    }

    // @todo Should this validate that it doesn't exist?
    return uniqid($this->name . ':');
  }

  public function createSubqueues($concurrency) {
    $item_ids = db_query("SELECT item_id FROM {queue} WHERE name = :name AND expire = 0", array(':name' => $this->name))->fetchCol();
    $item_count = count($item_ids);

    if (!$item_count) {
      throw new Exception("No items to process in subqueues.");
    }
    elseif ($item_count < $concurrency) {
      $concurrency = $item_count;
    }

    $chunk_size = ceil(count($item_ids) / $concurrency);
    $item_chunks = array_chunk($item_ids, $chunk_size);

    $subqueue_names = array();
    foreach ($item_chunks as $id => $item_chunk) {
      $subqueue_name = $subqueue_names[] = $this->generateSubqueueName();
      if ($subqueue_name != $this->name) {
        drush_queue_get($subqueue_name)->createQueue();
        drush_log("Created subqueue $subqueue_name which has " . count($item_chunk) . " items to process.", 'ok');
        db_update('queue')
          ->fields(array('name' => $subqueue_name))
          ->condition('item_id', $item_chunk, 'IN')
          ->condition('expire', 0)
          ->execute();
      }
    }

    return $subqueue_names;
  }

  public function deleteSubqueues(array $subqueue_names) {
    // Reset the subqueue items back to the parent queue name.
    if ($diff = array_diff($subqueue_names, array($this->name))) {
      db_update('queue')
        ->fields(array('name' => $this->name))
        ->condition('name', $diff, 'IN')
        ->execute();
      foreach ($diff as $subqueue_name) {
        drush_queue_get($subqueue_name)->deleteQueue();
      }
    }
  }

  /**
   * Spin off a specific number of concurrent workers to run queues.
   *
   * @param int $concurrency
   *   The number of worker queues to start.
   *
   * @return int
   *   The number of queue items processed by the worker queues.
   */
  public function invokeWorkers($concurrency) {
    if (!is_a($this->queue, 'SystemQueue')) {
      throw new Exception("drush queue-run-concurrent only works for queues that use the SystemQueue class.");
    }

    // @todo Should we check to validate that concurrency might be greater than the number of items in the queue?

    $subqueue_names = $this->createSubqueues($concurrency);
    $invocations = array();
    foreach ($subqueue_names as $subqueue_name) {
      $invocations[] = array(
        'site' => '@self',
        'command' => 'queue-run-concurrent',
        'args' => array($subqueue_name),
      );
    }

    $commandline_options = array(
      'concurrency' => 1,
      'no-reclaim' => TRUE,
    );
    foreach (array('override-time-limit') as $option) {
      $value = drush_get_option($option);
      if (isset($value)) {
        $commandline_options[$option] = $value;
      }
    }
    $backend_options = array();

    $this->getTimeLimit();

    $this->startTimer();
    $results = drush_backend_invoke_concurrent($invocations, $commandline_options, $backend_options);
    $elapsed = $this->readTimer();

    // Calculate the number of items actually processed using the results from
    // the drush_backend_invoke_concurrent() call.
    $count = 0;
    if (!empty($results['concurrent'])) {
      foreach ($results['concurrent'] as $result) {
        if (!empty($result['object']) && is_integer($result['object'])) {
          $count += $result['object'];
        }
      }
    }

    $this->deleteSubqueues($subqueue_names);

    drush_log(dt("Concurrently processed @count items from the @queue queue in @elapsed seconds (@rate/min). There are @remaining remaining items.", array(
      '@count' => $count,
      '@queue' => $this->name,
      '@elapsed' => round($elapsed, 2),
      '@rate' => round(($count / $elapsed) * 60),
      '@remaining' => $this->queue->numberOfItems(),
    )), 'ok');
    return $count;
  }

  /**
   * Run a queue process for as long as possible.
   *
   * @return int
   *   The number of items successfully processed from the queue.
   *
   * @throws Exception
   */
  public function run() {
    $function = $this->info['worker callback'];
    $end = time() + $this->getTimeLimit();
    $count = 0;

    $this->startTimer();
    while (time() < $end && ($item = $this->queue->claimItem())) {
      try {
        drush_log(dt("Processing item @id from @queue.", array('@id' => $item->item_id, '@queue' => $this->name)), 'info');
        $function($item->data);
        $this->queue->deleteItem($item);
        $count++;
      }
      catch (Exception $e) {
        watchdog_exception('concurrent_queue', $e);
        // @todo Figure out how to requeue the item?
        // Duplicate of releaseItem() that also updates the created time.
        /*$requeue = !db_update('queue')
          ->fields(array('created' => time(), 'expire' => 0))
          ->condition('item_id', $item->id)
          ->execute();
        if (!$requeue) {
          watchdog('concurrent_queue', "Unable to requeue item $item->id.", array(), WATCHDOG_ERROR);
        }*/
      }
    }
    $elapsed = $this->readTimer();

    drush_log(dt("Processed @count items from the @queue queue in @elapsed seconds (@rate/min). There are @remaining remaining items.", array(
      '@count' => $count,
      '@queue' => $this->name,
      '@elapsed' => round($elapsed, 2),
      '@rate' => round(($count / $elapsed) * 60),
      '@remaining' => $this->queue->numberOfItems(),
    )), 'ok');
    return $count;
  }

}
