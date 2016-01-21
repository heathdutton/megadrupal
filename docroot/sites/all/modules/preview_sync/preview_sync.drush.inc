<?php

/**
 * @file
 * Drush commands that help with Preview Sync.
 */

/**
 * Implements hook_drush_command()
 */
function preview_sync_drush_command() {
  $items = array();

  $items['preview-sync-worker'] = array(
    'aliases' => array('psw'),
    'description' => dt('The Preview Sync worker task that runs in a loop X number of times. There is exclusive locking in Drupal which means only one of these workers will ever be doing anything at a single point in time.'),
    'options' => array(
      'pause' => dt('Number of seconds to pause between loops. Defaults to 10.'),
      'repeat' => dt('Number of times to loop. Defaults to 17.'),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'examples' => array(
      'drush preview-sync-worker' => dt('Runs the Preview Sync worker.'),
      'drush preview-sync-worker --pause=5 --repeat=36' => dt('Runs the Preview Sync worker with a 5 second pause and 36 times (so 3 minutes).'),
    ),
  );

  $items['workbench-moderation-publish'] = array(
    'aliases' => array('wmp'),
    'description' => dt('Publishes all Drupal content in the "review" state. This should only ever be run on the preview server. The command assumes you are using the workbench_moderation module for workflow.'),
    'options' => array(
      'offset' => dt('What index should the content be published starting at. Defaults to 0.'),
      'limit' => dt('How many items of content should be published (after the offset). Defaults to all content.'),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'drupal dependencies' => array('preview_sync', 'workbench_moderation'),
    'examples' => array(
      'drush workbench-moderation-publish @preview' => dt('Publishes all Drupal content in the review state.'),
      'drush workbench-moderation-publish @preview --offset=10 --limit=10' => dt('Publishes Drupal content in the review state with an offset of the first 10 items, processing the next 10 items.'),
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function preview_sync_drush_help($section) {
  switch ($section) {
    case 'drush:preview-sync-worker' :
      return dt('The Preview Sync worker task that runs in a loop X number of times. There is exclusive locking in Drupal which means only one of these workers will ever be doing anything at a single point in time.');
    case 'drush:workbench-moderation-publish' :
      return dt('Publishes all Drupal content in the "review" state. This should only ever be run on the preview server. The command assumes you are using the workbench_moderation module for workflow.');
  }
}

/**
 * Generic exception logging.
 *
 * The usual exception messages we're receiving in these cases are fairly
 * useless, so this includes more of the details.
 */
function _preview_sync_log_exception($e, $item, $output, $time) {
  $args = array(
    '!msg' => $e->getMessage(),
    '@file' => $e->getFile(),
    '@line' => $e->getLine(),
    '@command' => $item->data['command'],
    '@time' => $time,
  );
  var_dump($args);
  $message = '!msg in @file at line @line. Task (@command), taking @time milliseconds.';
  drush_log(dt($message, $args), 'error');

  // Custom logging.
  db_insert('preview_sync_log')
    ->fields(array(
      'pid' => $item->data['pid'],
      'qid' => $item->item_id,
      'uid' => $item->data['uid'],
      'start' => $item->data['start'],
      'duration' => $time,
      'type' => $item->data['type'],
      'status' => PREVIEW_SYNC_ERROR,
      'message' => t($message, $args),
    ))
    ->execute();
}

/**
 * Generic success logging.
 *
 * Logs to both Drush and the Preview Sync table.
 */
function _preview_sync_log_success($item, $result, $time) {
  $args = array(
    '@command' => $item->data['command'],
    '@time' => $time,
  );
  $message = $result . ': Task (@command), taking @time milliseconds.';
  drush_log(dt($message, $args), 'ok');

  // Custom logging.
  db_insert('preview_sync_log')
    ->fields(array(
      'pid' => $item->data['pid'],
      'qid' => $item->item_id,
      'uid' => $item->data['uid'],
      'start' => $item->data['start'],
      'duration' => $time,
      'type' => $item->data['type'],
      'status' => PREVIEW_SYNC_SUCCESS,
      'message' => t($message, $args),
    ))
    ->execute();
}

/**
 * Basic validation for the arguments to the Drush command.
 */
function drush_preview_sync_worker_validate() {
  $pause = trim(drush_get_option('pause'));
  if ($pause) {
    if (!is_numeric($pause) || $pause < 1) {
      return drush_set_error('INVALID_PAUSE_FORMAT', dt('Pause argument must be a positive integer (e.g. 10)'));
    }
  }

  $repeat = trim(drush_get_option('repeat'));
  if ($repeat) {
    if (!is_numeric($repeat) || $repeat < 1) {
      return drush_set_error('INVALID_REPEAT_FORMAT', dt('Repeat argument must be a positive integer (e.g. 17)'));
    }
  }
}

/**
 * Attempts to claim an item in the queue, and execute it. Every item in the
 * queue will execute a further Drush command. So this is Drush running Drush,
 * very meta.
 */
function drush_preview_sync_worker() {
  $pause = trim(drush_get_option('pause'));
  if (empty($pause)) {
    $pause = 10;
  }
  $repeat = trim(drush_get_option('repeat'));
  if (empty($repeat)) {
    $repeat = 17;
  }

  $stderr = fopen('php://stderr', 'w');

  // Install a signal handler to exit cleanly on TERM. Note, this requires
  // Process Control support in PHP, this is usually found in Ubuntu, but not
  // Mac OSX, because that would be too easy.
  $GLOBALS['terminate'] = false;
  if (function_exists('pcntl_fork')) {
    declare(ticks = 1);
    function term_handler($signo) {
      global $stderr;
      file_put_contents('php://stderr', 'Term caught, shutting down...' . PHP_EOL);
      $GLOBALS['terminate'] = true;
    }
    pcntl_signal(SIGTERM, 'term_handler');
    pcntl_signal(SIGINT, 'term_handler');
  }

  drush_log(dt('Attempting to get an item to process for Preview Sync.'), 'ok');

  // Attempt to claim the item from the queue.
  try {
    $output = '';
    $queue = DrupalQueue::get(PREVIEW_SYNC_QUEUE);
    $queue->createQueue();

    // Loop X times, with a pause of Y, while we have not terminated the Drush
    // command.
    fwrite($stderr, 'Processing Queue ' . format_plural($repeat, "1 time", "@count times") . "\n");
    for ($i = 0; ($i < $repeat) && !$GLOBALS['terminate']; $i++) {

      // Attempt to acquire an exclusive lock for the worker. This will
      // potentially fail if you have concurrent workers.
      // @see https://api.drupal.org/api/drupal/includes%21lock.inc/function/lock_acquire/7
      if (!lock_acquire(__FUNCTION__, PREVIEW_SYNC_QUEUE_LEASE_TIME)) {
        drush_print(dt('Worker is already currently active, only one can run at a time.'), 'error');
        sleep($pause);
      }

      // You have the lock, now look for an item in the queue that you can try
      // to process.
      else if ($item = $queue->claimItem(PREVIEW_SYNC_QUEUE_LEASE_TIME)) {
        fwrite($stderr, "{$i}. " . dt('Found job ID [@id]', array('@id' => $item->item_id)) . "\n");
        $timer = timer_start(__FUNCTION__);
        $item->data['start'] = REQUEST_TIME;

        // Sanity check, ensure that the servers have appropriate sync settings.
        // This is another safe guard to prevent syncing to production for
        // example.
        list($from, $to) = preview_sync_get_aliases();
        list($from_alias) = array_keys($from);
        list($to_alias) = array_keys($to);
        $from_alias = 'preview.' . $from_alias;
        $to_alias = 'preview.' . $to_alias;
        if ($from_alias !== $item->data['from']) {
          $msg = 'The site alias you specified for the "from" argument (!from) is not configured as a server that you can sync from.';
          $args = array('!from' => $item->data['from']);
          drush_log(dt($msg, $args), 'error');
          throw new Exception(dt($msg, $args));
        }
        if ($to_alias !== $item->data['to']) {
          $msg = 'The site alias you specified for the "to" argument (!to) is not configured as a server that you can sync to.';
          $args = array('!to' => $item->data['to']);
          drush_log(dt($msg, $args), 'error');
          throw new Exception(dt($msg, $args));
        }

        // Execute the Drush command, this is done through drush_invoke_process()
        // as this lets you use a site alias, and return the debug output for
        // later processing.
        $cmd = preview_sync_task_to_string($item->data);
        drush_log(dt('Executing the queued command "!command".', array('!command' => $cmd)), 'ok');

        // Commented out due to Drush 4.5.0 and 5.10.0 both not working
        // correctly here.
        //$output = drush_invoke_process(drush_sitealias_get_record($item->data['site']), $item->data['command'], $item->data['arguments'], $item->data['options']);

        // Run command.
        $ret = drush_shell_exec($cmd);
        $output = drush_shell_exec_output();
        $backend_output = array_pop($output);
        $parsed = drush_backend_parse_output($backend_output);

        // Release the lock, the process has now finished.
        lock_release(__FUNCTION__);

        // Log and delete the queued item as this command worked.
        if (!$ret) {
          fwrite($stderr, "{$i}. " . dt('Error processing job ID [@id]', array('@id' => $item->item_id)) . "\n");
          throw new Exception(t('Error found while processing Drush command, debug "@output"', array(
            '@output' => print_r($output, TRUE),
          )));
        }
        else {
          fwrite($stderr, "{$i}. " . dt('Successfully processed job ID [@id]', array('@id' => $item->item_id)) . "\n");
          _preview_sync_log_success($item, $backend_output, timer_read(__FUNCTION__));
          $queue->deleteItem($item);
        }
      }
      // You got the lock, but there were no items in the queue.
      else {
        sleep($pause);
      }
    }
  }
  catch (Exception $e) {
    _preview_sync_log_exception($e, $item, print_r($output, TRUE), timer_read(__FUNCTION__));
    $queue->releaseItem($item);

    // If you failed, wait some time and try again.
    sleep($pause);

    return FALSE;
  }
}

/**
 * Basic validation for the arguments to the Drush command.
 */
function drush_preview_sync_workbench_moderation_publish_validate() {
  $offset = trim(drush_get_option('offset'));
  if ($offset) {
    if (!is_numeric($offset) || $offset < 0) {
      return drush_set_error('INVALID_OFFSET_FORMAT', dt('Offset argument must be a positive integer (e.g. 10)'));
    }
  }

  $limit = trim(drush_get_option('limit'));
  if ($limit) {
    if (!is_numeric($limit) || $limit < 1) {
      return drush_set_error('INVALID_LIMIT_FORMAT', dt('Limit argument must be a positive integer (e.g. 10)'));
    }
  }
}

/**
 * Publishes content in the review state.
 */
function drush_preview_sync_workbench_moderation_publish() {
  $offset = trim(drush_get_option('offset'));
  if (empty($offset)) {
    $offset = 0;
  }
  $limit = trim(drush_get_option('limit'));
  if (empty($limit)) {
    $limit = 10000;
  }

  $nodes = preview_sync_get_review_content($offset, $limit);
  foreach ($nodes as $node) {
    $revision = node_load($node['nid'], $node['vid']);
    $revision->status = 1;
    $revision->workbench_moderation_state_new == workbench_moderation_state_published();
    node_save($revision);
  }
}
