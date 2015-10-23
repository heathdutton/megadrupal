<?php
/**
 * @file
 * Provides Drupal-version-agnostic Queue Runner functions.
 *
 * Functionality that changes between Drupal versions is contained in the main
 * module file.
 */

/**
 * Prepares a given task to be run.
 *
 * This function loads any necessary include files, ensures we have a functional
 * worker callback, and collects together data required for processing the task.
 * If something goes wrong during this process, the task is aborted.
 *
 * @param array $data
 *   The data array for a given task.
 *
 * @return mixed
 *   Either the fully-populated data array or FALSE on error.
 */
function queue_runner_prepare($data) {
  $data += array(
    'coll_id' => NULL,
    'retries' => 0,
    'nid' => NULL,
  );
  $data['retries'] = (int) $data['retries'];
  // The action could be packaged in a different file or files.
  if (isset($data['worker']['includes']) && is_array($data['worker']['includes'])) {
    foreach ($data['worker']['includes'] as $include) {
      module_load_include($include['file_extension'], $include['module'], $include['file']);
    }
  }
  // Make sure we have a functional worker method.
  if (!isset($data['worker']) || !isset($data['worker']['callback']) || !is_callable($data['worker']['callback'])) {
    $message = 'Processed item does not have a callable worker callback to execute @worker';
    $variables = array();
    if (isset($data['worker'])) {
      $variables['@worker'] = print_r($data['worker'], TRUE);
    }
    queue_runner_log($data['item_id'], $data['coll_id'], $data['nid'], $message, $variables, WATCHDOG_ERROR);
    // Abort!
    return FALSE;
  }

  // Some queue items have an associated entity. Load it if needed so each
  // worker doesn't need to implement this logic.
  if (isset($data['entity_id']) && isset($data['entity_type'])) {
    $data['entity'] = _queue_runner_entity_load($data);

    if (empty($data['entity'])) {
      $message = "Failed to load item entity '@entity_type' @entity_id";
      $variables['@entity_type'] = $data['entity_type'];
      $variables['@entity_id'] = $data['entity_id'];
      queue_runner_log($data['item_id'], $data['coll_id'], $data['nid'], $message, $variables, WATCHDOG_ERROR);
      // Abort!
      return FALSE;
    }
    if ($data['entity_type'] == 'node') {
      $data['nid'] = $data['entity_id'];
    }
  }
  return $data;
}

/**
 * Processes fully one or more items from the queue.
 *
 * @param int $max
 *   The maximum number of items that will be processed.
 *
 * @return int
 *   The number of items claimed from the queue.
 */
function queue_runner_do($max = 1) {
  $processed = 0;
  $queue = queue_runner_queue();
  $lease_time = variable_get('queue_runner_lease_time', 1200);

  while ($max > 0) {
    if ($item = $queue->claimItem($lease_time)) {
      $processed += 1;
      $state = queue_runner_do_item($item, $queue);
      if ($state != QUEUERUNNER_STATE_RETRYING) {
        $queue->deleteItem($item);
      }
    }
    $max--;
  }
  return $processed;
}

/**
 * Processes a single item from the queue.
 *
 * @param object $item
 *   The item array for a given task.
 *
 * @return int
 *   The status flag indicating the result state of this operation.
 */
function queue_runner_do_item($item, $queue = NULL) {
  // Mark this item as in progress.
  $progress = queue_runner_in_progress($item->data);
  // Do the actual work.
  $data = queue_runner_prepare($item->data);
  if ($data) {
    $state = queue_runner_process_task($data);
  }
  else {
    $state = QUEUERUNNER_STATE_FAILED;
  }

  if ($state == QUEUERUNNER_STATE_RETRYING) {
    // Even if it failed, we can sucessfully queue for retry. Releasing it
    // allows it to be claimed again.
    if ($progress->retries >= $data['retries'] || ($queue && !$queue->releaseItem($item))) {
      // We really failed since we already retried, have no retries, or the
      // release call failed.
      $state = QUEUERUNNER_STATE_FAILED;
    }
  }
  if ($state == QUEUERUNNER_STATE_FAILED) {
    queue_runner_record_failure(empty($data) ? $item->data : $data);
  }
  queue_runner_update_progress($state, $progress);
  return $state;
}

/**
 * Checks whether a module is blocking actions on an entity.
 *
 * In certain cases a module may need to block any other actions on an entity.
 * This function allows modules to declare this entity as being "blocked" and
 * any tasks that are attempted on it while it is in this blocked state will
 * fail.
 *
 * @param array $data
 *   The data array for a given task.
 *
 * @return bool
 *   Whether this entity is blocked from having tasks run on it.
 */
function queue_runner_entity_blocked($data) {
  if (isset($data['entity_id']) && isset($data['entity_type'])) {
    $result = module_invoke_all('queue_runner_entity_blocked', $data['entity_type'], $data['entity_id'], $data);
    // This will be TRUE if any implementing module returns TRUE.
    return $result && array_filter($result);
  }
  return FALSE;
}

/**
 * Processes a task from the queue.
 *
 * @param array $item
 *   The task to process.
 *
 * @throws
 *   QueueRunnerException
 *
 * @return mixed
 *   Returns either a status flag constant, or the result of call_user_func() to
 *   indicate whether the finalize callback succeeded or not.
 */
function queue_runner_process_task($data) {
  $state = QUEUERUNNER_STATE_FAILED;

  if ($data['coll_id']) {
    // Make sure the collector as a whole wasn't canceled.
    if (queue_runner_collector_canceled($data['coll_id'])) {
      return QUEUERUNNER_STATE_CANCELED;
    }
  }
  if (queue_runner_entity_blocked($data)) {
    return QUEUERUNNER_STATE_FAILED;
  }

  try {
    $result = call_user_func($data['worker']['callback'], $data);
    $state = queue_runner_finalize($data, $result);
  }
  catch (Exception $e) {
    queue_runner_log_exception($e, $data);
    $state = QUEUERUNNER_STATE_RETRYING;
  }

  return $state;
}

/**
 * Loads a row from the database in-progress table.
 *
 * @param object $data
 *   The data array for a given task.
 *
 * @return object
 *   The progress object for a given task.
 */
function queue_runner_in_progress($data) {
  $progress = queue_runner_in_progress_load($data['item_id']);
  $progress->state = QUEUERUNNER_STATE_CLAIMED;
  drupal_write_record('queue_runner_in_progress', $progress, 'item_id');
  return (object) $progress;
}

/**
 * Creates a new row in the database in-progress table.
 *
 * @param array $data
 *   The data array for a given task. The item_id and created properties will be
 *   populated by this function.
 *
 * @return object
 *   The progress object for a given task.
 */
function queue_runner_populate_progress_id(&$data) {
  // This item has not been associated with an ID.
  $data['created'] = time();
  $progress = array(
    'state' => QUEUERUNNER_STATE_WAITING,
    'retries' => 0,
  );
  // Add in nid, coll_id, etc.
  $progress += $data;
  drupal_write_record('queue_runner_in_progress', $progress);
  $data['item_id'] = $progress['item_id'];
  return (object) $progress;
}

/**
 * Calls the finalize function if it exists, otherwise just logs a message.
 *
 * @param array $data
 *   The data array for a given task.
 * @param string $result
 *   The result message of the task.
 *
 * @see queue_runner_process_task()
 *
 * @return mixed
 *   Returns either a status flag constant, or the result of call_user_func() to
 *   indicate whether the finalize callback succeeded or not.
 */
function queue_runner_finalize($data, $result) {
  if (isset($data['worker']['finalize']) && is_callable($data['worker']['finalize'])) {
    $state = call_user_func($data['worker']['finalize'], $data, $result);
  }
  else {
    // Otherwise, just finalize by logging the result.
    queue_runner_log($data['item_id'], $data['coll_id'], $data['nid'], 'RESULT: @result', array('@result' => $result), WATCHDOG_INFO);
    $state = QUEUERUNNER_STATE_DONE;
  }

  return $state;
}

/**
 * Writes a failed item to the queue_runner_failures database table.
 *
 * @param $data
 *   The data array for a given task.
 */
function queue_runner_record_failure($data) {
  // @todo Notify the collector.
  $fields = array(
    'item_id' => $data['item_id'],
    'title' => isset($data['title']) ? $data['title'] : t('Unnamed item created !created', array('!created' => gmdate('c', $data['created']))),
    'data' => $data,
    'nid' => $data['nid'],
    'coll_id' => $data['coll_id'],
    'created' => $data['created'],
  );
  drupal_write_record('queue_runner_failures', $fields);
}

/**
 * Logs an exception when something goes wrong with processing a task.
 *
 * @param $e exception
 *   The exception that was thrown.
 * @param array $data
 *   The data array for a given task.
 */
function queue_runner_log_exception(Exception $e, $data) {
  // Extract the first line of the exception.
  $short_exception = strtok((string) $e, "\n");
  queue_runner_log($data['item_id'], $data['coll_id'], $data['nid'], $short_exception, array(), WATCHDOG_ERROR);
}

/**
 * Logs a message to all implemented log destinations.
 *
 * @param int $item_id
 *   The item_id for the task.
 * @param int $coll_id
 *   The coll_id for the task.
 * @param int $nid
 *   The related node ID for the task.
 * @param string $message
 *   The message to log, in t() placeholder format.
 * @param array $variables
 *   The variables for the t() placeholders in the $message parameter.
 * @param int $severity
 *   The severity level of the log entry. Uses Drupal watchdog module constants.
 *   Defaults to notice.
 * @param string $link
 *   An optional link to something related.
 */
function queue_runner_log($item_id, $coll_id, $nid, $message, $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL) {
  // REQUEST_TIME isn't accurate for long processes, use microtime() instead.
  $microtime = microtime(TRUE);

  // Prepare the fields to be logged.
  $log_entry = array(
    'timestamp' => $microtime,
    'message' => $message,
    'variables' => $variables,
    'severity' => $severity,
    'hostname' => php_uname('n'),
    'item_id' => $item_id,
    'coll_id' => $coll_id,
    'nid' => $nid,
    'link' => $link,
    'uniqid' => uniqid('', TRUE),
  );

  // Allow the log entry data to be manipulated by other modules.
  drupal_alter('queue_runner_log_entry', $log_entry);

  // Provide a mechanism to supply log destinations.
  $info = module_invoke_all('queue_runner_log_info');
  drupal_alter('queue_runner_log_info', $info);

  _queue_runner_log_methods_invoke($info, $log_entry);
}

/**
 * Invokes all implemented log methods on an individual log entry.
 *
 * This function also handles log method settings which could restrict which log
 * entries get logged to which destinations. Specifically, setting the option
 * skip_collector_subtasks to TRUE in the method definition will stop sub-tasks
 * of collectors being logged to that destination.
 *
 * @param array $info
 *   The log method implementations array.
 * @param array $log_entry
 *   The log entry array of the message being saved.
 */
function _queue_runner_log_methods_invoke($info, $log_entry) {
  foreach ($info as $method) {
    // Check if tasks that are part of collectors should be logged.
    if (queue_runner_log_entry_is_collector_subtask($log_entry) && !empty($method['skip_collector_subtasks'])) {
      continue;
    }

    // Check if a callback has been specified and if it's callable.
    if (isset($method['callback']) && is_callable($method['callback'])) {
      call_user_func($method['callback'], $log_entry);
    }
  }
}

/**
 * Implements hook_queue_runner_log_info().
 *
 * Declares log destinations provided by this module.
 *
 * Options:
 *
 * - callback (required): the name of a function that will handle the logging.
 * - skip_collector_subtasks (optional): a boolean value indicating whether to
 *   log sub-tasks of collectors.
 *
 * @return array
 *   An array of log destinations with their associated settings.
 */
function queue_runner_queue_runner_log_info() {
  return array(
    'dblog' => array(
      'callback' => 'queue_runner_dblog',
      'skip_collector_subtasks' => TRUE,
    ),
    'file' => array(
      'callback' => 'queue_runner_file_log',
    ),
  );
}

/**
 * Logs a message to the database.
 *
 * @param array $log_entry
 *   The log entry array of the message being saved.
 */
function queue_runner_dblog($log_entry) {
  $record = array(
    'item_id' => $log_entry['item_id'],
    'coll_id' => $log_entry['coll_id'],
    'nid' => $log_entry['nid'],
    'message' => check_plain($log_entry['message']),
    'variables' => serialize($log_entry['variables']),
    'severity' => $log_entry['severity'],
    'link' => substr($log_entry['link'], 0, 255),
    'hostname' => substr($log_entry['hostname'], 0, 128),
    'timestamp' => (int) $log_entry['timestamp'],
  );
  drupal_write_record('queue_runner_dblog', $record);
}

/**
 * Logs a message to a file.
 *
 * @param array $log_entry
 *   The log entry array of the message being saved.
 */
function queue_runner_file_log($log_entry) {
  // Levels from watchdog_severity_levels() without t() calls.
  static $levels = array(
    WATCHDOG_EMERGENCY => 'emergency',
    WATCHDOG_ALERT => 'alert',
    WATCHDOG_CRITICAL => 'critical',
    WATCHDOG_ERROR => 'error',
    WATCHDOG_WARNING => 'warning',
    WATCHDOG_NOTICE => 'notice',
    WATCHDOG_INFO => 'info',
    WATCHDOG_DEBUG => 'debug',
  );

  $log_entry['timestamp'] = gmdate('c', $log_entry['timestamp']);
  $log_entry['message'] = t($log_entry['message'], $log_entry['variables']);
  $log_entry['severity'] = $levels[$log_entry['severity']];
  unset($log_entry['variables']);
  unset($log_entry['uniqid']);

  // Log error-level messages and above to an additional error log file.
  if (in_array($log_entry['severity'], array('emergency', 'alert', 'critical', 'error'))) {
    queue_runner_file_log_append($log_entry, 'error.log');
  }

  queue_runner_file_log_append($log_entry);
}

/**
 * Appends a message to a log file.
 *
 * @param array $log_entry
 *   The log entry array of the message.
 * @param string $filename
 *   The name of the file (including extension) to log messages to.
 */
function queue_runner_file_log_append($log_entry, $filename = 'queue.log') {
  $file = fopen(queue_runner_filepath_get($log_entry, $filename), 'a');
  fputcsv($file, $log_entry, QUEUE_RUNNER_FILE_LOG_CSV_DELIMITER);
  fclose($file);
}

/**
 * Returns the GELF-formatted JSON string for a given log entry.
 *
 * Implements GELF version 1.0:
 *
 *   https://github.com/Graylog2/graylog2-docs/wiki/GELF
 *
 * @param array $log_entry
 *   The log entry array of the message.
 *
 * @return string
 *   The JSON-formatted log entry string.
 */
function queue_runner_json_get($log_entry) {
  $gelf = array(
    'version' => '1.0',
    'host' => $log_entry['hostname'],
    'short_message' => t($log_entry['message'], $log_entry['variables']),
    // @todo Possibly include backtrace information where appropriate.
    //'full_message' => '',
    'timestamp' => $log_entry['timestamp'],
    'level' => $log_entry['severity'],
    'facility' => 'queue',
    '_uniqid' => $log_entry['uniqid'],
    '_item_id' => $log_entry['item_id'],
    '_coll_id' => $log_entry['coll_id'],
    '_nid' => $log_entry['nid'],
    '_link' => $log_entry['link'],
  );

  drupal_alter('queue_runner_json', $gelf, $log_entry);

  return json_encode($gelf);
}

/**
 * Gets a formatted gmdate.
 *
 * @param array $date
 *   An optional date string in YYYY-MM-DD format.
 *
 * @return array
 *   An array of date fragments in [YYYY, MM, DD] format.
 */
function queue_runner_file_log_date_get($date = NULL) {
  // Validate the date.
  if (empty($date) || !preg_match('/[0-9]{4}-[0-9]{2}-[0-9]{2}/', $date)) {
    $date = gmdate('Y-m-d');
  }
  return $date;
}

/**
 * Gets the log filepath for a particular message.
 *
 * This function will also create the directory where the log file resides, if
 * it doesn't already exist.
 *
 * @param array $log_entry
 *   The log entry array of the message being saved.
 * @param string $filename
 *   The name of the file (including extension) to log messages to.
 *
 * @return string
 *   The filepath to the log file.
 */
function queue_runner_filepath_get($log_entry, $filename) {
  $fragments = array(
    variable_get('queue_runner_log_path', file_directory_temp() . '/queue_runner'),
  );

  // Write logs for sub-tasks of collectors to a sub-directory in the format:
  // collectors/[coll_id].
  if (queue_runner_log_entry_is_collector_subtask($log_entry)) {
    $fragments[] = 'collectors';
    $fragments[] = $log_entry['coll_id'];
  }
  else {
    // Normal tasks go in a general log named by date.
    $fragments[] = queue_runner_file_log_date_get();
  }

  $directory = implode('/', $fragments);

  // If the directory doesn't exist, create it.
  if (!file_exists($directory)) {
    mkdir($directory, 0777, TRUE);
  }

  return $directory . '/' . $filename;
}

/**
 * Checks whether a log entry is for a task that is part of a collector.
 *
 * @param array $log_entry
 *   The log entry array of the message being saved.
 *
 * @return int
 *   The collector ID if log entry is part of a collector, or FALSE.
 */
function queue_runner_log_entry_is_collector_subtask($log_entry) {
  // If the log entry has an item ID and a collector ID, assume this is a
  // sub-task of a collector.
  if (!empty($log_entry['item_id']) && !empty($log_entry['coll_id'])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Populates the workers array based on a given action name.
 *
 * @param array $data
 *   The data array for a given task.
 * @param string $action
 *   The worker action name.
 */
function _queue_runner_set_action_worker(&$data, $action) {
  $workers = array();
  if (isset($data['entity_id']) && isset($data['entity_type'])) {
    // If we are adding an entity, also fetch the entity workers.
    if (isset($data['entity_bundle'])) {
      $workers = queue_runner_get_entity_workers($data['entity_type'], $data['entity_bundle']);
    }
    else {
      $workers = queue_runner_get_entity_workers($data['entity_type']);
    }
  }
  $workers += queue_runner_get_workers();
  if (isset($workers[$action])) {
    $data['worker'] = $workers[$action];
  }
}

/**
 * Adds a node to the queue with a certain worker action.
 *
 * @param object $node
 *   The node object to add.
 * @param string $action
 *   The worker action name.
 * @param string $method
 *   The method of processing; can be either "queued" to add the task to the
 *   queue, or "direct" to immediately process the task.
 * @param int $retries
 *   The number of time a queued item should be retried upon failure.
 *
 * @return mixed
 *   Various return values depending on whether the task should be queued or
 *   processed directly.
 */
function queue_runner_node_add(stdClass $node, $action, $method = 'queued', $retries = 5) {
  $data = array(
    'title' => t('Processing node !title : !action', array('!title' => $node->title, '!action' => $action)),
    'entity_id' => $node->nid,
    'entity_type' => 'node',
    'entity_bundle' => $node->type,
  );

  if ($method == 'queued') {
    // Add the task to the queue.
    return queue_runner_add($data, $action, $retries);
  }
  elseif ($method == 'direct') {
    // Directly process the task now.
    $item = new stdClass();
    $item->data = $data;
    _queue_runner_set_action_worker($item->data, $action);
    // Set a progress id for this item
    queue_runner_populate_progress_id($item->data);
    // Directly execute the task for the item
    do {
      $state = queue_runner_do_item($item);
    } while ($state == QUEUERUNNER_STATE_RETRYING);

    return $item->data['item_id'];
  }
}

/**
 * Adds an item to the queue.
 *
 * @param array $data
 *   The data for the queue item.
 * @param string $action
 *   What to do with the entity if it is being processed. This is just a key
 *   into the data returned by queue_runner_get_workers().
 * @param int $retries
 *   The number of times a queued item should be retried upon failure.
 *
 * @return
 *   The item ID of the task.
 */
function queue_runner_add($data, $action = '', $retries = 0) {
  if ($action) {
    _queue_runner_set_action_worker($data, $action);
  }
  if (empty($data['retries'])) {
    $data['retries'] = $retries;
  }
  $progress = queue_runner_populate_progress_id($data);

  // Enqueue the task.
  if (queue_runner_queue()->createItem($data)) {
    return $progress->item_id;
  }
}

/**
 * Gets the actions that can be executed for items in the queue.
 *
 * This method allows modules to define worker parameters that can be referenced
 * with a simple action name in queue_runner_add().
 *
 * @return array
 *   An array of workers.
 */
function queue_runner_get_workers() {
  $workers = module_invoke_all('queue_runner_workers');
  drupal_alter('queue_runner_workers', $workers);
  return $workers;
}

/**
 * Gets the actions that can be executed for entity items in the queue.
 *
 * This method is like queue_runner_get_workers(), but implementations must
 * filter their response to return actions suitable for queue items with an
 * entity of a specific entity type and possibly a specific bundle.
 *
 * @param string $entity_type
 *   The type of the entity being worked on.
 * @param string $bundle
 *   The bundle type of the entity.
 *
 * @return array
 *   An array of workers.
 */
function queue_runner_get_entity_workers($entity_type, $bundle = NULL) {
  $workers = module_invoke_all('queue_runner_entity_workers', $entity_type, $bundle);
  drupal_alter('queue_runner_entity_workers', $workers, $entity_type, $bundle);
  return $workers;
}

/**
 * Implements hook_queue_runner_entity_workers() on behalf of node module.
 *
 * This is a dummy implmentation for testing.
 *
 * @param string $entity_type
 *   The type of the entity being worked on.
 * @param string $bundle
 *   The bundle type of the entity.
 *
 * @return array
 *   An array of workers.
 */
function node_queue_runner_entity_workers($entity_type, $bundle) {
  if ($entity_type == 'node' && $bundle == 'page') {
    return array(
      'page_node_worker' => array(
        'callback' => 'queue_runner_page_worker',
      ),
    );
  }
}

/**
 * Defines a dummy worker callback for a entity task for testing.
 *
 * @param array $data
 *   The data array for a given task.
 *
 * @return string
 *   Dummy data displaying the node ID and title.
 */
function queue_runner_page_worker($data) {
  return t('Node ID: @nid Title: @title', array('@nid' => $data['entity']->nid, '@title' => $data['entity']->title));
}

/**
 * Creates a new collector.
 *
 * @param string $title
 *   The title of the collector.
 * @param array $data
 *   Optional data to be saved with the collector.
 *
 * @throws
 *   QueueRunnerException
 *
 * @return object
 *   The collector record.
 */
function queue_runner_collector_create($title, $data) {
  $record = array(
    'title' => $title,
    'created' => time(),
    'coll_id' => $data['item_id'],
    'data' => $data,
    'status' => 'created',
    'result' => array(),
    'nid' => (isset($data['nid']) ? $data['nid'] : 0),
  );
  if (!isset($record['data']['collector'])) {
    $record['data']['collector'] = array();
  }
  if (!isset($record['data']['collector']['discard'])) {
    // By default nested collectors are not preserved.
    $record['data']['collector']['discard'] = !empty($data['coll_id']);
  }
  if (!isset($record['data']['collector']['store_results'])) {
    // By default nested collectors store their results and pass them back.
    $record['data']['collector']['store_results'] = !empty($data['coll_id']);
  }
  $result = drupal_write_record('queue_runner_collector', $record);
  if ($result === FALSE || empty($record['coll_id'])) {
    throw new QueueRunnerException('Failed to create new collector', $data);
  }
  return (object) $record;
}

/**
 * Loads an individual collector by ID.
 *
 * @param int $coll_id
 *   The ID of a collector to load.
 *
 * @return object
 *   The collector record.
 *
 * @throws
 *   QueueRunnerException
 */
function queue_runner_collector_load($coll_id) {
  $records = queue_runner_collector_load_multiple(array($coll_id));
  return $records[$coll_id];
}

/**
 * Loads multiple collectors by ID.
 *
 * @param array $coll_ids
 *   Array of (int) collector IDs to load.
 * @return array
 *   Array of collector objects, keyed by collector ID.
 */
function queue_runner_collector_load_multiple($coll_ids) {
  $records = _queue_runner_collector_load($coll_ids);
  if (array_diff($coll_ids, array_keys($records))) {
    throw new QueueRunnerException('Failed to load collector', array(':coll_ids' => $coll_ids));
  }
  foreach ($records as &$record) {
    if ($record->data) {
      $record->data = unserialize($record->data);
    }
    if ($record->result) {
      $record->result = unserialize($record->result);
    }
  }
  return $records;
}

/**
 * Loads an individual collector by ID.
 *
 * @param int $coll_id
 *   The ID of the collector to load.
 *
 * @return bool
 *   TRUE if the collector exists.
 */
function queue_runner_collector_exists($coll_id) {
  return (bool) _queue_runner_collector_load(array($coll_id));
}

/**
 * Logs a message when a collector in created.
 *
 * @param array $data
 *   The data array for a given task.
 * @param string $result
 *   The result message of the task.
 *
 * @return int
 *   The task status flag for "collecting".
 */
function queue_runner_finalize_collector_creation($data, $result) {
  queue_runner_log($data['item_id'], $data['coll_id'], $data['nid'], 'RESULT: @result', array('@result' => $result), WATCHDOG_INFO);
  return QUEUERUNNER_STATE_COLLECTING;
}

/**
 * Flattens nested results to an array of strings.
 *
 * @param mixed $result
 *   An array, object, or string representing task results.
 *
 * @return array
 *   A flattened array of task results.
 */
function queue_runner_result_flatten($result) {
  $flattened = array();

  if (is_array($result) || is_object($result)) {
    foreach ($result as $row) {
      // Recurse and merge results.
      $flattened = array_merge($flattened, queue_runner_result_flatten($row));
    }
  }
  else {
    $flattened[] = (string) $result;
  }

  return $flattened;
}

/**
 * Creates a serial collector and queues one sub-task.
 *
 * This is just a decorator around queue_runner_create_bounded_collector which
 * enforces number=1 and a couple of minor details.
 *
 * @param array $data
 *   The collector data array.
 *
 * @throws
 *   QueueRunnerException
 *
 * @return int
 *   The collector ID.
 */
function queue_runner_create_serial_collector($data) {
  $data['title'] = (isset($data['title']) ? $data['title'] : 'serial collector ' . gmdate('c'));
  $data['number'] = 1;
  $data['type'] = (isset($data['type']) ? $data['type'] : 'serial_collector');
  return queue_runner_create_bounded_collector($data);
}

/**
 * Creates a bounded collector and queues a specified number of sub-tasks.
 *
 * @param array $data
 *   The collector data array.
 *
 * @throws
 *   QueueRunnerException
 *
 * @return int
 *   The collector ID.
 */
function queue_runner_create_bounded_collector($data) {
  $data['subtasks'] = (array) $data['subtasks'];

  // Set some defaults.
  $data += array(
    'number' => 1,
    'type' => 'bounded_collector',
  );
  $title = isset($data['title']) ? $data['title'] : 'bounded collector ' . gmdate('c');

  $total = count($data['subtasks']);

  if ($total == 0) {
    $message = 'COLLECTOR ABORTED: @title had 0 subtasks';
    queue_runner_log($data['item_id'], NULL, $data['nid'], $message, array('@title' => $title), WATCHDOG_WARNING);
    return t($message, array('@title' => $title));
  }

  $record = queue_runner_collector_create($title, $data);
  $record->total = $total;
  $record->queued = 0;
  for ($i = 1; $i <= $data['number'] && count($record->data['subtasks']) > 0; ++$i) {
    $task = array_shift($record->data['subtasks']);
    _queue_runner_serial_task_create($task, $record);
    $record->queued++;
    $record->data['subtasks_queued'][] = $task;
  }
  $record->status = 'started';
  $record->remaining = $record->total;
  $record->data['collector_finalize'] = 'queue_runner_collector_finalize';
  $record->nid = $data['nid'];
  queue_runner_collector_update($record);

  $variables = array(
    '@type' => $record->data['type'],
    '@coll_id' => $record->coll_id,
    '@total' => $record->total,
  );
  return t('Created @type @coll_id with @total sub-tasks', $variables);
}

/**
 * Populates the task array and immediately adds it to the queue.
 *
 * @param array $task
 *   The task array to populate.
 * @param object $record
 *   The database record object for the collector.
 *
 * @return int
 *   The item ID of the task.
 */
function _queue_runner_serial_task_create(&$task, $record) {
  $task['data']['coll_id'] = $record->coll_id;

  if (!empty($task['action'])) {
    _queue_runner_set_action_worker($task['data'], $task['action']);
  }
  if (empty($task['data']['worker']['finalize'])) {
    $task['data']['worker']['finalize'] = 'queue_runner_serial_subtask_finalize';
  }

  // Immediately enqueue this task.
  $item_id = queue_runner_add($task['data']);
  _queue_runner_collector_relate($record->coll_id, $item_id);

  return $item_id;
}

/**
 * Helper function to retry multiple times the acquisition of a lock.
 *
 * @param string $lock
 *   The lock name.
 * @param string $message
 *   The message to use on failure for the exception message.
 * @param mixed $data
 *   Optional array or object related to the lock acquisition to be included
 *   in the exception if thrown.
 *
 * @throws
 *   QueueRunnerException
 */
function _queue_runner_lock_acquire($lock, $error_message, $data = array()) {
  if (!lock_acquire($lock)) {
    $retry = 0;
    while (lock_wait($lock) || !lock_acquire($lock)) {
      $retry += 1;
      if ($retry > QUEUERUNNER_LOCK_MAX_RETRIES) {
        throw new QueueRunnerException($error_message, $data);
      }
    }
  }
}

/**
 * Updates the database record for an individual collector.
 *
 * @param object $record
 *   The database record object for the collector.
 *
 * @return object
 *   The updated database record object for the collector.
 */
function queue_runner_collector_update(&$record) {
  drupal_write_record('queue_runner_collector', $record, 'coll_id');
  return $record;
}

/**
 * Ensures only a single sub-task is in the queue at one time.
 *
 * When a serial sub-task is processed from the queue it calls this function
 * after its own worker callback has completed. This function then adds another
 * task to the queue. This loop continues until all of the sub-tasks have been
 * queued and processed.
 *
 * @param array $data
 *   The data array for the sub-task.
 * @param string $result
 *   The result message of the sub-task.
 *
 * @throws
 *   QueueRunnerException
 *
 * @return int
 *   The status flag indicating that this sub-task was "done".
 */
function queue_runner_serial_subtask_finalize($data, $result) {
  // Lock the collector to avoid multiple processes updating the same collector.
  $lock = 'queue_runner_collector:' . $data['coll_id'];
  _queue_runner_lock_acquire($lock, 'Failed to acquire collector lock.', $data);

  $record = queue_runner_collector_load($data['coll_id']);

  if (!empty($record->data['collector']['store_results'])) {
    // By default nested collectors store their results and pass them back.
    $record->result[$data['item_id']] = $result;
  }
  else {
    // Keep track of the number of results immediately passed to this collector.
    $record->result[$data['item_id']] = $data['item_id'];
    // Flatten and log all results we received.
    $flattened = queue_runner_result_flatten($result);
    foreach ($flattened as $row) {
      queue_runner_log($data['item_id'], $record->coll_id, $data['nid'], 'RESULT: @result', array('@result' => $row), WATCHDOG_INFO);
    }
  }

  // Decrease the number of remaining items.
  $record->remaining = $record->total - count($record->result);

  if (count($record->data['subtasks']) == 0) {
    if ($record->remaining == 0) {
      // If there are no sub-tasks remaining, mark this collector as finished.
      $record->status = 'finished';
      $record->queued = 0;
    }
    else {
      $record->status = 'working';
      $record->queued = count($record->data['subtasks_queued']) - count($record->result);
    }
  }
  else {
    $record->status = 'working';

    // We only add a single sub-task to the queue for every one completed; we
    // don't explicitly check the number this bounded collector is maintaining.
    $task = array_shift($record->data['subtasks']);
    _queue_runner_serial_task_create($task, $record);

    // Keep track of which tasks have been queued and the total number of queued
    // tasks.
    $record->data['subtasks_queued'][] = $task;
    $record->queued = count($record->data['subtasks_queued']) - count($record->result);
  }

  queue_runner_collector_update($record);

  // This sub-task is now completed, so invoke the post-completion callback to
  // roll-up any results that may need to be passed back, update the progress,
  // and potentially delete any nested collectors that have fully completed.
  if (count($record->result) >= $record->total) {
    queue_runner_collector_finalize($record);
  }

  lock_release($lock);

  return QUEUERUNNER_STATE_DONE;
}

/**
 * Creates a parallel collector and enqueues all the sub-tasks.
 *
 * @param array $data
 *   The collector data array.
 *
 * @throws
 *   QueueRunnerException
 *
 * @return string
 *   The result message.
 */
function queue_runner_create_parallel_collector($data) {
  $data['subtasks'] = (array) $data['subtasks'];

  // Set some defaults.
  $data['type'] = isset($data['type']) ? $data['type'] : 'parallel_collector';
  $title = isset($data['title']) ? $data['title'] : 'parallel collector ' . gmdate('c');

  $total = count($data['subtasks']);

  // If there are no sub-tasks in this collector, abort processing and log a
  // message.
  if ($total == 0) {
    $message = 'COLLECTOR ABORTED: @title had 0 subtasks';
    queue_runner_log($data['item_id'], NULL, $data['nid'], $message, array('@title' => $title), WATCHDOG_WARNING);
    return t($message, array('@title' => $title));
  }

  $record = queue_runner_collector_create($title, $data);
  $record->total = $total;
  $record->queued = 0;

  foreach ($data['subtasks'] as $task) {
    $task['data']['coll_id'] = $record->coll_id;

    // Set the various worker callbacks for this sub-task.
    if (!empty($task['action'])) {
      _queue_runner_set_action_worker($task['data'], $task['action']);
    }
    if (empty($task['data']['worker']['finalize'])) {
      $task['data']['worker']['finalize'] = 'queue_runner_parallel_subtask_finalize';
    }

    // Enqueue the sub-task.
    $item_id = queue_runner_add($task['data']);
    _queue_runner_collector_relate($record->coll_id, $item_id);
    $record->queued++;
  }

  $record->status = 'started';
  $record->remaining = $record->queued;
  $record->nid = $data['nid'];
  queue_runner_collector_update($record);

  $variables = array(
    '@coll_id' => $record->coll_id,
    '@total' => $record->total,
  );
  return t('Created parallel collector @coll_id with @total sub-tasks', $variables);
}

/**
 * Runs after each parallel sub-task has completed.
 *
 * When a parallel sub-task is processed from the queue it calls this function
 * after its own worker callback has completed. This function keeps track of how
 * many sub-tasks are remaining, collects task result data from nested
 * collectors, updates the database, and invokes the collector finalize
 * callback.
 *
 * @param array $data
 *   The data array for the sub-task.
 * @param string $result
 *   The result message of the sub-task.
 *
 * @throws
 *   QueueRunnerException
 *
 * @return int
 *   The status flag indicating that this sub-task was "done".
 */
function queue_runner_parallel_subtask_finalize($data, $result) {
  // Lock the collector to avoid multiple processes updating the same collector.
  $lock = 'queue_runner_collector:' . $data['coll_id'];
  _queue_runner_lock_acquire($lock, 'Failed to acquire collector lock.', $data);

  $record = queue_runner_collector_load($data['coll_id']);
  $record->queued--;
  $record->remaining = _queue_runner_collector_remaining($record->coll_id, $data['item_id']);

  if (!empty($record->data['collector']['store_results'])) {
    // By default nested collectors store their results and pass them back.
    $record->result[$data['item_id']] = $result;
  }
  else {
    // Keep track of the number of results immediately passed to this collector.
    $record->result[$data['item_id']] = $data['item_id'];
    // Flatten and log all results we received.
    $flattened = queue_runner_result_flatten($result);
    foreach ($flattened as $row) {
      queue_runner_log($data['item_id'], $record->coll_id, $data['nid'], 'RESULT: @result', array('@result' => $row), WATCHDOG_INFO);
    }
  }

  // Beware narrow race condition of the last 2 tasks finalizing at once.
  if ($record->remaining <= 0) {
    $record->status = 'finished';
    $record->queued = 0;
    $record->remaining = 0;
  }
  else {
    $record->status = 'working';
  }

  queue_runner_collector_update($record);

  // This collector has now completed all remaning tasks so call the collector
  // completion callback to handle any post-completion duties.
  if (!$record->remaining) {
    queue_runner_collector_finalize($record);
  }

  lock_release($lock);

  return QUEUERUNNER_STATE_DONE;
}

/**
 * Handles final duties for completed sub-tasks of collectors.
 *
 * This function runs after a sub-task has completed its worker callback. It
 * invokes collector passback (for instance, to consolidate log message output),
 * updates the progress table in the database, and will delete nested collectors
 * if all of their tasks have completed.
 *
 * @param object $record
 *   The collector's record object that had a sub-task complete.
 */
function queue_runner_collector_finalize($record) {
  $state = queue_runner_passback($record);
  $progress = queue_runner_in_progress_load($record->data['item_id']);
  queue_runner_update_progress($state, $progress);

  // By default, nested collectors can be deleted
  if (!empty($record->data['collector']['discard'])) {
    _queue_runner_collector_delete($record->coll_id);
  }
}

/**
 * Invokes the collector's passback callback, if one exists.
 *
 * @param object $record
 *   The collector's record object that had a sub-task complete.
 *
 * @return int
 *   The status flag indicating the result state of this operation.
 */
function queue_runner_passback($record) {
  $data = queue_runner_prepare($record->data);

  // If a collector passback callback is set and callable, invoke it now.
  if (isset($data['worker']['collector_passback']) && is_callable($data['worker']['collector_passback'])) {
    $state = call_user_func($data['worker']['collector_passback'], $data, $record->result);
  }
  else {
    $state = QUEUERUNNER_STATE_DONE;

    $message = 'COLLECTOR FINISHED: @title with @count subtask results';
    $variables = array(
      '@title' => $record->title,
      '@count' => count($record->result),
    );
    queue_runner_log(NULL, $record->coll_id, $record->nid, $message, $variables, WATCHDOG_INFO);
  }

  return $state;
}

/**
 * Implements hook_queue_runner_workers().
 */
function queue_runner_queue_runner_workers() {
  return array(
    'parallel_collector' => array(
      'callback' => 'queue_runner_create_parallel_collector',
      'finalize' => 'queue_runner_finalize_collector_creation',
    ),
    'serial_collector' => array(
      'callback' => 'queue_runner_create_serial_collector',
      'finalize' => 'queue_runner_finalize_collector_creation',
    ),
    'bounded_collector' => array(
      'callback' => 'queue_runner_create_bounded_collector',
      'finalize' => 'queue_runner_finalize_collector_creation',
    ),
  );
}

/**
 * Throws an exception on failure, unless the number of retries is exhausted.
 *
 * This is a helper function for subtasks within nested collectors where a hard
 * failure (setting the subtask to failed) would be too disruptive. Setting the
 * subtask to failed would cause the parent collector(s) to abort, meaning
 * subsequent tasks within the collector would not have a chance to run.
 * Instead, this function throws an exception if below the retry limit,
 * otherwise it sets a collector message and returns the error string as the
 * result.
 *
 * @param array $data
 *   The data array for a given task.
 * @param string $error
 *   The error message for the exception or result.
 * @param string $message
 *   Optional message for the collector. Defaults to "failure".
 * @param string $exception_class
 *   Optional class for the exception. Defaults to "Exception".
 *
 * @return string
 *   The error message.
 */
function queue_runner_subtask_failure($data, $error, $message = 'failure', $exception_class = 'Exception') {
  $progress = queue_runner_in_progress_load($data['item_id']);
  if ($progress->retries >= $data['retries']) {
    // Retries are exhausted, set a failure message.
    queue_runner_collector_message_add($data['coll_id'], $message);
    return $error;
  }
  else {
    throw new $exception_class($error);
  }
}

/**
 * Throws an exception for failed items.
 *
 * This is a worker function for testing failed items.
 *
 * @param array $data
 *   The data array for a given task.
 *
 * @return string
 *   The title of the task, if one is set, otherwise an empty string.
 */
function queue_runner_test_worker($data) {
  $title = !empty($data['title']) ? $data['title'] : '';
  if (!empty($data['state']) && $data['state'] == 'failed') {
    throw new Exception("Failing item: $title");
  }
  return $title;
}

/**
 * Gets the current time rounded down to the start of the hour.
 *
 * @return int
 *   The current time rounded down to the start of the hour.
 */
function _queue_runner_get_current_hour() {
  $current_time = time();
  return $current_time - ($current_time % 3600);
}

/**
 * Returns a row representing the combined data of the rows passed in.
 *
 * @param array $records
 *   An array of associative arrays containing task stats.
 * @param string $date_format
 *   A date format string to be passed to date().
 *
 * @return array
 *   An associative array combining the information in the passed in records.
 */
function _queue_runner_reaverage_daily($records, $date_format = NULL) {
  $num_days = ceil(count($records)/24);
  $rows = array();
  for ($i = 0; $i < $num_days; $i++) {
    $stats = array();
    $day = array_slice($records, ($i * 24), 24);
    foreach ($day as $record) {
      foreach (array_keys($record) as $field) {
        $stats[$field][] = $record[$field];
      }
    }
    // Use the timestamp of the last record for the new row representing the
    // lot.
    $last_record = end($day);
    $last_ts = $last_record['hour_ts'];
    // Precede the fields with a 'time' element containing the fomatted date.
    $row = array(
      'hour_ts' => $last_ts,
      'completed_tasks' => array_sum($stats['completed_tasks']),
      'max_duration' => max($stats['max_duration']),
      'av_duration' => round(array_sum($stats['av_duration']) / count($day)),
      'max_retries' => max($stats['max_retries']),
      'num_failures' => array_sum($stats['num_failures']),
    );
    if ($date_format) {
      // Show a formatted date instead of the timestamp.
      unset($row['hour_ts']);
      $rows[] = array('date' => date($date_format, $last_ts)) + $row;
    }
    else {
      $rows[] = $row;
    }
  }
  return $rows;
}

/**
 * Cancels a collector.
 *
 * This function cancels a collector by simply updating the "status" column in
 * the database for the given collector.
 *
 * @param object $collector
 *   The collector object to cancel.
 *
 * @throws
 *   QueueRunnerException
 */
function queue_runner_collector_cancel($collector) {
  if (queue_runner_collector_cancel_allowed($collector)) {
    // Lock the collector to ensure multiple processes can't update the same
    // collector.
    $lock = 'queue_runner_collector:' . $collector->coll_id;
    _queue_runner_lock_acquire($lock, 'Failed to acquire collector lock.', $collector);

    // Set the status of the collector to "canceled".
    $collector->status = 'canceled';
    queue_runner_collector_update($collector);

    // Log a message including the user who canceled the collector.
    $message = 'CANCELED: Collector @coll_id was canceled by @user';
    $variables = array(
      '@coll_id' => $collector->coll_id,
      '@user' => $GLOBALS['user']->name,
    );
    queue_runner_log(NULL, $collector->coll_id, $collector->nid, $message, $variables, WATCHDOG_INFO);

    lock_release($lock);
  }
}

/**
 * Cancels a collector and all of its child collectors/tasks
 *
 * Note that any subtask, queued or not does not need explicit cancellation, but
 * items marked as failed do need to be removed from the fail log, otherwise it
 * would be possible to retry them, and behavior then is undefined.
 *
 * @param $collector
 *   Parent collector to cancel.
 */
function queue_runner_collector_cancel_recursive($collector) {
  // It is important to cancel parents before recursing, otherwise the children
  // would not permit their cancellation.
  queue_runner_collector_cancel($collector);
  _queue_runner_failures_delete($collector->coll_id);
  if ($children = queue_runner_load_collector_children($collector)) {
    foreach ($children as $child) {
      queue_runner_collector_cancel_recursive($child);
    }
  }
}


/**
 * Inserts a test task into the queue to check queue health.
 */
function queue_runner_add_ping_task() {
  $data = array(
    'worker' => array('callback' => 'queue_runner_check_callback'),
  );
  queue_runner_add($data);
}

/**
 * Queue worker: updates the timestamp on a file as the "last known OK" time.
 *
 * Note: if needed for any reason in future, this function is passed one
 * argument, which is an array of queue job data.
 *
 */
function queue_runner_check_callback($data = array()) {
  touch(_queue_runner_queue_monitor_file());
}

/**
 * Returns the timestamp of the last time a test task ran successfully.
 *
 * @return int
 *   Unix timestamp of the last succesful test of the task queue (zero if the
 *   file does not exist).
 */
function queue_runner_last_check_timestamp() {
  return (int) @filemtime(_queue_runner_queue_monitor_file());
}

/**
 * Returns a status string corresponding to the amount of time since the task server was last checked as OK.
 *
 * @param int $age
 *   Number of seconds since the task server was last known to be OK.
 *
 * @return string
 *   Nagios levels: 'ok', 'warning' or 'error'
 */
function queue_runner_check_health_thresholds($timestamp, $now = NULL) {
  if (!isset($now)) {
    // Since a request may span a minute or more, we want time()
    // rather than the REQUEST_TIME.
    $now = time();
  }
  $age = $now - $timestamp;
  // Default thresholds: ok < 5 mins; warning < 10 mins; error > 10 mins
  if ($age < variable_get('queue_runner_queue_ok_threshold', 300)) {
    return 'ok';
  }
  elseif ($age < variable_get('queue_runner_queue_warning_threshold', 600)) {
    return 'warning';
  }
  else {
    return 'error';
  }
}


/**
 * Parses the lines from CSV files to rows using a paging mechanism.
 *
 * @param string $filepath
 *   The system path to the log file to parse.
 * @param int $items
 *   The number of log items to retrieve per page.
 * @param int $page
 *   The page of results to retrieve.
 *
 * @return array
 *   An array of rows for passing to theme_table().
 */
function queue_runner_file_log_paged_csv_get($filepath, $items, $page) {
  $rows = array();

  if (file_exists($filepath)) {
    if (($file = fopen($filepath, 'r')) !== FALSE) {
      // Get the beginning and end line numbers for the range we want to keep.
      $range_first = ($page - 1) * $items + 1;
      $range_last = $page * $items;

      // Always start counting at the first line.
      $pointer = 1;

      // Iterate until the last line in the range we want.
      while ($pointer <= $range_last) {
        if ($pointer < $range_first) {
          // Throw away unwanted lines.
          fgetcsv($file, QUEUE_RUNNER_FILE_LOG_MAX_LINE_LENGTH, QUEUE_RUNNER_FILE_LOG_CSV_DELIMITER);
        }
        else {
          // The call to fgetcsv() returns FALSE on error or EOF.
          if ($row = fgetcsv($file, QUEUE_RUNNER_FILE_LOG_MAX_LINE_LENGTH, QUEUE_RUNNER_FILE_LOG_CSV_DELIMITER)) {
            // Row arrays need to be padded to ensure the table is well-formed.
            $rows[] = array_pad($row, 8, '');
          }
          else {
            break;
          }
        }
        $pointer++;
      }

      fclose($file);
    }
  }

  return $rows;
}


/**
 * Get the log files for specific collectors
 *
 * @param type $coll_id
 * @param type $type
 * @param type $page
 * @param string $num_items
 */
function queue_runner_file_get_log_collectors($coll_id, $type = 'queue', $page = '1', $num_items = NULL) {
  // Make sure type is an expected value to prevent file traversal.
  $type = ($type == 'error') ? 'error' : 'queue';

  // Only allow items to be less than 1000.
  if (empty($num_items) || $num_items > 1000) {
    $num_items = QUEUE_RUNNER_FILE_LOG_DEFAULT_RESULTS_PER_PAGE;
  }
  // Generate the filepath to the collector's log file.
  $path = variable_get('queue_runner_log_path', file_directory_temp() . '/queue_runner');
  $filepath = "$path/collectors/$coll_id/$type.log";
  if (file_exists($filepath)) {
    $rows = queue_runner_file_log_paged_csv_get($filepath, $num_items, $page);
   }
  return $rows;

}
