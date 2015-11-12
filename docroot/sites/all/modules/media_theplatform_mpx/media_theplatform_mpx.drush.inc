<?php

/**
 * @file
 * Drush integration for the media_theplatform_mpx module.
 */

define('MEDIA_THEPLATFORM_MPX_VIDEO_INGESTION_LOCK_BASENAME', 'drush_media_theplatform_mpx_ingest_mpx_videos');

// 870 s are 14 min 30 sec
define('MTPM_HANDLER_TIMEOUT', 870.0);
define('MTPM_LOCK_TIMEOUT', 60.0);
define('MTPM_PARENT_LOCK_TIMEOUT', 900.0);
define('MTPM_REQUEST_ITEM_LIMIT', 25);
define('MTPM_PROCESS_TIMEOUT', 300.0);
define('MTPM_REQUEST_TIMEOUT', 60.0);
define('MTPM_REQUEST_TIMEOUT_SHORT', 30.0);
define('MTPM_TIMEOUT_DELTA', 0.1);

/**
 * Implements hook_drush_command().
 */
function media_theplatform_mpx_drush_command() {
  $commands = array();

  $commands['mpx-account-list'] = array(
    'description' => dt('List key MPX account data.'),
    'aliases' => array('mpxal'),
  );

  $ingest_options = array(
    'all' => dt('If provided will attempt to ingest all possible items. Useful for initial ingestion or catching up when behind on notifications. If provided will also set --queue-run.'),
    'limit' => dt('The number of new or updated items to process from the feed at once. The default is 100.'),
    'queue-run' => dt('Will run the process queue after ingestion.'),
    'force' => dt('If provided will break any process locks.'),
    'reset' => dt('If provided will start ingestion from the beginning again.'),
    'queue-concurrency' => dt('The number of concurrent queue workers. Default is 10.'),
  );

  $commands['mpx-ingest'] = array(
    'description' => dt('Ingest videos for a specific mpx account'),
    'aliases' => array('mpxi'),
    'arguments '=> array(
      'account_id' => dt('The ID of the account to import.'),
    ),
    'options' => $ingest_options,
    'examples' => array(
      'drush mpx-ingest 1' => dt('Ingest videos for mpx account 1.'),
      'drush mpxi 1 --limit=100' => dt('Ingest up to 100 new and 100 updated videos for mpx account 1.'),
    ),
  );
  $commands['mpx-ingest-all'] = array(
    'description' => dt('Ingest videos for all mpx accounts'),
    'aliases' => array('mpxia'),
    'options' => $ingest_options,
    'examples' => array(
      'drush mpx-ingest-all' => dt('Ingest videos for each mpx account.'),
      'drush mpxia --limit=100' => dt('Ingest up to 100 new and 100 updated videos for each mpx accounts.'),
    ),
  );
  $commands['mpx-queue-list'] = array(
    'description' => 'List queues for an mpx account.',
    'aliases' => array('mpxql'),
    'arguments' => array(
      'account_id' => 'The ID of the mpx account.',
    ),
    'outputformat' => array(
      'default' => 'table',
      'pipe-format' => 'csv',
      'field-labels' => array(
        'queue' => 'Queue',
        'items' => 'Items',
      ),
      'ini-item' => 'items',
      'table-metadata' => array(
        'key-value-item' => 'items',
      ),
      'output-data-type' => 'format-table',
    ),
  );
  $commands['mpx-queue-run'] = array(
    'description' => 'Run queues for an mpx account.',
    'aliases' => array('mpxqr'),
    'arguments '=> array(
      'account_id' => 'The ID of the mpx account.',
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'limit' => 'The number of video queue items to process before running the other queues again.'
    ),
  );

  $commands['ingest-mpx-videos'] = array(
    'description' => dt('Runs a continuous mpx video ingestion process.'),
    'hidden' => TRUE,
    'aliases' => array('mpxv'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
    'arguments' => array(
      'account_id' => dt('The ID of the account to ingest videos for.  DEFAULT: all'),
    ),
    'options' => array(
      'force' => array(
        'description' => dt('Force the ingestion process to run even if other ingestion processes are currently running.  WARNING: Forcing this command to run while other mpx video ingestion processes are active may, and likely will, have unwanted side-effects.') . "\n",
        'example_value' => '1',
        'value' => 'optional',
      ),
      'quit-when-complete' => array(
        'description' => dt('Have the command exit once all video assets have been processed.  Otherwise, it will run indefinitely listening for updates as they are returned from thePlatform.') . "\n",
        'example_value' => '1',
        'value' => 'optional',
      ),
      'process-count' => array(
        'description' => dt('The number of ingestion processes that will be run concurrently.  IMPORTANT NOTE: This value should be a multiple of 2, a "retriever" process that retrieves video data from thePlatform and a "worker" process that creates/updates/deletes videos.') . "\n",
        'example_value' => 6,
        'value' => 'optional',
      ),
      'x-account-id' => array(
        'description' => dt('Internal use only.  Used when invoking child processes.'),
        'value' => 'optional',
      ),
      'x-parent-id' => array(
        'description' => dt('Internal use only.  Used when invoking child processes.'),
        'value' => 'optional',
      ),
      'x-child-id' => array(
        'description' => dt('Internal use only.  Used when invoking child processes.'),
        'value' => 'optional',
      ),
      'x-child-type' => array(
        'description' => dt('Internal use only.  Used when invoking child processes.'),
        'value' => 'optional',
      ),
      'x-sibling-count' => array(
        'description' => dt('Internal use only.  Used when invoking child processes.'),
        'value' => 'optional',
      ),
      'x-max-processing-time' => array(
        'description' => dt('Internal use only.  Used when invoking child processes.'),
        'value' => 'optional',
      ),
    ),
    'examples' => array(
      'drush ingest-mpx-videos' => dt('Ingest videos for all configured accounts.'),
      'drush mpxv' => dt('Ingest videos for all configured accounts.'),
      'drush mpxv all' => dt('Ingest videos for all configured accounts.'),
      'drush mpxv 1' => dt('Ingest videos for account 1.'),
      'drush mpxv --force=1 1' => dt('Ingest videos for account 1, even if other ingestion processes are currently running.'),
      'drush mpxv --quit-when-complete=1' => dt('Ingest videos for all configured accounts, stopping after all videos and video updates have been ingested.'),
      'drush mpxv --process-count=6' => dt('Ingest videos for all configured accounts, limiting the number of concurrent retriever and worker processes across accounts to 6.  However, if there are 4 accounts configured, the process limit will automatically be raised to 8, so that a retriever and worker process can be invoked for each account.  If there are 5 configured accounts, the process limit is raised to 10.  And so on.'),
    ),
  );

  return $commands;
}

/**
 * Command callback for drush mpx-account-list.
 */
function drush_media_theplatform_mpx_mpx_account_list() {
  $accounts = MpxAccount::loadAll();

  if (empty($accounts)) {
    return drush_print(dt("No mpx accounts available."));
  }

  $rows = array();
  $rows[] = array(
    dt('Id'),
    dt('Username'),
    dt('Import account'),
    dt('Imported videos'),
  );
  foreach ($accounts as $account) {
    $row = array(
      $account->id,
      $account->username,
    );

    if (!empty($account->import_account)) {
      $row[] = $account->import_account;
    }
    else {
      $row[] = dt('Not configured');
    }

    // Imported video count.
    $row[] = db_query("SELECT COUNT(fid) FROM {mpx_video} WHERE parent_account = :id", array(':id' => $account->id))->fetchField();

    $rows[] = $row;
  }

  drush_print_table($rows, TRUE);
}

/**
 * Validation callback for drush mpx-ingest.
 */
function drush_media_theplatform_mpx_mpx_ingest_validate($account_id) {
  if (!MpxAccount::load($account_id)) {
    return drush_set_error('MPX_ACCOUNT_NOT_FOUND', dt("Unable to load mpx account @id.", array('@id' => $account_id)));
  }

  // Check for locks used by drush ingest-mpx-videos.
  if (!drush_get_option('force') && !lock_may_be_available(MEDIA_THEPLATFORM_MPX_VIDEO_INGESTION_LOCK_BASENAME . '_parent_' . $account_id)) {
    return drush_set_error('MPX_INGEST_LOCK_ERROR', dt("Drush video ingestion is currently running for mpx account @id.", array('@id' => $account_id)));
  }
}

/**
 * Command callback for drush mpx-ingest.
 *
 * @param int $account_id
 *   The mpx account ID.
 */
function drush_media_theplatform_mpx_mpx_ingest($account_id) {
  try {
    $account = MpxAccount::load($account_id);

    if (drush_get_option('reset')) {
      if (drush_confirm(dt('Are you sure you want to reset video ingestion for @account?', array('@account' => (string) $account)))) {
        $account->resetIngestion();
        drush_log(dt('Reset video ingestion for @account', array('@account' => (string) $account)), 'ok');
      }
    }

    $options = array(
      'method' => 'via Drush',
      'limit' => (int) drush_get_option('limit', variable_get('media_theplatform_mpx__cron_videos_per_run', 100)),
      'force' => (bool) drush_get_option('force'),
      'all' => (bool) drush_get_option('all'),
    );
    $summary = $account->ingestVideos($options);
    $lines = explode('<br>', format_string($summary['message'], $summary['args']));
    foreach ($lines as $line) {
      drush_log($line, 'ok');
    }

    if ($options['all']) {
      // Convert the current batch to the request queue.
      MpxRequestQueue::populateBatchItems($account, $options['limit']);
    }

    // Show the summary of the account's queues.
    drush_invoke_process('@self', 'mpx-queue-list', array($account_id));

    if (drush_get_option('queue-run', $options['all'])) {
      // Ensure that the concurrency is not higher than the maximum number of
      // items in the queues for the account.
      $queues = drush_media_theplatform_mpx_mpx_queue_list($account_id);
      $max_items = 0;
      foreach ($queues as $queue) {
        $max_items = max($max_items, $queue['items']);
      }
      $concurrency = (int) drush_get_option('queue-concurrency', 10);
      $concurrency = min($concurrency, $max_items);

      if ($concurrency) {
        $invocations = array_fill(0, $concurrency, array(
          'site' => '@self',
          'command' => 'mpx-queue-run',
          'args' => array($account_id),
          'options' => array('limit' => $options['limit']),
        ));
        drush_backend_invoke_concurrent(
          $invocations,
          array(),
          array(
            'concurrency' => $concurrency,
          )
        );

        // Force the database connection to be reconnected since the queues mau
        // have run for an extended amount of time.
        Database::closeConnection();
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('media_theplatform_mpx', $e);
    return drush_set_error('MPX_INGEST_EXCEPTION', $e->getMessage());
  }
}

/**
 * Validation callback for drush mpx-ingest-all.
 */
function drush_media_theplatform_mpx_mpx_ingest_all_validate() {
  if (!MpxAccount::loadAll()) {
    return drush_set_error('MPX_NO_ACCOUNTS', "No mpx accounts available.");
  }

  // Check for locks used by drush ingest-mpx-videos.
  if (!drush_get_option('force') && !lock_may_be_available('media_theplatform_mpx_ingest_videos_all')) {
    return drush_set_error('MPX_INGEST_LOCK_ERROR', dt("Drush video ingestion is currently running for all accounts."));
  }
}

/**
 * Command callback for drush mpx-ingest-all.
 */
function drush_media_theplatform_mpx_mpx_ingest_all() {
  $accounts = MpxAccount::loadAll();
  $invocations = array();
  foreach ($accounts as $account) {
    $invocations[] = array(
      'site' => '@self',
      'command' => 'mpx-ingest',
      'args' => array($account->id),
    );
  }
  $common_options = drush_redispatch_get_options();
  $common_backend_options = array(
    'concurrency' => count($invocations),
  );
  drush_backend_invoke_concurrent($invocations, $common_options, $common_backend_options);
}

/**
 * Validation callback for drush mpx-queue-list.
 */
function drush_media_theplatform_mpx_mpx_queue_list_validate($account_id = NULL) {
  if (isset($account_id) && !MpxAccount::load($account_id)) {
    return drush_set_error('MPX_ACCOUNT_NOT_FOUND', dt("Unable to load mpx account @id.", array('@id' => $account_id)));
  }
}

/**
 * Command callback for drush mpx-queue-list.
 */
function drush_media_theplatform_mpx_mpx_queue_list($account_id = NULL) {
  $queues = media_theplatform_mpx_cron_queue_info($account_id);
  $result = array();
  foreach (array_keys($queues) as $queue_name) {
    $queue = DrupalQueue::get($queue_name, TRUE);
    $result[$queue_name] = array(
      'queue' => $queue_name,
      'items' => $queue->numberOfItems(),
    );
  }
  return $result;
}

/**
 * Validation callback for drush mpx-queue-run.
 */
function drush_media_theplatform_mpx_mpx_queue_run_validate($account_id) {
  if (!MpxAccount::load($account_id)) {
    return drush_set_error('MPX_ACCOUNT_NOT_FOUND', dt("Unable to load mpx account @id.", array('@id' => $account_id)));
  }
}

/**
 * Command callback for drush mpx-queue-run.
 */
function drush_media_theplatform_mpx_mpx_queue_run($account_id) {
  $account = MpxAccount::load($account_id);

  $queues = media_theplatform_mpx_cron_queue_info($account_id);
  $request_queue = MpxRequestQueue::get($account);
  /** @var DrupalReliableQueueInterface $video_queue */
  $video_queue = DrupalQueue::get('media_theplatform_mpx_video_cron_queue', TRUE);
  $video_queue_run_limit = (int) drush_get_option('limit', variable_get('media_theplatform_mpx__cron_videos_per_run', 100));

  // Increase the execution time limit to one hour.
  if (function_exists('set_time_limit')) {
    $current = ini_get('max_execution_time');
    // Do not set time limit if it is currently unlimited.
    if ($current != 0) {
      @set_time_limit(3600);
    }
  }

  // @todo Should attempt to reclaim stalled queue items at some point.

  do {
    // Run one item from the request queue if there is one.
    if ($item = $request_queue->claimItem()) {
      try {
        drush_log("Starting request queue item $item->item_id.", 'info');
        // @todo This queue callback is hardcoded.
        MpxRequestQueue::processItem($item->data);
        $request_queue->deleteItem($item);
        drush_log("Finished request queue item $item->item_id.", 'info');
      }
      catch (Exception $e) {
        watchdog_exception('media_theplatform_mpx', $e);
        drush_set_error('MPX_QUEUE_EXCEPTION', $e->getMessage());
      }
    }

    // Force the database connection to be reconnected since each request queue
    // item generally may take a long time.
    Database::closeConnection();

    // Process up to $video_queue_limit items before looping back and working
    // on the next request queue item. If the request queue no longer contains
    // any items, keep processing video queue items until the queue is empty.
    $video_count = 0;
    while ($item = $video_queue->claimItem()) {
      try {
        drush_log(dt("Processing video queue item $item->item_id"), 'info');
        $video_count++;
        // @todo This queue callback is hardcoded.
        process_media_theplatform_mpx_video_cron_queue_item($item->data);
        $video_queue->deleteItem($item);
        if ($video_count >= $video_queue_run_limit && $request_queue->numberOfItems()) {
          break;
        }
      }
      catch (Exception $e) {
        watchdog_exception('media_theplatform_mpx', $e);
        drush_set_error('MPX_QUEUE_EXCEPTION', $e->getMessage());
      }
    }
    if ($video_count) {
      drush_log("Processed $video_count video queue items.", 'ok');
    }

    // @todo This assumes that the queues use the SystemQueue class.
    $claimable_items = db_query('SELECT COUNT(item_id) FROM {queue} WHERE name IN (:names) AND expire = 0', array(':names' => array_keys($queues)))->fetchField();
  } while ($claimable_items);

  /*foreach (array_keys($queues) as $queue_name) {
    if (DrupalQueue::get($queue_name)->numberOfItems()) {
      //drush_invoke_process('@self', 'queue-run', array($queue_name));
      drush_invoke('queue-run', array($queue_name));
      // Force the database connection to be reconnected since the queue may
      // have run for an extended amount of time.
      Database::closeConnection();
    }
  }*/
}

/**
 * Command validation handler for the `drush ingest-mpx-videos` command.
 */
function drush_media_theplatform_mpx_ingest_mpx_videos_validate($account_id = 'all') {

  $forced_to_run = drush_get_option('force');

  // Make sure the site is installed.
  if (!db_table_exists('queue') || !db_table_exists('mpx_video') || !db_table_exists('variable')) {
    return _drush_media_theplatform_mpx_print('Site is not installed - queue, mpx_video or variable table does not exist.  Aborting mpx video ingestion.',
      array(), 'error');
  }

  // Return an error if no accounts are configured.
  $account_data = $account_id == 'all' ? MpxAccount::loadAll() : MpxAccount::load($account_id);

  if (empty($account_data)) {
    return _drush_media_theplatform_mpx_print('There are no mpx accounts configured for this site.  Aborting mpx video ingestion.',
      array(), 'error');
  }

  // Do not run CLI ingestion if cron video ingestion is enabled.
  if (!$forced_to_run && variable_get('media_theplatform_mpx__cron_videos', 1)) {
    return _drush_media_theplatform_mpx_print('MPX cron video ingestion is enabled.  Aborting drush mpx video ingestion.',
      array(), 'error');
  }

  // Special validation handling when processing all accounts.
  if ($account_id == 'all') {

    // Check if an ingestion process is already running for all accounts.
    if (!$forced_to_run && !lock_acquire('media_theplatform_mpx_ingest_videos_all')) {
      return _drush_media_theplatform_mpx_print('MPX video ingestion already running for all accounts.  Aborting.',
        array(), 'error');
    }

    if (!$forced_to_run) {
      foreach ($account_data as $id => $account) {
        // If a parent ingestion process is running for any account, return an error.
        if (!lock_may_be_available('media_theplatform_mpx_ingest_videos_' . $id)) {
          return _drush_media_theplatform_mpx_print('MPX video ingestion already running for account @id.  Aborting mpx video ingestion for all accounts.',
            array('@id' => $id), 'error');
        }
        // Throw an error if any of the account data is incorrect or absent.
        if (empty($account->import_account)) {
          return _drush_media_theplatform_mpx_print('Import account not set for account @id.  Aborting mpx video ingestion for all accounts.',
            array('@id' => $id), 'error');
        }
        if (!$account->acquireToken()) {
          return _drush_media_theplatform_mpx_print('MPX authentication token invalid for account @id.  Aborting mpx video ingestion for all accounts.',
            array('@id' => $id), 'error');
        }
        if (!media_theplatform_mpx_is_valid_player_for_account($account->default_player, $account)) {
          return _drush_media_theplatform_mpx_print('Default player invalid for account @id.  Aborting mpx video ingestion for all accounts.',
            array('@id' => $id), 'error');
        }
      }
    }

    return TRUE;
  }

  // Return an error if an mpx video ingestion for all accounts is running.
  if (!$forced_to_run && !lock_may_be_available('media_theplatform_mpx_ingest_videos_all')) {
    return _drush_media_theplatform_mpx_print('MPX video ingestion already running for all accounts.  Aborting mpx video ingestion for account @id.',
      array('@id' => $account_id), 'error');
  }

  // If a parent ingestion process is running for this account, return an error.
  if (!$forced_to_run && $account_id && !lock_may_be_available('media_theplatform_mpx_ingest_videos_' . $account_id)) {
    return _drush_media_theplatform_mpx_print('MPX video ingestion already running for account @id.  Aborting mpx video ingestion.',
      array('@id' => $account_id), 'error');
  }

  // Throw an error if any of the account data is incorrect or absent.
  if (!$forced_to_run && empty($account_data->import_account)) {
    return _drush_media_theplatform_mpx_print('Import account not set for account @id.  Aborting mpx video ingestion.',
      array('@id' => $id), 'error');
  }
  if (!$forced_to_run && !$account_data->acquireToken()) {
    return _drush_media_theplatform_mpx_print('MPX authentication token invalid for account @id.  Aborting mpx video ingestion.',
      array('@id' => $id), 'error');
  }
  if (!$forced_to_run && !media_theplatform_mpx_is_valid_player_for_account($account_data->default_player, $account_data)) {
    return _drush_media_theplatform_mpx_print('Default player invalid for account @id.  Aborting mpx video ingestion.',
      array('@id' => $id), 'error');
  }

  return TRUE;
}

/**
 * Command handler for the `drush ingest-mpx-videos` command.
 */
function drush_media_theplatform_mpx_ingest_mpx_videos($account_id = 'all') {

  global $process_metadata;

  $start_time = time();

  // Set the MySQL wait_timeout variable to 30 minutes.
  db_query('SET interactive_timeout = 1800');
  db_query('SET wait_timeout = 1800');

  // If the memory limit is set to less than 256M, increase it.
  $memory_limit = ini_get('memory_limit');
  // A memory_limit of -1 is unlimited.
  if ($memory_limit != -1) {
    $memory_limit_in_bytes = _media_theplatform_mpx_convert_shorthand_to_bytes($memory_limit);
    if ($memory_limit_in_bytes < 536870912) {
      ini_set('memory_limit', '512M');
    }
  }

  $parent_id = drush_get_option('x-parent-id');

  // A parent ID has been passed, so this is a child process.
  if ($parent_id) {
    return _drush_media_theplatform_mpx_start_child_process();
  }

  // If no parent ID is passed as a command option, this process should serve
  // as the top-level parent process that invokes the account handler processes,
  // which in-turn invoke the retriever and worker processes.

  // Acquire a lock to identify this as a parent process.
  $process_metadata = _drush_media_theplatform_mpx_acquire_parent_process_lock($account_id);

  // If a lock could not be acquired, bail.
  if (empty($process_metadata)) {
    return _drush_media_theplatform_mpx_print('Aborting video ingestion.', array(), 'error');
  }

  // Announce what this process will be doing, and get the relevant account data.
  if ($account_id == 'all') {

    $account_data = MpxAccount::loadAll();
    _drush_media_theplatform_mpx_print('Ingesting mpx videos for all configured accounts.');

    // Invoke the account handler processes that will be doing all of the work.
    _drush_media_theplatform_mpx_invoke_account_handler_processes($account_data);
  }
  else {

    $account_data = MpxAccount::load($account_id);
    $parent_lock_timeout = MTPM_PARENT_LOCK_TIMEOUT + MTPM_TIMEOUT_DELTA;
    $quit_when_complete = drush_get_option('quit-when-complete');

    _drush_media_theplatform_mpx_print('Ingesting mpx videos for account @id "@name".',
      array('@id' => $account_id, '@name' => $account_data->import_account));

    do {

      if (!_drush_media_theplatform_mpx_check_process_health($parent_lock_timeout)) {
        return _drush_media_theplatform_mpx_print('Aborting account handling.', array(), 'warning');
      }

      // Call the account handler function that will be doing all of the work.
      _drush_media_theplatform_mpx_process_video_ingestion_as_account_handler($account_data);
    }
    while (!$quit_when_complete);
  }

  lock_release($process_metadata['lock_name']);

  return _drush_media_theplatform_mpx_print('*** MPX video ingestion ran for @time seconds. ***',
    array('@time' => (time() - $start_time)));
}

/**
 * Helper that generates prefix for process messages.
 */
function _drush_media_theplatform_mpx_print_prefix() {

  global $process_metadata;

  $prefix = '';

  if ($parent_id = drush_get_option('x-parent-id')) {
    $child_id = drush_get_option('x-child-id', t('[Unknown ID]'));
    $child_type = drush_get_option('x-child-type', '[Unknown Type]');
    $prefix = dt('@type: child @child_id of parent @parent_id: ',
      array(
        '@type' => $child_type,
        '@child_id' => $child_id,
        '@parent_id' => $parent_id,
      ))
      . $prefix;
  }
  else {
    $prefix = dt('PARENT PROCESS: ') . $prefix;
  }

  if ($account_id = drush_get_option('x-account-id')) {
    $account = MpxAccount::load($account_id);
    $prefix = dt('Account @id "@name": ',
      array(
        '@id' => $account_id,
        '@name' => $account->import_account,
      ))
      . $prefix;
  }

  return $prefix;
}

/**
 * Helper for outputting messages, automatically prefixing messages with
 * process information.
 */
function _drush_media_theplatform_mpx_print($message, $message_variables = array(), $type = 'status') {

  $prefix = &drupal_static(__FUNCTION__, _drush_media_theplatform_mpx_print_prefix());

  switch ($type) {

    case 'error':
      // watchdog() will output the error to the console for us.
      watchdog('drush_media_theplatform_mpx', $prefix . $message, $message_variables, WATCHDOG_ERROR);
      return FALSE;

    case 'warning':
      // watchdog() will output the error to the console for us.
      watchdog('drush_media_theplatform_mpx', $prefix . $message, $message_variables, WATCHDOG_WARNING);
      return FALSE;

    default:
      drush_print($prefix . dt($message, $message_variables), 2);
      return TRUE;
  }
}

/**
 * Helper function that assigns a lock name to a parent process, and acquires
 * the corresponding lock to maintain while the process is running.
 */
function _drush_media_theplatform_mpx_acquire_parent_process_lock($account_id, $lock_timeout = MTPM_PARENT_LOCK_TIMEOUT) {

  $process_metadata = array();

  $base_lock_name = MEDIA_THEPLATFORM_MPX_VIDEO_INGESTION_LOCK_BASENAME . '_parent_' . $account_id;
  $max_processing_time = 3500.0;

  // If this process is forced to run, append a sub-ID to the process lock name.
  // Arbitrary maximum of 1024 processes.  Anything more than that, and
  // something is probably wrong and the process shouldn't run.
  if (lock_acquire($base_lock_name, $lock_timeout)) {
    $process_metadata = array(
      'type' => 'parent',
      'id' => $account_id,
      'lock_name' => $base_lock_name,
      'max_processing_time' => $max_processing_time,
    );
  }
  elseif (drush_get_option('force')) {
    for ($i = 1; $i <= 1024; $i++) {
      $lock_name = $base_lock_name . '_' . $i;
      if (lock_acquire($lock_name, $lock_timeout)) {
        $process_metadata = array(
          'type' => 'parent',
          'id' => $account_id . '-' . $i,
          'lock_name' => $lock_name,
          'max_processing_time' => $max_processing_time,
        );
        break;
      }
    }
  }

  if (!empty($process_metadata['lock_name'])) {
    _drush_media_theplatform_mpx_print('@time: Lock "@lock" acquired for parent process for @timeout seconds.',
      array('@time' => time(), '@lock' => $process_metadata['lock_name'], '@timeout' => $lock_timeout));
  }
  else {
    _drush_media_theplatform_mpx_print('@time: Failed to acquire lock for parent process: @lock',
      array('@time' => time(), '@lock' => $base_lock_name), 'error');
  }

  return $process_metadata;
}

/**
 * thePlatform only stores notification sequence IDs for 7 days.  After that
 * period, they are expired and no longer any use.  If a notification
 * sequence ID is expired, reset it to NULL for this account, so that
 * a complete re-ingestion can be performed.
 */
function _drush_media_theplatform_mpx_check_notification_sequence_id(MpxAccount $account) {

  $token = $account->acquireToken();
  $url = 'https://read.data.media.theplatform.com/media/notify?token=' . $token
    . '&account=' . rawurlencode($account->import_account)
    . '&clientId=drupal_media_theplatform_mpx_reports_' . $account->account_pid
    . '&since=' . $account->getDataValue('last_notification')
    . '&block=false&filter=Media';

  $response = _media_theplatform_mpx_retrieve_feed_data($url);

  if (isset($response[0]['type']) && $response[0]['type'] == 'Exception') {

    $account->setDataValue('last_notification', NULL);

    return _drush_media_theplatform_mpx_print('Last notification sequence ID is invalid.  Setting last_notification value to NULL to perform a complete re-ingestion for this account.', array(), 'error');;
  }

  return TRUE;
}

/**
 * Invoke account handler child processes.
 */
function _drush_media_theplatform_mpx_invoke_account_handler_processes(array $account_data) {

  global $process_metadata;

  $start_time = time();

  $command_args = drush_get_arguments();
  array_shift($command_args);
  $process_count = drush_get_option('process-count', 10);
  $quit_when_complete = drush_get_option('quit-when-complete');

  // Set processing times for this parent process and each account handler.
  // Add 0.1 to avoid a bug due to microtime()'s lack of fine precision on
  // (some of) Acquia's servers.
  $parent_lock_timeout = MTPM_PARENT_LOCK_TIMEOUT + MTPM_TIMEOUT_DELTA;
  $child_max_processing_time = MTPM_HANDLER_TIMEOUT;

  $invocation_command = array(
    'site' => '@self',
    'command' => 'mpxv',
    'args' => array(),
    'options' => array(
      'x-child-type' => 'account_handler',
      'x-parent-id' => $process_metadata['id'],
      'x-sibling-count' => count($account_data),
      'x-max-processing-time' => $child_max_processing_time,
      'process-count' => $process_count,
      'quit-when-complete' => (int) $quit_when_complete,
      'force' => 1,
    ),
  );

  $invocations = array();
  $lock_id_counter = 0;

  foreach ($account_data as $account) {
    $command = $invocation_command;
    $command['options']['x-child-id'] = ++$lock_id_counter;
    $command['options']['x-account-id'] = $account->id;
    $invocations[] = $command;
  }

  do {

    $time = time();

    if (($time - $start_time) >= $process_metadata['max_processing_time']) {
      return _drush_media_theplatform_mpx_print('@time: Parent process has run over its maximum runtime of @runtime seconds.  Stopping process to be restarted.',
        array('@time' => $time, '@runtime' => $process_metadata['max_processing_time']));
    }

    if (!lock_acquire($process_metadata['lock_name'], $parent_lock_timeout)) {
      return _drush_media_theplatform_mpx_print('Unable to (re-)acquire lock "@lock" for parent process.  Aborting video ingestion.',
        array('@lock' => $process_metadata['lock_name']), 'error');
    }

    // Check that cron ingestion hasn't been re-enabled.  The database has
    // to be queried directly, as the system variable will just pull from the
    // static $conf variable.
    $cron_ingestion_enabled = db_select('variable', 'v')
      ->fields('v', array('value'))
      ->condition('name', 'media_theplatform_mpx__cron_videos', '=')
      ->execute()
      ->fetchField();
    $cron_ingestion_enabled = unserialize($cron_ingestion_enabled);
    if ($cron_ingestion_enabled) {
      return _drush_media_theplatform_mpx_print('Cron ingestion has been enabled.  Aborting child retriever and worker process invocation.', array(), 'error');
    }

    $message_variables = array(
      '@time' => $time,
      '@lock_name' => $process_metadata['lock_name'],
      '@lock_timeout' => $parent_lock_timeout,
    );
    _drush_media_theplatform_mpx_print('@time: Lock "@lock_name" acquired for parent process for @lock_timeout seconds.',
      $message_variables);
    _drush_media_theplatform_mpx_print('Invoking @count child account handler processes.',
      array('@count' => count($invocations)));

    drush_backend_invoke_concurrent($invocations, array(), array('concurrency' => count($invocations)));
  }
  while (!$quit_when_complete);
}

/**
 * Invoke the processes that retrieve video data from thePlatform and process
 * the cron queue items created from this retrieved data.
 */
function _drush_media_theplatform_mpx_invoke_retriever_and_worker_processes(MpxAccount $account, $max_processing_time = MTPM_PROCESS_TIMEOUT, $request_item_limit = NULL) {

  global $process_metadata;

  if (!$request_item_limit) {
    $request_item_limit = variable_get('media_theplatform_mpx__drush_video_ingestion_request_item_limit', 100);
  }

  // Ensure the batch and cron queues exist, and initialize them.
  DrupalQueue::get('media_theplatform_mpx_video_request_queue_' . $account->id)->createQueue();
  $request_queue = DrupalQueue::get('media_theplatform_mpx_video_request_queue_' . $account->id);
  DrupalQueue::get('media_theplatform_mpx_video_cron_queue')->createQueue();
  $cron_queue = DrupalQueue::get('media_theplatform_mpx_video_cron_queue');

  $command_args = drush_get_arguments();
  array_shift($command_args);
  $account_id = drush_get_option('x-account-id', 1);

  // Vary the number of invoked processes by the number of queue items.
  $request_queue_item_count = $request_queue->numberOfItems();
  $cron_queue_item_count = $cron_queue->numberOfItems();
  $max_process_count = drush_get_option('process-count', 10);
  // If the max process count is already at the minimum, that's what it has to be.
  if ($max_process_count == 2) {
    $process_count = $max_process_count;
  }
  // Often when change notifications are being processed, there will only be
  // a single request queue item created.  Prevent the full gamut of retriever
  // and worker processes from being invoked, if there isn't that much to do.
  // e.g. if ($request_queue_item_count == 2 && $cron_queue_item_count <= (2 * $request_item_limit))
  elseif ($cron_queue_item_count <= ($request_queue_item_count * $request_item_limit)
      && ($request_queue_item_count * 2) < $max_process_count) {
    $process_count = $request_queue_item_count * 2;
  }
  elseif ($cron_queue_item_count <= ((2 * $request_queue_item_count) * $request_item_limit)
      && ($request_queue_item_count * 4) < $max_process_count) {
    $process_count = $request_queue_item_count * 4;
  }
  elseif ($cron_queue_item_count <= ((3 * $request_queue_item_count) * $request_item_limit)
      && ($request_queue_item_count * 6) < $max_process_count) {
    $process_count = $request_queue_item_count * 6;
  }
  // If there is a lot to be done, simply call the full gamut.
  else {
    $process_count = $max_process_count;
  }

  $invocation_command = array(
    'site' => '@self',
    'command' => 'mpxv',
    'args' => $command_args,
    'options' => array(
      'x-parent-id' => !empty($process_metadata['id']) ? $process_metadata['id'] : 0,
      'x-account-id' => $account->id,
      'x-sibling-count' => $process_count,
      'x-max-processing-time' => (int) $max_processing_time,
      'process-count' => $process_count,
      'quit-when-complete' => 1,
      'force' => 1,
    ),
  );

  $invocations = array();
  $lock_id_counter = 0;

  for ($i = 0; $i < ceil($process_count / 2); $i++) {
    foreach (array('retriever', 'worker') as $type) {
      $command = $invocation_command;
      $command['options']['x-child-id'] = ++$lock_id_counter;
      $command['options']['x-child-type'] = $type;
      $invocations[] = $command;
    }
  }

  while ($request_queue->numberOfItems() || $cron_queue->numberOfItems()) {

    if (!_drush_media_theplatform_mpx_check_process_health($max_processing_time * 1.025)) {
      return _drush_media_theplatform_mpx_print('Aborting child retriever and worker process invocation.', array(), 'warning');
    }

    _drush_media_theplatform_mpx_print('Invoking @count child retriever and worker processes to process request and cron queue items.',
      array('@count' => count($invocations)));
    drush_backend_invoke_concurrent($invocations, array(), array('concurrency' => count($invocations)));
  }

  return TRUE;
}

/**
 * Calls process handler function for the appropriate child process type, as well
 * as handles the lock for the child process.
 */
function _drush_media_theplatform_mpx_start_child_process() {

  global $process_metadata;

  $parent_id = drush_get_option('x-parent-id');
  $child_id = drush_get_option('x-child-id');
  $child_type = drush_get_option('x-child-type');
  $account_id = drush_get_option('x-account-id');
  $process_count = drush_get_option('process-count', 10);
  $sibling_count = drush_get_option('x-sibling-count', $process_count);
  $max_processing_time = drush_get_option('x-max-processing-time', 90.0);
  $initial_lock_timeout = MTPM_LOCK_TIMEOUT;

  if (!$parent_id) {
    return _drush_media_theplatform_mpx_print('Unable to determine parent ID.  Aborting child process.', array(), 'error');
  }
  if (!$child_id) {
    return _drush_media_theplatform_mpx_print('Unable to determine child ID.  Aborting child process.', array(), 'error');
  }
  if (!$child_type) {
    return _drush_media_theplatform_mpx_print('Unable to determine child type.  Aborting child process.', array(), 'error');
  }
  if (!$account_id) {
    return _drush_media_theplatform_mpx_print('Unable to determine account ID.  Aborting child process.', array(), 'error');
  }

  $account = MpxAccount::load($account_id);

  $child_lock_name = MEDIA_THEPLATFORM_MPX_VIDEO_INGESTION_LOCK_BASENAME . '_' . $parent_id . '_' . $child_id;

  if (!lock_acquire($child_lock_name, $initial_lock_timeout)) {
    $message_variables = array(
      '@time' => time(),
      '@name' => $child_lock_name,
      '@type' => $child_type,
    );
    return _drush_media_theplatform_mpx_print('@time: Unable to acquire lock "@name".  Aborting @type process.',
      $message_variables, 'error');
  }

  $message_variables = array(
    '@time' => time(),
    '@name' => $child_lock_name,
    '@timeout' => $initial_lock_timeout,
  );
  _drush_media_theplatform_mpx_print('@time: Successfully acquired lock "@name" for @timeout seconds.', $message_variables);

  $process_metadata = array(
    'type' => 'child',
    'parent_id' => $parent_id,
    'child_type' => $child_type,
    'child_id' => $child_id,
    'account' => $account,
    'lock_name' => $child_lock_name,
    'process_count' => $process_count,
    'sibling_count' => $sibling_count,
    'max_processing_time' => $max_processing_time,
  );

  $function = '_drush_media_theplatform_mpx_process_video_ingestion_as_' . $child_type;

  if (!function_exists($function)) {
    return _drush_media_theplatform_mpx_print('Unable to find child process function.  Aborting.', array(), 'error');
  }

  $function($account);

  lock_release($child_lock_name);

  return _drush_media_theplatform_mpx_print('@time: Released lock "@name".  Ending process.',
    array('@time' => time(), '@name' => $child_lock_name));
}

/**
 * Checks that all sibling processes are still running.  If this fails, the
 * assumption is that the current process should shutdown.  It then checks
 * that the current child process is able to re-acquire its own lock.
 */
function _drush_media_theplatform_mpx_check_process_health($lock_timeout = MTPM_LOCK_TIMEOUT) {

  global $process_metadata;

  static $start_time = NULL;

  if (!$start_time) {
    $start_time = time();
  }

  // Check that cron ingestion hasn't been re-enabled.  The database has
  // to be queried directly, as the system variable will just pull from the
  // static $conf variable.
  $cron_ingestion_enabled = db_select('variable', 'v')
    ->fields('v', array('value'))
    ->condition('name', 'media_theplatform_mpx__cron_videos', '=')
    ->execute()
    ->fetchField();
  $cron_ingestion_enabled = unserialize($cron_ingestion_enabled);
  if ($cron_ingestion_enabled) {
    return _drush_media_theplatform_mpx_print('Cron ingestion has been enabled.  Aborting child retriever and worker process invocation.', array(), 'error');
  }

  $max_processing_time = !empty($process_metadata['max_processing_time']) ? $process_metadata['max_processing_time'] : drush_get_option('x-max-processing-time', MTPM_PARENT_LOCK_TIMEOUT + MTPM_TIMEOUT_DELTA);
  $run_time = time() - $start_time;

  if ($run_time >= $max_processing_time) {
    $message_variables = array(
      '@time' => time(),
      '@runtime' => $run_time,
      '@max' => $max_processing_time,
    );

    return _drush_media_theplatform_mpx_print('@time: Process has run for @runtime second(s), over its maximum processing time of @max seconds.',
      $message_variables, 'warning');
  }

  // Check that all sibling processes are still running, by checking if their
  // locks are available.  If the sibling process lock is available, it's safe
  // to assume that it has quit.  But only do this after giving them enough time
  // (1 second per sibling) to be invoked by the parent process.
  if (!empty($process_metadata['sibling_count']) && $run_time > $process_metadata['sibling_count']) {
    for ($i = 1; $i <= $process_metadata['sibling_count']; $i++) {
      $sibling_lock_name = MEDIA_THEPLATFORM_MPX_VIDEO_INGESTION_LOCK_BASENAME . '_' . $process_metadata['parent_id'] . '_' . $i;
      if ($sibling_lock_name != $process_metadata['lock_name'] && lock_may_be_available($sibling_lock_name)) {
        $message = '@time: A sibling process with lock "@lock" has quit.  Stopping process to restart all sibling processes.';
        $message_variables = array(
          '@time' => time(),
          '@lock' => $sibling_lock_name,
        );

        return _drush_media_theplatform_mpx_print($message, $message_variables, 'warning');
      }
    }
  }

  // Once the sibling process check is successful, re-acquire the lock for
  // this child process.
  $message_variables = array(
    '@time' => time(),
    '@name' => $process_metadata['lock_name'],
    '@timeout' => $lock_timeout,
  );

  // usleep() for 1/100th of a second to avoid a bug due to microtime()'s lack of
  // fine precision on (some of) Acquia's servers.
  usleep(10000);

  if (!lock_acquire($process_metadata['lock_name'], $lock_timeout)) {
    return _drush_media_theplatform_mpx_print('@time: Unable to re-acquire lock "@name".', $message_variables, 'error');
  }

  return _drush_media_theplatform_mpx_print('@time: Re-acquired lock "@name" for @timeout seconds.', $message_variables);
}

/**
 * Performs a new account video ingestion or a complete video re-ingestion.
 */
function _drush_media_theplatform_mpx_process_complete_video_ingestion(MpxAccount $account) {

  $start_time = time();

  // Ensure the request queue exists.  No harm in re-creating it if it does.
  DrupalQueue::get('media_theplatform_mpx_video_request_queue_' . $account->id)->createQueue();
  $request_queue = DrupalQueue::get('media_theplatform_mpx_video_request_queue_' . $account->id);

  // Populate the request queue for the complete ingestion process if not done
  // so already by another process.
  if (!$request_queue->numberOfItems()) {

    _drush_media_theplatform_mpx_print('Starting complete video (re-)ingestion.');
    _drush_media_theplatform_mpx_print('Adding request queue item(s) for complete video (re-)ingestion.');

    // The default 100 item limit was found to be the most performant. The
    // wait/reward is less ideal when requesting more or less video data at
    // a time.  Though this could be different for non-test environments.
    $request_item_limit = variable_get('media_theplatform_mpx__drush_video_ingestion_request_item_limit', 100);
    $request_count = $account->getDataValue('proprocessing_batch_item_count');
    $current_batch_item = (int) $account->getDataValue('proprocessing_batch_current_item');

    for ($i = $current_batch_item; $i < $request_count; $i += $request_item_limit) {
      $range = $i . '-' . ($i + ($request_item_limit - 1));
      $request_queue_item = array(
        'request_url' => $account->getDataValue('proprocessing_batch_url'),
        'request_range' => $range,
        'request_count' => $request_count,
        'account_id' => $account->id,
      );
      $request_queue->createItem($request_queue_item);
    }

    _drush_media_theplatform_mpx_print('Created @count video request queue item(s) for a complete (re-)ingestion.',
      array('@count' => $request_queue->numberOfItems()));
  }
  else {

    _drush_media_theplatform_mpx_print('Continuing complete video (re-)ingestion.');
  }

  // Invoke the child retriever and worker processes that will actually request
  // and process the video data.
  _drush_media_theplatform_mpx_invoke_retriever_and_worker_processes($account, MTPM_HANDLER_TIMEOUT);

  // Reset the batch system variables to indicate the ingestion is done.
  if (!$request_queue->numberOfItems()) {
    $account->setDataValue('proprocessing_batch_url', '');
    $account->setDataValue('proprocessing_batch_item_count', 0);
    $account->setDataValue('proprocessing_batch_current_item', 0);

    if (!$account->getDataValue('last_notification')) {
      media_theplatform_mpx_set_last_notification($account);
    }

    return _drush_media_theplatform_mpx_print('Full video (re-)ingestion completed.');
  }
  // Otherwise, something went wrong with the retriever and worker processes.
  else {
    return _drush_media_theplatform_mpx_print('Complete (re-)ingestion was unable to complete.  Process will continue from where it left off during the next invocation.', array(), 'warning');
  }
}

/**
 * Parse notification data for updated and deleted videos, and create
 * request queue and cron queue items from this parsed data to be processed
 * by retriever and worker child processes.
 */
function _drush_media_theplatform_mpx_process_notification_data($notification_data, $account) {

  // Ensure the request and cron queues exist, and initialize them.
  DrupalQueue::get('media_theplatform_mpx_video_request_queue_' . $account->id)->createQueue();
  $request_queue = DrupalQueue::get('media_theplatform_mpx_video_request_queue_' . $account->id);
  DrupalQueue::get('media_theplatform_mpx_video_cron_queue')->createQueue();
  $cron_queue = DrupalQueue::get('media_theplatform_mpx_video_cron_queue');

  $updated_video_ids = array();
  $deleted_video_ids = array();
  $last_notification_id = NULL;
  $request_queue_item_count = 0;
  $cron_queue_item_count = 0;

  // Parse the notification data for updated and deleted videos.
  foreach ($notification_data as $notification) {
    $last_notification_id = $notification['id'];
    if (!empty($notification['entry']['id']) && !empty($notification['method'])) {
      $video_id = basename($notification['entry']['id']);
      if ($notification['method'] == 'delete') {
        $deleted_video_ids[$video_id] = $video_id;
      }
      else {
        $updated_video_ids[$video_id] = $video_id;
      }
    }
  }

  // Queue the updated video request items for the retriever process.
  if (!empty($updated_video_ids)) {
    $request_item_limit = variable_get('media_theplatform_mpx__drush_video_notification_ingestion_request_item_limit', 100);
    for ($i = 0; $i < count($updated_video_ids); $i += $request_item_limit) {
      $request_video_ids = array_slice($updated_video_ids, $i, $request_item_limit);
      $request_ids = implode(',', $request_video_ids);
      $request_queue_item = array(
        'request_url' => _media_theplatform_mpx_get_video_feed_url($request_video, $account),
        'request_ids' => $request_ids,
        'request_count' => count($request_video_ids),
        'account_id' => $account->id,
      );
      $request_queue->createItem($request_queue_item);
      $request_queue_item_count++;
    }
  }

  // Queue the deleted videos for the worker process.
  foreach ($deleted_video_ids as $video_id) {
    $cron_queue_item = array(
      'queue_operation' => 'delete',
      'delete_id' => $video_id,
      'account_id' => $account->id,
    );
    $cron_queue->createItem($cron_queue_item);
    $cron_queue_item_count++;
  }

  _drush_media_theplatform_mpx_print('Created @count request queue item(s).', array('@count' => $request_queue_item_count));
  _drush_media_theplatform_mpx_print('Created @count cron queue item(s).', array('@count' => $cron_queue_item_count));

  // Invoke the retriever and worker processes to actually ingest the videos.
  if ($request_queue_item_count || $cron_queue_item_count) {
    _drush_media_theplatform_mpx_invoke_retriever_and_worker_processes($account, MTPM_PROCESS_TIMEOUT, MTPM_REQUEST_ITEM_LIMIT);
  }

  media_theplatform_mpx_set_last_notification($account, $last_notification_id);

  return $last_notification_id;
}

/**
 * Either request or listen for change notifications returned from thePlatform,
 * depending on the value of $blocking.
 */
function _drush_media_theplatform_mpx_process_outstanding_video_notifications(MpxAccount $account, $blocking = FALSE) {

  global $process_metadata;

  // The timeout setting is hardly respected by drupal_http_request().  Set
  // a 2-minute lock timeout to be sure it doesn't expire while the request
  // is being made.
  if (!_drush_media_theplatform_mpx_check_process_health(2 * MTPM_REQUEST_TIMEOUT)) {
    return _drush_media_theplatform_mpx_print('Aborting video change notification processing.', array(), 'warning');
  }

  $memory_limit = ini_get('memory_limit');
  $memory_limit_in_bytes = _media_theplatform_mpx_convert_shorthand_to_bytes($memory_limit);

  // Set a timer.
  $start_time = time();

  // Build the notification service request base URL.
  $last_notification_id = $account->getDataValue('last_notification');
  $base_update_url = 'https://read.data.media.theplatform.com/media/notify?account=' . rawurlencode($account->import_account)
    . '&clientId=drupal_drush_media_theplatform_mpx_' . $account->account_pid
    . '&block=' . ($blocking ? 'true' : 'false')
    . '&filter=Media&size=500';

  // Build the URL for the next notication service request.
  $token = $account->acquireToken();
  $url = $base_update_url . '&since=' . $last_notification_id . '&token=' . $token;

  $message_variables = array(
    '@action' => ($blocking ? dt('Listening for') : dt('Requesting')),
    '!url' => $url,
  );
  _drush_media_theplatform_mpx_print('@action video change notification data from: !url', $message_variables);

  $default_request_timeout = variable_get('media_theplatform_mpx__drush_video_ingestion_notification_timeout', 15.0);

  $request_options = array(
    'timeout' => $default_request_timeout,
  );

  // The $blocking check needs to come second, otherwise the request is never made.
  while (($result_data = _drush_media_theplatform_mpx_retrieve_feed_data($url, TRUE, $request_options)) || $blocking) {

    // If not listening for change notifications, return on any exception
    // returned by thePlatform.
    if (!$blocking && isset($result_data[0]['type']) && $result_data[0]['type'] == 'Exception') {
      return _drush_media_theplatform_mpx_print("Exception returned from thePlatform: \n@e\n.  Aborting change notification processing.",
        array('@e' => print_r($result_data, TRUE)), 'error');
    }

    // If not listening for change notifications, return if the first
    // notification ID returned is the same as the notification ID we processed
    // in the previous request.
    if (!$blocking && isset($result_data[0]['id']) && $result_data[0]['id'] == $last_notification_id) {
      return _drush_media_theplatform_mpx_print('Notification sequence ID is the same as the ID already processed.  Finished processing video change notifications.');
    }

    // If not listening for change notification, and if all that is returned
    // by thePlatform is a notification ID, save it for this account and break.
    if (!$blocking && !empty($result_data[0]['id']) && count($result_data) == 1 && count($result_data[0]) == 1) {
      media_theplatform_mpx_set_last_notification($account, $result_data[0]['id']);

      return _drush_media_theplatform_mpx_print('Only a sequence ID was returned in the change notification data.  Saving ID for this account.  Finished processing video change notifications.');
    }

    $message = "Notification data returned by thePlatform: \n!data\n  ^ End of notification data. ^";
    _drush_media_theplatform_mpx_print($message, array('!data' => print_r($result_data, TRUE)));

    if (!_drush_media_theplatform_mpx_check_process_health()) {
      return _drush_media_theplatform_mpx_print('Aborting video change notification data processing.', array(), 'warning');
    }

    // Process the notification data, if any was returned.
    if (!empty($result_data)) {
      $last_notification_id = _drush_media_theplatform_mpx_process_notification_data($result_data, $account);
    }

    $message_variables = array(
      '@time' => (time() - $start_time),
      '@mem' => round(memory_get_peak_usage(TRUE) / 1048576),
    );
    _drush_media_theplatform_mpx_print('Finished processing outstanding video change notifications in @time seconds.  Peak memory usage is @mem MB.', $message_variables);

    // Check memory usage.  Return if getting too close to the current limit.
    $peak_mem_usage = memory_get_peak_usage(TRUE);
    if (($peak_mem_usage / $memory_limit_in_bytes) > 0.80) {
      return _drush_media_theplatform_mpx_print('Peak real memory usage at @pct% of memory limit.  Aborting video change notification request.',
        array('@pct' => round(($peak_mem_usage / $memory_limit_in_bytes) * 100)), 'warning');
    }

    // Reset the timer.
    $start_time = time();

    // Re-establish our lock.
    if (!_drush_media_theplatform_mpx_check_process_health(20.0)) {
      return _drush_media_theplatform_mpx_print('Aborting video change notification request.', array(), 'warning');
    }

    // Build the URL for the next notication service request.
    $token = $account->acquireToken();
    $url = $base_update_url . '&since=' . $last_notification_id . '&token=' . $token;

    $message_variables = array(
      '@action' => ($blocking ? dt('Listening for') : dt('Requesting')),
      '!url' => $url,
    );
    _drush_media_theplatform_mpx_print('@action video change notification data from: !url', $message_variables);
  }

  return TRUE;
}

/**
 * Ingestion handler for accounts that run a complete (re-)ingestion if
 * necessary and then request change notifications.
 */
function _drush_media_theplatform_mpx_process_video_ingestion_as_account_handler($account) {

  global $process_metadata;

  $start_time = time();
  $account_handler_timeout = MTPM_HANDLER_TIMEOUT;
  $parent_process_metadata = NULL;

  // Account handlers will act as parent processes to the child retriever and
  // worker processes they invoke.  Acquire a parent lock, and set
  // $process_metadata['id'] to indicated this is a parent process, if not already.
  if (empty($process_metadata['id'])) {

    $parent_process_metadata = _drush_media_theplatform_mpx_acquire_parent_process_lock($account->id, $account_handler_timeout);
    if (empty($parent_process_metadata)) {

      return _drush_media_theplatform_mpx_print('Aborting account processing.', array(), 'error');
    }
    $process_metadata['id'] = $parent_process_metadata['id'];

    // Re-acquire the child lock as well.
    $message_variables = array(
      '@time' => time(),
      '@name' => $process_metadata['lock_name'],
      '@type' => $process_metadata['child_type'],
      '@timeout' => $account_handler_timeout,
    );
    if (!lock_acquire($process_metadata['lock_name'], $account_handler_timeout)) {

      return _drush_media_theplatform_mpx_print('@time: Unable to acquire lock "@name".  Aborting @type process.',
        $message_variables, 'error');
    }
    _drush_media_theplatform_mpx_print('@time: Successfully acquired lock "@name" for @timeout seconds.', $message_variables);
  }

  // Ensure we have a valid notification sequence ID.  If not, then
  // reset the last notification value and start a complete video ingestion.
  _drush_media_theplatform_mpx_check_notification_sequence_id($account);

  // If a batch url stored or a last_notification sequence ID is not stored,
  // start an initial ingestion, or a complete re-ingestion.
  if (!$account->getDataValue('last_notification') || $account->getDataValue('proprocessing_batch_url')) {
    _drush_media_theplatform_mpx_process_complete_video_ingestion($account);
    if ($parent_process_metadata) {
      lock_release($parent_process_metadata['lock_name']);
    }

    return TRUE;
  }

  // Check if there are any cron queue items left, and process them.  If the
  // complete (re-)ingestion could not complete, this can act as a second
  // attempt to complete it.
  _drush_media_theplatform_mpx_invoke_retriever_and_worker_processes($account, MTPM_HANDLER_TIMEOUT);

  // Leave if we've been processing cron queue items for the entire time this
  // process/function should run.
  if (!_drush_media_theplatform_mpx_check_process_health()) {

    return _drush_media_theplatform_mpx_print('Aborting account notification handling.', array(), 'warning');
  }

  // If the command is supposed to quit after pulling updates, do not
  // establish a polling connection to thePlatform, and simply retrieve and
  // process all outstanding updates instead.
  if ($account->getDataValue('last_notification') && drush_get_option('quit-when-complete')) {
    $return = _drush_media_theplatform_mpx_process_outstanding_video_notifications($account);
  }
  // Begin listening for change notifications.
  elseif ($account->getDataValue('last_notification')) {
    $return = _drush_media_theplatform_mpx_process_outstanding_video_notifications($account, TRUE);
  }
  // Something is wrong if we don't have a last_notification value by this point.
  else {
    $return = _drush_media_theplatform_mpx_print('Account missing a last_notification value.', array(), 'error');
  }

  if ($parent_process_metadata) {
    lock_release($parent_process_metadata['lock_name']);
  }

  return $return;
}

/**
 * Perform actual requests for video data from thePlatform, retrying the
 * request up to 20 times until the request is successful.
 */
function _drush_media_theplatform_mpx_request_video_data($request_queue_item) {

  $request_timeout = variable_get('media_theplatform_mpx__drush_video_ingestion_request_timeout', MTPM_REQUEST_TIMEOUT);

  $request_url = $request_queue_item->data['request_url'];
  /** @var MpxAccount $request_account */
  $request_account = NULL;
  if (!empty($request_queue_item->data['account_id'])) {
    $request_account = MpxAccount::load($request_queue_item->data['account_id']);
  }
  else {
    $request_account = $request_queue_item->data['account'];
  }
  $request_count = $request_queue_item->data['request_count'];

  if ($request_queue_item->data['request_range']) {
    $request_range = $request_queue_item->data['request_range'];
    $base_url = $request_url . '&range=' . $request_range;
  }
  elseif ($request_queue_item->data['request_ids']) {
    $request_range = $request_queue_item->data['request_ids'];
    $base_url = $request_url . '&byId=' . str_replace(',', '|', $request_range);
  }
  else {
    // Calls to the releaseItem() method can cause MySQL deadlocks.  Try
    // to release the queue item a few times, then give up.  The queue item
    // will be released once its expiration is reached.
    for ($i = 0; $i < 5; $i++) {
      try {
        $request_queue = DrupalQueue::get('media_theplatform_mpx_video_request_queue_' . $request_account->id);
        $request_queue->releaseItem($request_queue_item);
      }
      catch (Exception $e) {
        usleep(10000);
      }
    }

    return _drush_media_theplatform_mpx_print('Request range or IDs missing from request queue item.  Aborting request queue processing.', array(), 'error');
  }

  // Rertrieve the video data with a maximum of 20 request attempts.
  for ($i = 0; $i < 20; $i++) {

    if (!_drush_media_theplatform_mpx_check_process_health(MTPM_REQUEST_TIMEOUT_SHORT)) {
      return _drush_media_theplatform_mpx_print('Aborting request queue processing.', array(), 'warning');
    }

    // Retrieve the authentication token for the media data service request.
    $token = $request_account->acquireToken();

    if (!$token) {
      _drush_media_theplatform_mpx_print('Failed to retrieve authentication token.  Trying again.', array(), 'error');
      continue;
    }

    // Setting a lock timeout with a buffer above the request timeout.
    if (!_drush_media_theplatform_mpx_check_process_health($request_timeout * 1.3)) {
      return _drush_media_theplatform_mpx_print('Aborting request queue processing.', array(), 'warning');
    }

    // Construct the url for the media data service request.
    $url = $base_url . '&token=' . $token;

    $message = 'Retrieving videos @range of @total via drush from url:  !url.';
    $message_variables = array(
      '@range' => $request_range,
      '@total' => $request_count,
      '!url' => $url,
    );
    _drush_media_theplatform_mpx_print($message, $message_variables);

    // Retrieve the video data from thePlatform's media data service.
    $result_data = array();

    try {
      $result_data = _media_theplatform_mpx_retrieve_feed_data($url, TRUE, array('timeout' => $request_timeout));
    }
    catch (Exception $e) {
      _drush_media_theplatform_mpx_print("Exception thrown when retrieving video data: \n!e\n", array('!e' => $e->getMessage()));
      continue;
    }

    // If the result data is not empty, the request was successful and we have
    // data to process.  Otherwise, try the request again in case of a timeout
    // or other problem another request attempt will resolve.
    if (!empty($result_data)) {
      break;
    }
    else {
      _drush_media_theplatform_mpx_print('Missing or invalid data returned from media data service request.  Trying again.',
        array(), 'error');
    }
  }

  return $result_data;
}

/**
 * Processes request queue items to retrieve video data from thePlatform, and
 * create cron queue items from the data.
 */
function _drush_media_theplatform_mpx_process_video_ingestion_as_retriever($account) {

  global $process_metadata;

  $memory_limit = ini_get('memory_limit');
  $memory_limit_in_bytes = _media_theplatform_mpx_convert_shorthand_to_bytes($memory_limit);
  $request_queue = DrupalQueue::get('media_theplatform_mpx_video_request_queue_' . $account->id);
  $cron_queue = DrupalQueue::get('media_theplatform_mpx_video_cron_queue');

  // Grab an item from the batch queue and queue up the videos retrieved.
  while ($request_queue->numberOfItems() || $cron_queue->numberOfItems()) {

    if (!_drush_media_theplatform_mpx_check_process_health()) {
      return _drush_media_theplatform_mpx_print('Aborting request queue processing.', array(), 'warning');
    }

    // Reset expired items in the queue table.
    try {
      // We have to use db_query() because db_update() doesn't have the orderBy()
      // method.  And we need to order the query to try to avoid MySQL deadlocks.
      db_query('UPDATE queue SET expire = 0 WHERE expire <> 0 AND expire < UNIX_TIMESTAMP(NOW()) ORDER BY item_id ASC');
    }
    catch (Exception $e) {
      // The only exception we've seen from this query have been MySQL deadlocks.
      // If this is the case, another concurrent process has already executed
      // the query, so this process can just continue on without doing it.
      _drush_media_theplatform_mpx_print('Exception caught when updating queue item expiration: @message',
        array('@message' => $e->getMessage()), 'warning');
    }

    // Claim an actual request queue item.
    $request_queue_item = $request_queue->claimItem();

    // If a request queue item could not be claimed, then either the request
    // queue has not been populated yet (and being done so by another process),
    // or mor likely, the request queue is empty and the worker process(es) are
    // busy churning through the cron queue.  In the latter case, wait until the
    // worker process(es) are finished before leaving this function and quitting
    // this process.
    if (!$request_queue_item) {
      _drush_media_theplatform_mpx_print('Waiting for request queue item(s) to become available for processing, or for sibling worker processes to complete processing the cron queue.');
      sleep(1);
      continue;
    }

    $start_time = time();

    // Request the video data to be processed.
    $result_data = _drush_media_theplatform_mpx_request_video_data($request_queue_item);

    // Ensure we have data and that an exception was not returned from
    // thePlatform.  In either case, release the request queue item and start
    // over.
    if (!is_array($result_data) || empty($result_data)) {
      // Calls to the releaseItem() method can cause MySQL deadlocks.  Try
      // to release the queue item a few times, then give up.  The queue item
      // will be released once its expiration is reached.
      for ($i = 0; $i < 5; $i++) {
        try {
          $request_queue->releaseItem($request_queue_item);
        }
        catch (Exception $e) {
          usleep(10000);
        }
      }
      _drush_media_theplatform_mpx_print('No video data returned from thePlatform.', array(), 'warning');
      continue;
    }
    elseif (isset($result_data['isException']) && $result_data['isException'] === TRUE) {
      // Calls to the releaseItem() method can cause MySQL deadlocks.  Try
      // to release the queue item a few times, then give up.  The queue item
      // will be released once its expiration is reached.
      for ($i = 0; $i < 5; $i++) {
        try {
          $request_queue->releaseItem($request_queue_item);
        }
        catch (Exception $e) {
          usleep(10000);
        }
      }
      _drush_media_theplatform_mpx_print("Exception returned by thePlatform: \n@e\n",
        array('@e' => print_r($result_data, TRUE)), 'error');
      continue;
    }

    // Parse the video data and create the cron queue items for the worker
    // process(es).
    $entries = array();
    $published_video_ids = array();

    // If multiple videos are returned, they should be in the "entries" element.
    // This can be the case for a single returned video as well.
    if (!empty($result_data['entryCount']) && !empty($result_data['entries'])) {
      $entries = $result_data['entries'];
    }
    // A single returned video's data can also be the entire response.
    elseif (!empty($result_data['entryCount'])) {
      $entries = array($result_data);
    }
    // If neither of the above cases apply, then no video data was returned.
    else {
      $entries = array();
    }

    // Create the cron queue items for each updated/created video for the worker
    // process(es).
    foreach ($entries as $data) {
      if (empty($data) || empty($data['id']) || empty($data['title'])) {
        _drush_media_theplatform_mpx_print('Video data entry is empty.', array(), 'error');
        drush_print_r($data);
        _drush_media_theplatform_mpx_print('  ^ End of returned video data.', array(), 'error');
        continue;
      }
      $published_video_ids[] = basename($data['id']);
      $item = array(
        'queue_operation' => 'publish',
        'video' => $data,
        'account_id' => $request_queue_item->data['account_id'],
      );
      $cron_queue->createItem($item);
    }

    // If a response contains less videos than were requested, then the videos
    // that were not returned should be unpublished.
    if (isset($result_data['entryCount']) && ($result_data['entryCount'] != $request_queue_item->data['request_count'])) {
      foreach (explode(',', $request_queue_item->data['request_ids']) as $video_id) {
        if (!in_array($video_id, $published_video_ids)) {
          $item = array(
            'queue_operation' => 'unpublish',
            'unpublish_id' => $video_id,
            'account_id' => $request_queue_item->data['account_id'],
          );
          $cron_queue->createItem($item);
        }
      }
    }

    // Delete the request queue item.  It has been successfully processed.
    $request_queue->deleteItem($request_queue_item);

    $message = 'Retreiving and parsing video data via drush took @time seconds.  Peak memory usage is @mem MB.';
    $message_variables = array(
      '@time' => (time() - $start_time),
      '@mem' => round(memory_get_peak_usage(TRUE) / 1048576),
    );
    _drush_media_theplatform_mpx_print($message, $message_variables);

    // Check memory usage.  Return if getting too close to the current limit.
    $peak_mem_usage = memory_get_peak_usage(TRUE);
    if (($peak_mem_usage / $memory_limit_in_bytes) > 0.80) {
      return _drush_media_theplatform_mpx_print('Peak real memory usage at @pct% of memory limit.  Aborting video change notification request.',
        array('@pct' => round(($peak_mem_usage / $memory_limit_in_bytes) * 100)), 'warning');
    }

    _drush_media_theplatform_mpx_print('There are @count item(s) in the request ingestion queue.',
      array('@count' => $request_queue->numberOfItems()));
  }
}

/**
 * Processes cron queue items to create, update, or delete videos.
 */
function _drush_media_theplatform_mpx_process_video_ingestion_as_worker($account) {

  global $process_metadata;

  $memory_limit = ini_get('memory_limit');
  $memory_limit_in_bytes = _media_theplatform_mpx_convert_shorthand_to_bytes($memory_limit);
  $request_queue = DrupalQueue::get('media_theplatform_mpx_video_request_queue_' . $account->id);
  $cron_queue = DrupalQueue::get('media_theplatform_mpx_video_cron_queue');
  $request_item_limit = variable_get('media_theplatform_mpx__drush_video_ingestion_request_item_limit', 100);

  while ($request_queue->numberOfItems() || $cron_queue->numberOfItems()) {

    if (!_drush_media_theplatform_mpx_check_process_health()) {
      return _drush_media_theplatform_mpx_print('Aborting cron queue processing.', array(), 'warning');
    }

    // Reset expired items in the queue table.
    try {
      // We have to use db_query() because db_update() doesn't have the orderBy()
      // method.  And we need to order the query to try to avoid MySQL deadlocks.
      db_query('UPDATE queue SET expire = 0 WHERE expire <> 0 AND expire < UNIX_TIMESTAMP(NOW()) ORDER BY item_id ASC');
    }
    catch (Exception $e) {
      // The only exception we've seen from this query have been MySQL deadlocks.
      // If this is the case, another concurrent process has already executed
      // the query, so this process can just continue on without doing it.
      _drush_media_theplatform_mpx_print('Exception caught when updating queue item expiration: @message',
        array('@message' => $e->getMessage()), 'warning');
    }

    $start_time = time();
    $item_count = 0;

    // Process cron queue items while they are available.
    while ($item_count < $request_item_limit && $item = $cron_queue->claimItem()) {

      $video_id = !empty($item->data['video']['id']) ? basename($item->data['video']['id']) : '';
      $video_title = !empty($item->data['video']['title']) ? $item->data['video']['title'] : '';

      if ($item->data['queue_operation'] == 'publish' && (!$video_id || !$video_title)) {
        _drush_media_theplatform_mpx_print('Publish cron item @item_id contains no video data.  Item data:',
          array('@item_id' => $item->item_id), 'error');
        drush_print_r($data);
        $cron_queue->deleteItem($item);
        _drush_media_theplatform_mpx_print('^ End of data. ^');

        continue;
      }

      $message_variables = array(
        '@id' => $item->item_id,
        '@video_id' => $video_id,
        '@video_title' => $video_title,
      );
      _drush_media_theplatform_mpx_print('Processing cron queue item @id, video @video_id "@video_title".', $message_variables);

      try {
        process_media_theplatform_mpx_video_cron_queue_item($item->data);
        $cron_queue->deleteItem($item);
        $item_count++;
      }
      catch (Exception $e) {
        // Calls to the releaseItem() method can cause MySQL deadlocks.  Try
        // to release the queue item a few times, then give up.  The queue item
        // will be released once its expiration is reached.
        for ($i = 0; $i < 5; $i++) {
          try {
            $cron_queue->releaseItem($item);
          }
          catch (Exception $e) {
            usleep(10000);
          }
        }
        $message_variables = array(
          '@e' => $e->getMessage(),
          '!data' => print_r($item_data, TRUE),
        );
        _drush_media_theplatform_mpx_print("Exception \"@e\" thrown when processing cron queue item data: \n!data\n",
          $message_variables, 'error');
      }

      // Re-acquire lock every 10 queue items.
      if ($item_count && !($item_count % 10) && !_drush_media_theplatform_mpx_check_process_health()) {
        return _drush_media_theplatform_mpx_print('Aborting cron queue processing.', array(), 'warning');
      }

      // Check peak memory usage every 25 items.  Break if it's getting too
      // close to the memory limit.
      if ($item_count && !($item_count % 25) && (memory_get_peak_usage(TRUE) / $memory_limit_in_bytes) > 0.80) {
        return _drush_media_theplatform_mpx_print('Peak real memory usage at @pct% of memory limit.  Aborting cron queue processing.',
          array('@pct' => round((memory_get_peak_usage(TRUE) / $memory_limit_in_bytes) * 100)), 'warning');
      }
    }

    // If no cron queue items could be processed, the cron queue is likely empty.
    // Wait a second for the cron queue to be populated by a retriever process
    // and try again.
    if (!$item_count) {
      _drush_media_theplatform_mpx_print('Waiting for cron queue item to become available for processing.');
      sleep(1);
      continue;
    }

    _drush_media_theplatform_mpx_print('There are @count item(s) in the cron ingestion queue.',
      array('@count' => $cron_queue->numberOfItems()));

    $message = 'Processing @count cron queue items via drush took @time seconds.  Peak memory usage is @mem MB.';
    $message_variables = array(
      '@count' => $item_count,
      '@time' => (time() - $start_time),
      '@mem' => round(memory_get_peak_usage(TRUE) / 1048576),
    );
    _drush_media_theplatform_mpx_print($message, $message_variables);
  }
}

/**
 * Replacement for drupal_http_request() that will respect timeout option.
 */
function _drush_media_theplatform_mpx_http_request($url, array $options = array()) {

  $result = new stdClass();

  // Parse the URL and make sure we can handle the schema.
  $uri = @parse_url($url);

  if ($uri == FALSE) {
    $result->error = 'unable to parse URL';
    $result->code = -1001;
    return $result;
  }

  if (empty($uri['scheme'])) {
    // Since we're executing this from the drush context we can't determine the
    // current page's protocol. Fallback to 'https'.
    $uri['scheme'] = variable_get('media_theplatform_mpx_assets_protocol_fallback', 'https');
  }

  timer_start(__FUNCTION__);

  // Merge the default options.
  $options += array(
    'headers' => array(),
    'method' => 'GET',
    'data' => NULL,
    'max_redirects' => 3,
    'timeout' => MTPM_REQUEST_TIMEOUT_SHORT,
    'context' => NULL,
  );

  // Merge the default headers.
  $options['headers'] += array(
    'User-Agent' => 'Drupal (+http://drupal.org/)',
  );

  // stream_socket_client() requires timeout to be a float.
  $options['timeout'] = (float) $options['timeout'];

  // Use a proxy if one is defined and the host is not on the excluded list.
  $proxy_server = variable_get('proxy_server', '');
  if ($proxy_server && _drupal_http_use_proxy($uri['host'])) {
    // Set the scheme so we open a socket to the proxy server.
    $uri['scheme'] = 'proxy';
    // Set the path to be the full URL.
    $uri['path'] = $url;
    // Since the URL is passed as the path, we won't use the parsed query.
    unset($uri['query']);

    // Add in username and password to Proxy-Authorization header if needed.
    if ($proxy_username = variable_get('proxy_username', '')) {
      $proxy_password = variable_get('proxy_password', '');
      $options['headers']['Proxy-Authorization'] = 'Basic ' . base64_encode($proxy_username . (!empty($proxy_password) ? ":" . $proxy_password : ''));
    }
    // Some proxies reject requests with any User-Agent headers, while others
    // require a specific one.
    $proxy_user_agent = variable_get('proxy_user_agent', '');
    // The default value matches neither condition.
    if ($proxy_user_agent === NULL) {
      unset($options['headers']['User-Agent']);
    }
    elseif ($proxy_user_agent) {
      $options['headers']['User-Agent'] = $proxy_user_agent;
    }
  }

  switch ($uri['scheme']) {
    case 'proxy':
      // Make the socket connection to a proxy server.
      $socket = 'tcp://' . $proxy_server . ':' . variable_get('proxy_port', 8080);
      // The Host header still needs to match the real request.
      $options['headers']['Host'] = $uri['host'];
      $options['headers']['Host'] .= isset($uri['port']) && $uri['port'] != 80 ? ':' . $uri['port'] : '';
      break;

    case 'http':
    case 'feed':
      $port = isset($uri['port']) ? $uri['port'] : 80;
      $socket = 'tcp://' . $uri['host'] . ':' . $port;
      // RFC 2616: "non-standard ports MUST, default ports MAY be included".
      // We don't add the standard port to prevent from breaking rewrite rules
      // checking the host that do not take into account the port number.
      $options['headers']['Host'] = $uri['host'] . ($port != 80 ? ':' . $port : '');
      break;

    case 'https':
      // Note: Only works when PHP is compiled with OpenSSL support.
      $port = isset($uri['port']) ? $uri['port'] : 443;
      $socket = 'ssl://' . $uri['host'] . ':' . $port;
      $options['headers']['Host'] = $uri['host'] . ($port != 443 ? ':' . $port : '');
      break;

    default:
      $result->error = 'invalid schema ' . $uri['scheme'];
      $result->code = -1003;
      return $result;
  }

  if (empty($options['context'])) {
    $fp = @stream_socket_client($socket, $errno, $errstr, $options['timeout']);
  }
  else {
    // Create a stream with context. Allows verification of a SSL certificate.
    $fp = @stream_socket_client($socket, $errno, $errstr, $options['timeout'], STREAM_CLIENT_CONNECT, $options['context']);
  }

  // Make sure the socket opened properly.
  if (!$fp) {
    // When a network error occurs, we use a negative number so it does not
    // clash with the HTTP status codes.
    $result->code = -$errno;
    $result->error = trim($errstr) ? trim($errstr) : t('Error opening socket @socket', array('@socket' => $socket));

    // Mark that this request failed. This will trigger a check of the web
    // server's ability to make outgoing HTTP requests the next time that
    // requirements checking is performed.
    // See system_requirements().
    variable_set('drupal_http_request_fails', TRUE);

    return $result;
  }

  // Construct the path to act on.
  $path = isset($uri['path']) ? $uri['path'] : '/';
  if (isset($uri['query'])) {
    $path .= '?' . $uri['query'];
  }

  // Only add Content-Length if we actually have any content or if it is a POST
  // or PUT request. Some non-standard servers get confused by Content-Length in
  // at least HEAD/GET requests, and Squid always requires Content-Length in
  // POST/PUT requests.
  $content_length = strlen($options['data']);
  if ($content_length > 0 || $options['method'] == 'POST' || $options['method'] == 'PUT') {
    $options['headers']['Content-Length'] = $content_length;
  }

  // If the server URL has a user then attempt to use basic authentication.
  if (isset($uri['user'])) {
    $options['headers']['Authorization'] = 'Basic ' . base64_encode($uri['user'] . (isset($uri['pass']) ? ':' . $uri['pass'] : ':'));
  }

  // If the database prefix is being used by SimpleTest to run the tests in a copied
  // database then set the user-agent header to the database prefix so that any
  // calls to other Drupal pages will run the SimpleTest prefixed database. The
  // user-agent is used to ensure that multiple testing sessions running at the
  // same time won't interfere with each other as they would if the database
  // prefix were stored statically in a file or database variable.
  $test_info = &$GLOBALS['drupal_test_info'];
  if (!empty($test_info['test_run_id'])) {
    $options['headers']['User-Agent'] = drupal_generate_test_ua($test_info['test_run_id']);
  }

  $request = $options['method'] . ' ' . $path . " HTTP/1.0\r\n";
  foreach ($options['headers'] as $name => $value) {
    $request .= $name . ': ' . trim($value) . "\r\n";
  }
  $request .= "\r\n" . $options['data'];
  $result->request = $request;

  // Calculate how much time is left of the original timeout value.
  $timeout = $options['timeout'] - timer_read(__FUNCTION__) / 1000;
  if ($timeout > 0) {
    stream_set_timeout($fp, floor($timeout), floor(1000000 * fmod($timeout, 1)));
    fwrite($fp, $request);
  }

  // Fetch response. Due to PHP bugs like http://bugs.php.net/bug.php?id=43782
  // and http://bugs.php.net/bug.php?id=46049 we can't rely on feof(), but
  // instead must invoke stream_get_meta_data() each iteration.
  $info = stream_get_meta_data($fp);
  $alive = !$info['eof'] && !$info['timed_out'];
  $response = '';

  $read_streams = array($fp);
  $write_streams = array($fp);
  $except_streams = array($fp);
  $stream_select_response = stream_select($read_streams, $write_streams, $except_streams, 3);

  while ($alive) {
    // Calculate how much time is left of the original timeout value.
    $timeout = $options['timeout'] - timer_read(__FUNCTION__) / 1000;
    if ($timeout <= 0) {
      $info['timed_out'] = TRUE;
      break;
    }
    // stream_select() doesn't seem to return correctly if the write and except
    // stream arrays aren't populated.
    $read_streams = array($fp);
    $write_streams = array($fp);
    $except_streams = array($fp);
    $stream_select_response = stream_select($read_streams, $write_streams, $except_streams, 3);
    // Only read from the stream if stream_select() returned a change to the read stream.
    if (!empty($read_streams)) {
      stream_set_timeout($fp, floor($timeout), floor(1000000 * fmod($timeout, 1)));
      $chunk = fread($fp, 1024);
      $response .= $chunk;
    }
    else {
      usleep(200);
    }
    $info = stream_get_meta_data($fp);
    $alive = !$info['eof'] && !$info['timed_out'];
  }
  fclose($fp);

  if ($info['timed_out']) {
    $result->code = HTTP_REQUEST_TIMEOUT;
    $result->error = 'request timed out';
    return $result;
  }
  // Parse response headers from the response body.
  // Be tolerant of malformed HTTP responses that separate header and body with
  // \n\n or \r\r instead of \r\n\r\n.
  list($response, $result->data) = preg_split("/\r\n\r\n|\n\n|\r\r/", $response, 2);
  $response = preg_split("/\r\n|\n|\r/", $response);

  // Parse the response status line.
  list($protocol, $code, $status_message) = explode(' ', trim(array_shift($response)), 3);
  $result->protocol = $protocol;
  $result->status_message = $status_message;

  $result->headers = array();

  // Parse the response headers.
  while ($line = trim(array_shift($response))) {
    list($name, $value) = explode(':', $line, 2);
    $name = strtolower($name);
    if (isset($result->headers[$name]) && $name == 'set-cookie') {
      // RFC 2109: the Set-Cookie response header comprises the token Set-
      // Cookie:, followed by a comma-separated list of one or more cookies.
      $result->headers[$name] .= ',' . trim($value);
    }
    else {
      $result->headers[$name] = trim($value);
    }
  }

  $responses = array(
    100 => 'Continue',
    101 => 'Switching Protocols',
    200 => 'OK',
    201 => 'Created',
    202 => 'Accepted',
    203 => 'Non-Authoritative Information',
    204 => 'No Content',
    205 => 'Reset Content',
    206 => 'Partial Content',
    300 => 'Multiple Choices',
    301 => 'Moved Permanently',
    302 => 'Found',
    303 => 'See Other',
    304 => 'Not Modified',
    305 => 'Use Proxy',
    307 => 'Temporary Redirect',
    400 => 'Bad Request',
    401 => 'Unauthorized',
    402 => 'Payment Required',
    403 => 'Forbidden',
    404 => 'Not Found',
    405 => 'Method Not Allowed',
    406 => 'Not Acceptable',
    407 => 'Proxy Authentication Required',
    408 => 'Request Time-out',
    409 => 'Conflict',
    410 => 'Gone',
    411 => 'Length Required',
    412 => 'Precondition Failed',
    413 => 'Request Entity Too Large',
    414 => 'Request-URI Too Large',
    415 => 'Unsupported Media Type',
    416 => 'Requested range not satisfiable',
    417 => 'Expectation Failed',
    500 => 'Internal Server Error',
    501 => 'Not Implemented',
    502 => 'Bad Gateway',
    503 => 'Service Unavailable',
    504 => 'Gateway Time-out',
    505 => 'HTTP Version not supported',
  );
  // RFC 2616 states that all unknown HTTP codes must be treated the same as the
  // base code in their class.
  if (!isset($responses[$code])) {
    $code = floor($code / 100) * 100;
  }
  $result->code = $code;

  switch ($code) {
    case 200: // OK
    case 304: // Not modified
      break;
    case 301: // Moved permanently
    case 302: // Moved temporarily
    case 307: // Moved temporarily
      $location = $result->headers['location'];
      $options['timeout'] -= timer_read(__FUNCTION__) / 1000;
      if ($options['timeout'] <= 0) {
        $result->code = HTTP_REQUEST_TIMEOUT;
        $result->error = 'request timed out';
      }
      elseif ($options['max_redirects']) {
        // Redirect to the new location.
        $options['max_redirects']--;
        $result = _drush_media_theplatform_mpx_http_request($location, $options);
        $result->redirect_code = $code;
      }
      if (!isset($result->redirect_url)) {
        $result->redirect_url = $location;
      }
      break;
    default:
      $result->error = $status_message;
  }

  return $result;
}

/**
 * Helper that retrieves and returns data from an mpx feed URL.
 * This is a copy of _media_theplatform_mpx_retrieve_feed_data() except that it
 * calls _drush_media_theplatform_mpx_http_request() instead of drupal_http_request().
 */
function _drush_media_theplatform_mpx_retrieve_feed_data($url, $json_decode = TRUE, $options = array()) {

  // Fetch the actual feed data.
  $feed_request_timeout = variable_get('media_theplatform_mpx__cron_videos_timeout', 180);

  $options += array('timeout' => $feed_request_timeout);

  // Allow for altering the URL before making the request.
  drupal_alter('media_theplatform_mpx_feed_request', $url, $options);

  // Inform site admins we're attempting to retrieve data.
  watchdog('media_theplatform_mpx', 'Retrieving mpx feed data (timeout = @timeout seconds) from the following feed url: @url',
    array(
      '@timeout' => $options['timeout'],
      '@url' => $url,
    ),
    WATCHDOG_INFO);

  $start_time = time();
  $result = _drush_media_theplatform_mpx_http_request($url, $options);
  $response_time = time() - $start_time;

  if (!is_object($result)) {
    watchdog('media_theplatform_mpx', 'Failed to retrieve mpx feed data. drupal_http_request() did not return an object.',
      array(), WATCHDOG_ERROR);

    return $json_decode ? array() : '';
  }
  if (empty($result->data)) {
    watchdog('media_theplatform_mpx', 'Failed to retrieve mpx feed data. No data returned from thePlatform after @time seconds from: @url
<br />Response information:
<br /><pre>@response</pre>',
      array(
        '@time' => $response_time,
        '@url' => $url,
        '@response' => print_r($result, TRUE),
      ),
      WATCHDOG_ERROR);

    return $json_decode ? array() : ''  ;
  }

  watchdog('media_theplatform_mpx', 'Request to retrieve mpx feed data took @time second(s).',
    array('@time' => $response_time), WATCHDOG_INFO);

  $memory_limit = ini_get('memory_limit');
  $memory_limit_in_bytes = _media_theplatform_mpx_convert_shorthand_to_bytes($memory_limit);
  $expensive_operation_min_memory = 536870912;

  if (variable_get('media_theplatform_mpx_debug', 0)) {
    // Increase memory if necessary beforehand, as this message can be
    // extremely large and use up available memory if set lower than 512M.
    if ($memory_limit_in_bytes < $expensive_operation_min_memory) {
      ini_set('memory_limit', '512M');
    }
    media_theplatform_mpx_debug($result, "Request to retrieve mpx feed data");
    if ($memory_limit_in_bytes < $expensive_operation_min_memory) {
      ini_set('memory_limit', $memory_limit);
    }
  }

  $result_data = $result->data;

  if ($json_decode) {

    if ($memory_limit_in_bytes < $expensive_operation_min_memory) {
      ini_set('memory_limit', '512M');
    }

    $result_data = drupal_json_decode($result_data);

    if ($memory_limit_in_bytes < $expensive_operation_min_memory) {
      ini_set('memory_limit', $memory_limit);
    }

    // Build a list of constant names associated to their values. This is done
    // to identify what error happened.
    if (!empty($result->data) && $result_data !== NULL && json_last_error() != JSON_ERROR_NONE) {
      $constants = get_defined_constants(true);
      $json_errors = array();
      foreach ($constants["json"] as $name => $value) {
        if (!strncmp($name, "JSON_ERROR_", 11)) {
          $json_errors[$value] = $name;
        }
      }
      watchdog('media_theplatform_mpx', 'Failed to decode JSON response data with drupal_json_decode() with error @json_error from the following URL: @url
<br />...and the following data returned from thePlatform:
<br /><pre>@response</pre>.', array(
        '@json_error' => $json_errors[json_last_error()],
        '@url' => $url,
        '@response' => print_r($result, TRUE)
      ), WATCHDOG_ERROR);
      return array();
    }
  }

  if (empty($result_data)) {
    watchdog('media_theplatform_mpx', 'Request to retrieve mpx feed data returned no data.', array(), WATCHDOG_ERROR);

    return $json_decode ? array() : '';
  }

  if (isset($result_data['isException']) && $result_data['isException'] === TRUE) {
    watchdog('media_theplatform_mpx', 'Request to retrieve mpx feed data returned a(n) "@title" exception.  Description:  @description  --  Response Code:  @response_code  --  Correlation ID:  @correlation_id',
      array(
        '@title' => $result_data['title'],
        '@description' => $result_data['description'],
        '@response_code' => $result_data['responseCode'],
        '@correlation_id' => $result_data['correlationId'],
      ),
      WATCHDOG_ERROR);

    return $json_decode ? array() : '';
  }

  if (variable_get('media_theplatform_mpx_debug', 0)) {
    // Increase memory if necessary beforehand, as this message can be
    // extremely large and use up available memory if set lower than 512M.
    if ($memory_limit_in_bytes < $expensive_operation_min_memory) {
      ini_set('memory_limit', '512M');
    }
    media_theplatform_mpx_debug($result_data, "Request to retrieve mpx feed data");
    if ($memory_limit_in_bytes < $expensive_operation_min_memory) {
      ini_set('memory_limit', $memory_limit);
    }
  }

  return $result_data;
}
