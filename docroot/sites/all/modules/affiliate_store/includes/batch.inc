<?php

/**
 * @file
 * Batch processing functions.
 *
 * As a convention, any application errors occurred in batch queue handler
 * should be logged to $context['results']['errors'] as an array of messages.
 */

/**
 * Count the number of batches of a specific type(s).
 *
 * @param mixed $type
 *   (optional) A single batch type or an array of batch types. Percent sign
 *   (%) can be used as wildcard that matches anything. Default to % meaning
 *   match all types.
 * @param boolean $all
 *   (optional) TRUE to also count inactive batches. Default to FALSE meaning
 *   count active batches only.
 *
 * @return int
 *   Number of pending batches.
 */
function affiliate_store_batch_count($type = '%', $all = FALSE) {
  $type = is_array($type) ? $type : array($type);
  $time = $all ? PHP_INT_MAX : time();
  $count = 0;
  foreach ($type as $pattern) {
    $count += (int) db_select('affiliate_store_batch')
      ->condition('type', $pattern, 'LIKE')
      ->condition('start_time', $time, '<=')
      ->countQuery()
      ->execute()
      ->fetchField();
  }
  return $count;
}

/**
 * Fetch a batch from FIFO queue and execute its handler function.
 *
 * The handler function will receives references to batch data and context,
 * where the context can be used just the same as Drupal batch API context.
 * Both the batch data and context are persistent across calls.
 *
 * There is a lock put on the batch once fetched from queue. It is intended for
 * preventing simultaneous execution like running the same batch from user
 * interface and cron.
 *
 * During execution, handler function is called continuously until it finishes,
 * time out, or maximum operation reached. The batch will be deleted when
 * $context['finished'] is 1. Handler function can also finishes and requeues
 * the current batch by setting $context['requeue'] to TRUE or a future active
 * timestamp, in order to delay processing until later time.
 *
 * @param mixed $type
 *   A string that specify type of batch to dequeue. Percent sign (%) can be
 *   used as wildcard that matches anything. Specify % to dequeue oldest batch
 *   regardless of type.
 * @param mixed &$context
 *   A reference to Drupal batch API context array, which is normally used if
 *   this function is called from within user interface. The context will be
 *   updated as if the handler function is called directly by Drupal batch API.
 *   Additional keys that can be passed are as follow:
 *   - max_operation: (optional) Maximum operation to run. An operation is
 *     count as a return from handler function. The actual number of operations
 *     performed after dequeued will be available in $context['operation']. The
 *     returned $context['operation'] can be perserved across dequeue calls to
 *     achieve overall maximum operation limit. Default to unlimited.
 *   - operation: (optional) Specify the initial operation count, most probably
 *     obtained from previous call. Default to 0.
 *   Pass a NULL variable to ignore or not use this parameter.
 * @param float $time_ratio
 *   (optional) Max execution time usage ratio from 0 to 1, before the function
 *   returns and saves the remaining work for next time. Default to 0.5 meaning
 *   use half of the available execution time.
 * @param string $timer
 *   (optional) Name of timer for timing execution time. Default to timer
 *   'page' which is started when a page load.
 *
 * @return mixed
 *   TRUE if the batch is fetched and run, regardless of whether the batch is
 *   finished or not. NULL if no more active batch to run. FALSE if error
 *   occurred (lock acquisition failed).
 *
 * @see affiliate_store_batch_queue()
 */
function affiliate_store_batch_dequeue($type, &$context, $time_ratio = 0.5,
$timer = 'page') {
  $max_time = ini_get('max_execution_time');
  // If there is no execution time limit, use 5 minutes.
  $max_time = $max_time ? ($max_time * $time_ratio) : (300 * $time_ratio);

  // Lock the batch during execution. The lock period is a bit longer to allow
  // time for cleaning up.
  if (lock_acquire(__FUNCTION__, $max_time - (timer_read($timer) / 1000) + 1)) {
    // Use order by start time to implement FIFO queue.
    // Also order by bid to preserve queue order in case batches have the same
    // start time.
    $result = db_select('affiliate_store_batch', 'b')
      ->fields('b', array('bid', 'batch'))
      ->condition('type', $type, 'LIKE')
      ->condition('start_time', time(), '<=')
      ->orderBy('start_time')
      ->orderBy('bid')
      ->range(0, 1)
      ->execute()
      ->fetchAssoc();
    if ($result) {
      // Prepare the batch.
      $batch = unserialize($result['batch']);
      if ($batch['file']) {
        affiliate_store_include($batch['file']);
      }
      // Mimic Drupal batch API context.
      $batch['context']['message'] = '';
      $batch['context']['requeue'] = FALSE;
      $batch['context'] += array(
        'results' => array(),
        'sandbox' => array(),
      );
      // Get maximum operation to run.
      $operation = isset($context['operation']) ? $context['operation'] : 0;
      $max_operation = (
        isset($context['max_operation']) ?
        $context['max_operation'] : PHP_INT_MAX
      );

      // Run the batch at least once.
      do {
        $batch['context']['finished'] = 1;
        $batch['function']($batch['data'], $batch['context']);
      } while (($batch['context']['finished'] != 1)
        && (++$operation < $max_operation)
        && ((timer_read($timer) / 1000) < $max_time)
      );

      // Update Drupal batch API context if specified.
      if (isset($context)) {
        $context['finished'] = $batch['context']['finished'];
        $context['message'] = $batch['context']['message'];
        // Transfer results into Drupal batch API context. Results that have
        // the same key will be aggregated into an array.
        $context['results'] = array_merge_recursive(
          isset($context['results']) ? $context['results'] : array(),
          array_splice($batch['context']['results'], 0)
        );
        $context['operation'] = $operation;
      }

      if (($batch['context']['finished'] == 1)
        && !$batch['context']['requeue']
      ) {
        // Batch finished, delete it.
        db_delete('affiliate_store_batch')
          ->condition('bid', $result['bid'])
          ->execute();
      }
      else {
        // Batch not finish, renew the batch with remaining data and requeue
        // the batch if requested by handler function.
        $fields = array('batch' => serialize($batch));
        if ($batch['context']['requeue']) {
          $time = max((int) $batch['context']['requeue'], time());
          $fields['start_time'] = $time;
        }
        db_update('affiliate_store_batch')
          ->fields($fields)
          ->condition('bid', $result['bid'])
          ->execute();
      }
    }
    lock_release(__FUNCTION__);
    return $result ? TRUE : NULL;
  }
  return FALSE;
}

/**
 * Add a batch to FIFO queue for execution later.
 *
 * @param array $batch
 *   Batch associative array with the following keys:
 *   - data: Arbitrary batch data for used by handler function.
 *   - function: Handler function responsible for the execution logic with
 *     function signature of function(&$data, &$context).
 *   - file: (optional) The file name without .inc where the handler function
 *     resides. It must be inside the module includes directory.
 *   The data key should be specified first to allow faster searching when
 *   batch uniqueness must be guaranteed.
 * @param string $type
 *   (optional) Batch type for identifying its function group. Hierarchical
 *   grouping can be done by using the convention group:subgroup, e.g. batch
 *   type of product:update can be dequeued by type product or product:update
 *   in increasing specificity. Default to empty.
 * @param mixed $start
 *   (optional) Timestamp when the batch will be active. Default to NULL
 *   meaning use current timestamp.
 * @param boolean $non_unique
 *   (optional) TRUE to allow duplicate batch to be queued. Default to FALSE.
 *
 * @see affiliate_store_batch_dequeue()
 */
function affiliate_store_batch_queue($batch, $type = '', $start = NULL,
$non_unique = FALSE) {
  $batch = serialize($batch);
  $hash = md5($batch);

  if (!$non_unique) {
    $exist = (bool) db_query(
      "SELECT 1 FROM {affiliate_store_batch}
      WHERE type = :type AND hash = :hash",
      array(':type' => $type, ':hash' => $hash)
    )->fetchField();
    if ($exist) {
      return;
    }
  }

  db_insert('affiliate_store_batch')
    ->fields(array(
      'type' => $type,
      'hash' => $hash,
      'batch' => $batch,
      'start_time' => ($start === NULL) ? time() : $start,
    ))
    ->execute();
}

/**
 * Return batch context results.
 *
 * @param array &$results
 *   A reference to context results of executed batch.
 * @param string $aspect
 *   (optional) Only return results that are keyed by this aspect. Default to
 *   NULL meaning return all results.
 * @param boolean $keep
 *   (optional) TRUE to keep the results intact in context. Default to FALSE
 *   which remove the results after they are returned.
 *
 * @return array
 *   Result messages.
 */
function affiliate_store_batch_results(&$results, $aspect = NULL,
$keep = FALSE) {
  if ($aspect !== NULL) {
    if (!isset($results[$aspect])) {
      return array();
    }
    $results = &$results[$aspect];
  }

  $messages = array();
  if (is_array($results)) {
    foreach ($results as $key => $message) {
      if (is_array($message)) {
        $messages = array_merge($messages, $message);
      }
      else {
        $messages[] = $message;
      }

      if (!$keep) {
        unset($results[$key]);
      }
    }
  }
  return $messages;
}
