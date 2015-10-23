<?php
/**
 * @file
 * The functions need to run harvester.
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Implements hook_cron().
 *
 * Run harvester periodically.
 */
function oaiharvester_cron() {
  xc_log_info('oaiharvester cron', 'cron started');
  ini_set('xdebug.collect_vars', 1);

  variable_del('oaiharvester_statistics');
  // TODO: check it!
  $cron_last = variable_get('oaiharvester_cron_last', NULL);
  $never_run = FALSE;
  if (!is_numeric($cron_last)) {
    $never_run = TRUE;
    $cron_last = variable_get('install_time', 0);
  }
  $last = _parse_date($cron_last);
  $now  = _parse_date();

  $distance = _parse_date($now->timestamp - $last->timestamp);
  xc_log_info('oaiharvester cron', 'Distance from last ran of cron is ' . oaiharvester_sec_to_time($distance->timestamp) . ' ' . var_export($distance, TRUE));

  $sql = oaiharvester_cron_get_selection_sql($now, $last, $distance);
  // TODO Please convert this statement to the D7 database API syntax.
  $result = db_query($sql);
  $schedules = array();
  foreach ($result as $schedule) {
    $schedules[] = $schedule;
  }

  if (empty($schedules)) {
    xc_log_info('oaiharvester cron', 'There is no schedule to launch now by cron.');
    return;
  }
  else {
    $msg = format_plural(count($schedules),
      'There is only one schedule to run',
      'There are @count schedules to run',
       array('@count' => count($schedules))
    );
    xc_log_info('oaiharvester cron', $msg);
  }

  // Delete all oaiharvester queue
  $last_last_run = NULL;
  _oaiharvester_queue_delete_all();

  $schedule_ids = array();
  // put items to queue if this item is not already there
  foreach ($schedules as $schedule) {
    $runnable = oaiharvester_schedule_is_runnable($schedule->harvest_schedule_id, false);
    if ($runnable === FALSE) {
      continue;
    }

    $schedule_ids[] = $schedule->harvest_schedule_id;
    $schedule = _oaiharvester_schedule_get_full($schedule->harvest_schedule_id);
    $additional_batch_sets = module_invoke_all('oaiharvester_additional_harvest_steps', $schedule->harvest_schedule_id);

    // put steps to queue
    $steps = _oaiharvester_schedule_steps_get($schedule->harvest_schedule_id);
    $last_last_run = $steps[count($steps) - 1]->last_ran;
    $provider = oaiharvester_repository_load($schedule->provider_id);
    if ($provider->type == 'cache') {
      oaiharvester_add_item_to_queue($schedule, 'xc', '', _oaiharvester_format_date(1, $provider->granularity));
    }
    else {
      foreach ($steps as $step) {
        oaiharvester_add_item_to_queue(
          $schedule,
          $step->name, // format name
          $step->set_spec,
          _oaiharvester_format_date($step->last_ran, $provider->granularity)
        );
      }
    }
    // set status active
    oaiharvester_schedule_set_active($schedule->harvest_schedule_id);
  }

  $queue = _oaiharvester_queue_get();
  $operations = array();

  $saved_batch = (object) array(
    'schedule_id' => $schedule->harvest_schedule_id,
    'timestamp' => time(),
  );
  drupal_write_record('oaiharvester_batch', $saved_batch);
  $operation_id = 0;

  $has_operations = FALSE;
  foreach ($queue as $queue_item) {
    if (!$has_operations) {
      $has_operations = TRUE;
    }

    $schedule = oaiharvester_schedule_load($queue_item->harvest_schedule_id);
    if ($schedule->skip_main_task) {
      continue;
    }

    // run batch harvest
    xc_log_info('oaiharvester cron', 'launch ' . join(', ', array(
      'id: ' . $queue_item->harvest_id,
      'url: ' . $queue_item->provider_url,
      'prefix: ' . $queue_item->metadata_prefix,
      'set: ' . $queue_item->set_name,
      'from: ' . $queue_item->from_date,
      'until: ' . $queue_item->until_date,
      'parsing: ' . $queue_item->parsing_mode,
      'batch id: ' . $saved_batch->id,
    )));

    $operations[] = oaiharvester_create_batch_harvester_operation(
      'cron',                       // launcher type,
      $queue_item->harvest_id,      // launcher_id,
      $queue_item->provider_url,    // url
      $queue_item->metadata_prefix, // metadataPrefix
      $queue_item->set_name,        // set
      $queue_item->from_date,       // from
      $queue_item->until_date,      // until
      $queue_item->parsing_mode,    // parsing_mode
      $saved_batch->id,             // saved batch id
      $operation_id++,              // operation count
      NULL                          // resumption token
    );
  }

  /*
      foreach ($steps as $step) {
        $set_spec = empty($step->set_spec) ? NULL : $step->set_spec;
        $last_ran = _oaiharvester_format_date($step->last_ran, $provider->granularity);
        $operations[] = oaiharvester_create_batch_harvester_operation(
          'manual',                       // launcher_type
          $schedule->harvest_schedule_id, // launcher_id
          $schedule->oai_provider_url,    // url
          $step->name,                    // metadataPrefix
          $step->set_spec,                // set
          $last_ran,                      // from
          NULL,                           // until
          $schedule->parsing_mode,        // parsing_mode
          $saved_batch->id,               // saved batch id
          $operation_id++,                // operation count
          NULL                            // resumption token
        );
      }
    }
    elseif ($provider->type == OAIHARVESTER_PROVIDERTYPE_CACHE) {
      $operations[] = oaiharvester_create_batch_harvester_operation(
        'manual',                       // launcher_type
        $schedule->harvest_schedule_id, // launcher_id
        $schedule->oai_provider_url,    // url
        NULL,                    // metadataPrefix
        NULL,                // set
        NULL,                      // from
        NULL,                           // until
        $schedule->parsing_mode,        // parsing_mode
        $saved_batch->id,               // saved batch id
        $operation_id++,                // operation count
        NULL                            // resumption token
      );
    }
  */

  if (!$has_operations) {
    xc_log_info('oaiharvester cron', 'There is no harvester to launch now by cron.');
    return;
  }

  $processing_cron = variable_get('oaiharvester_processing_cron', 'NOT_RUNNING');
  xc_log_info('oaiharvester cron', 'Processing variable: ' . $processing_cron);
  if ($processing_cron != 'NOT_RUNNING') {
    xc_log_warning('oaiharvester cron', 'Other harvesting task is already running. (\'oaiharvester_processing_cron\' variable is set to "' . $processing_cron . '")');
    return;
  }

  xc_log_info('oaiharvester cron', 'oaiharvester harvest starting');
  variable_set('oaiharvester_processing_cron', $last_last_run);
  variable_set('oaiharvester_cron_last', time());

  // The $operations array is empty if the schedule's skip_main_task field is 1
  // in that case we skip the harvesting process, only the additional data processing steps
  if (!empty($operations)) {
    oaiharvester_batch_set_add($operations);
  }

  // remove duplicated values from the additional batch sets
  $additional_batch_sets = xc_util_array_unique($additional_batch_sets);
  foreach ($additional_batch_sets as $batch_set) {
    $len = count($batch_set['operations']);
    for ($i = 0; $i < $len; $i++) {
      // add saved batch id, and operation_id as additional parameters
      array_push($batch_set['operations'][$i][1], $saved_batch->id, $operation_id++);
      array_push($operations, $batch_set['operations'][$i]);
    }
    batch_set($batch_set);
  }
  // save operations
  $saved_batch->sets = serialize($operations);
  drupal_write_record('oaiharvester_batch', $saved_batch, 'id');

  // Start!
  module_invoke_all('oaiharvester_harvest_starting', $schedule_ids);

  // set batch non prograssive
  $batch =& batch_get();
  xc_log_info('oaiharvester cron', 'the batch: ' . var_export($batch, TRUE));
  $batch['progressive'] = FALSE;

  batch_process('admin/xc/harvester/schedule/list');
} // oaiharvester_cron

/**
 * Gets the cron selection object, which helps to select the appropriate schedules to run
 *
 * @param $now (object)
 *   The current time as object
 * @param $last (object)
 *   The last run of cron
 * @param $distance (int)
 *   The distance between now and last running in seconds.
 *
 * @return (string)
 *   The SQL string to select schedules
 */
function oaiharvester_cron_get_selection_sql($now, $last, $distance) {

  $hours = '';
  $minutes = '';

  // TODO: handling minutes, only if the distance is less than an hour, else
  // we should run the hourly tasks anyway
  if ($distance->timestamp < OAIHARVESTER_MINUTE) {
    xc_log_info('oaiharvester cron', 'distance is less than a minute');
  }
  // calculates hours and minutes
  elseif ($distance->timestamp < OAIHARVESTER_HOUR) {
    xc_log_info('oaiharvester cron', 'distance is less than a hour');
    if ($last->hour != $now->hour) {
      $hours = sprintf(" AND hour IN (%d, %d)", $last->hour, $now->hour);
      $minutes = sprintf(" AND minute BETWEEN %d AND 59 OR minute BETWEEN 0 AND %d", $last->minute, $now->minute);
    }
    else {
      $hours = sprintf(" AND hour = %d", $now->hour);
      $minutes = sprintf(" AND minute BETWEEN %d AND %d", $last->minute, $now->minute);
    }
  }
  elseif ($distance->timestamp < OAIHARVESTER_DAY) {
    xc_log_info('oaiharvester cron', 'distance is less than a day');
    if ($last->hour > $now->hour) {
      $hours = sprintf(" AND (hour BETWEEN %d AND 23 OR hour BETWEEN 0 AND %d)", $last->hour, $now->hour);
    }
    elseif ($last->hour < $now->hour) {
      $hours = sprintf(" AND (hour BETWEEN %d AND %d)", $last->hour, $now->hour);
    }
  }

  // calculate the days
  if ($distance->month > 0 || $distance->day >= 8) {
    $days = range(1, 7);
  }
  else {
    if ($last->day_of_week > $now->day_of_week) {
      $days = array_merge(range($last->day_of_week, 7), range(1, $now->day_of_week));
    }
    else {
      $days = range($last->day_of_week, $now->day_of_week);
    }
  }
  $days = join(', ', $days);

  $curdate = (db_driver() == 'pgsql') ?  'current_date' : 'CURDATE()';

  $sql = "SELECT * FROM {oaiharvester_harvester_schedules}
    WHERE ($curdate >= DATE(start_date) AND $curdate <= DATE(end_date))
      AND status = '" . OAIHARVESTER_STATUS_PASSIVE . "'
      AND ((recurrence = 'Weekly' AND day_of_week IN (" . $days . ") " . $hours . ")
       OR (recurrence = 'Daily' " . $hours . ")
       OR (recurrence = 'Hourly' " . $minutes . "))";
  xc_log_info('oaiharvester cron', 'SQL: ' . $sql);

  return $sql;
}

/**
 * Checks whether the schedule is runnable.
 *
 * @param int $schedule_id
 *   The harvest schedule identifier
 */
function oaiharvester_schedule_is_runnable($schedule_id, $display_messages = TRUE) {
  $runnable_checks = module_invoke_all('oaiharvester_schedule_is_runnable', $schedule_id);

  $runnable = TRUE;
  if (isset($runnable_checks['runnable'])) {
    if ($runnable_checks['runnable'] === FALSE) {
      $runnable = FALSE;
      $message = t('Drupal is not able to launch harvest because of the following problem(s): ') . $runnable_checks['message'];
    }
  }
  else {
    foreach ($runnable_checks as $runnable_check) {
      if ($runnable_check['runnable'] === FALSE) {
        $runnable = FALSE;
        $message = t('Drupal is not able to launch harvest because of the following problem(s): ') . $runnable_check['message'];
      }
    }
  }

  if (isset($message)) {
    xc_log_error('harvester', $message);
    if ($display_messages) {
      drupal_set_message($message, 'error');
    }
  }

  return $runnable;
}

/**
 * Displays a confirmation form to manual schedule start
 *
 * @param $form_state (array)
 *   The FAPI form state
 * @param $schedule (object)
 *   The
 */
function oaiharvester_schedule_start_form($form, &$form_state, $schedule) {

  $runnable = oaiharvester_schedule_is_runnable($schedule->harvest_schedule_id);

  if ($runnable) {
    $form = array(
      'schedule' => array(
        '#type' => 'value',
        '#value' => $schedule,
      )
    );

    return confirm_form(
      $form,
      t('Are you sure, that you would like to run schedule %schedule_name?', array('%schedule_name' => $schedule->schedule_name)),
      'admin/xc/harvester/schedule/' . $schedule->harvest_schedule_id, // path to go if user click on 'cancel'
      t('This action cannot be undone.'),
      t('Start harvest'),
      t('Cancel')
    );
  }
}

/**
 * Handles the submission of the harvest launching confrimation form.
 *
 * It simply calls oaiharvester_schedule_start().
 *
 * @param $form (array)
 *   The FAPI form
 * @param $form_state (array)
 *   The FAPI form state
 */
function oaiharvester_schedule_start_form_submit($form, &$form_state) {
  $schedule = $form_state['values']['schedule'];
  oaiharvester_schedule_start($schedule, TRUE);
}

/**
 * Manually launch the harvester.
 *
 * @param $schedule (Object)
 *   The schedule object
 */
function oaiharvester_schedule_start($schedule) {
  xc_log_info('harvester', 'START manual harvesting');
  variable_del('oaiharvester_statistics');

  if (!isset($schedule->oai_provider_url)) {
    $schedule = _oaiharvester_schedule_get_full($schedule->harvest_schedule_id);
  }
  // set schedule status
  oaiharvester_schedule_set_active($schedule->harvest_schedule_id);

  // initializing saved batch
  $saved_batch = (object) array(
    'schedule_id' => $schedule->harvest_schedule_id,
    'timestamp' => time(),
  );
  drupal_write_record('oaiharvester_batch', $saved_batch);
  $operation_id = 0;

  $provider = oaiharvester_repository_load($schedule->provider_id);
  $operations = array();

  if ($provider->type == OAIHARVESTER_PROVIDERTYPE_SERVER) {
    // get steps
    if (!$schedule->skip_main_task) {
      $steps = _oaiharvester_schedule_steps_get($schedule->harvest_schedule_id);

      foreach ($steps as $step) {
        $set_spec = empty($step->set_spec) ? NULL : $step->set_spec;
        $last_ran = _oaiharvester_format_date($step->last_ran, $provider->granularity);
        $operations[] = oaiharvester_create_batch_harvester_operation(
          'manual',                       // launcher_type
          $schedule->harvest_schedule_id, // launcher_id
          $schedule->oai_provider_url,    // url
          $step->name,                    // metadataPrefix
          $step->set_spec,                // set
          $last_ran,                      // from
          NULL,                           // until
          $schedule->parsing_mode,        // parsing_mode
          $saved_batch->id,               // saved batch id
          $operation_id++,                // operation count
          NULL                            // resumption token
        );
      }
    }
  }
  elseif ($provider->type == OAIHARVESTER_PROVIDERTYPE_CACHE) {
    if (!$schedule->skip_main_task) {
      $operations[] = oaiharvester_create_batch_harvester_operation(
        'manual',                       // launcher_type
        $schedule->harvest_schedule_id, // launcher_id
        $schedule->oai_provider_url,    // url
        NULL,                           // metadataPrefix
        NULL,                           // set
        NULL,                           // from
        NULL,                           // until
        $schedule->parsing_mode,        // parsing_mode
        $saved_batch->id,               // saved batch id
        $operation_id++,                // operation count
        NULL                            // resumption token
      );
    }
  }

  // add operations
  if (!empty($operations)) {
    batch_set(oaiharvester_batch_set_create($operations));
  }

  $additional_sets = module_invoke_all('oaiharvester_additional_harvest_steps', $schedule->harvest_schedule_id);
  foreach ($additional_sets as $batch_set) {
    // adding identifiers to operations, and adding additional operations to others
    $c = count($batch_set['operations']);
    for ($i = 0; $i < $c; $i++) {
      array_push($batch_set['operations'][$i][1], $saved_batch->id, $operation_id++);
      array_push($operations, $batch_set['operations'][$i]);
    }
    batch_set($batch_set);
  }
  // save operations
  $saved_batch->sets = serialize($operations);
  drupal_write_record('oaiharvester_batch', $saved_batch, 'id');

  $target_url = 'admin/xc/harvester/schedule/' . $schedule->harvest_schedule_id . '/report/' . $saved_batch->id;

  // Start!
  module_invoke_all('oaiharvester_harvest_starting', $schedule->harvest_schedule_id);
  batch_process($target_url);
} // oaiharvester_schedule_start

/**
 * Continue/restart harvesting after stoped.
 * @param $saved_batch (stdClass)
 *   The saved batch object, stored in DB
 * @param $operation (int)
 *   Which operation is the first, the program should restart/continue
 * @param $resumption_token (String)
 *   The last resumptionToken
 */
function oaiharvester_schedule_continue($saved_batch, $operation = 0, $resumption_token = NULL) {
  xc_log_info('harvester', 'continue process');
  $sets = unserialize($saved_batch->sets);
  $c = count($sets);
  for ($i = $operation; $i < $c; $i++) {
    if ($i == $operation && $sets[$i][0] == 'oaiharvester_harvest_as_batch'
        && !is_null($resumption_token)) {
      // TODO: manipulate set: replace resumption token as the last parameter
      $sets[$i][1][10] = base64_decode($resumption_token);
    }

    if ($sets[$i][0] == 'oaiharvester_harvest_as_batch') {
      $set = oaiharvester_batch_set_create(array(($sets[$i])));
    }
    else {
      drupal_set_message('problem', 'error');
    }
    batch_set($set);
  }
  $saved_batch->sets = serialize($sets);
  drupal_write_record('oaiharvester_batch', $saved_batch, 'id');

  $target_url = 'admin/xc/harvester/schedule/' . $saved_batch->schedule_id . '/report/' . $saved_batch->id;
  batch_process($target_url);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function oaiharvester_schedule_restart($saved_batch, $operation) {
  echo 'Restart world!';
}

/**
 * Create an operation for batch harvesting
 *
 * @param $launcher_type (String)
 *   The launcher's type ('manual' or 'cron')
 * @param $launcher_id (int)
 *   The id of the launcher (schedule id or queue item id)
 * @param $url (String)
 *   The URL of OAI data provider
 * @param $metadata_prefix (String)
 *   The metadata prefix parameter for ListRecords call
 * @param $set (String)
 *   The set specification (set name)
 * @param $from
 *   The from date parameter
 * @param $until
 *   The until date parameter
 *
 * @return (Array)
 *   A simple array with key as the function (oaiharvester_harvest_as_batch) and
 *   a value as the parameters of that function
 */
function oaiharvester_create_batch_harvester_operation($launcher_type,
    $launcher_id, $url, $metadata_prefix, $set = NULL, $from = NULL, $until = NULL,
    $parsing_mode = 'dom', $saved_batch_id = NULL, $operation_id = NULL,
    $resumption_token = NULL) {
  return array(
    'oaiharvester_harvest_as_batch',
    array(
      $launcher_type,
      $launcher_id,
      $url,
      $metadata_prefix,
      $set,
      $from,
      $until,
      $parsing_mode,
      $saved_batch_id,
      $operation_id,
      $resumption_token,
    ),
  );
}

/**
 * Launch a batch harvester, which use oaiharvester_harvest_as_batch funtion
 * with different parameters to run multiple harvester schedules sequentially.
 * When all finished, it calls the oaiharvester_harvest_finished functions,
 * then redirects the browser to the list of schedules.
 *
 * @param $operations (Array)
 *   An array of operation. Each operation is a simple array, with a function
 *   name, and the list of parameters. We use the oaiharvester_harvest_as_batch
 *   funtion here.
 * @param $title (String)
 *   The title of the progress page. Only safe strings should be passed.
 * @param $init_message (String)
 *   The message displayed while the processing is initialized. Defaults to
 *   t('Initializing.').
 * @param $progress_message (String)
 *   Message displayed while processing the batch. Available placeholders are
 *   @current, @remaining, @total, @percentage, @estimate and @elapsed.
 *   Defaults to t('Completed @current of @total.').
 * @param $finishing_function (String)
 *   Name of a function to be executed after the batch has completed. This
 *   should be used to perform any result massaging that may be needed, and
 *   possibly save data in $_SESSION for display after final page redirection.
 * @param $ending_url
 *
 * @return (array)
 */
function _batch_harvester($operations, $title = 'Harvesting',
    $init_message = 'The harvesting process is beginning.',
    $progress_message = 'Keep on running.',
    $finishing_function = 'oaiharvester_harvest_finished',
    $ending_url = 'admin/xc/harvester/schedule/list') {

  variable_del('oaiharvester_statistics');
  oaiharvester_batch_set_add($operations, $title, $init_message, $progress_message, $finishing_function);
  // only needed if not inside a form _submit handler :
  batch_process($ending_url);
}

/**
 * Add batch set to the batch process.
 * @param $operations (Array) An array of operation. Each operation is a simple
 *   array, with a function name, and the list of parameters. We use the
 *   oaiharvester_harvest_as_batch funtion here.
 * @param $title (String) The title of the progress page. Only safe strings
 *   should be passed.
 * @param $init_message (String) The message displayed while the processing is
 *   initialized. Defaults to t('Initializing.').
 * @param $progress_message (String) Message displayed while processing the
 *   batch. Available placeholders are @current, @remaining, @total, @percentage,
 *   @estimate and @elapsed. Defaults to t('Completed @current of @total.').
 * @param $finishing_function (String) Name of a function to be executed after
 *   the batch has completed. This should be used to perform any result
 *   massaging that may be needed, and possibly save data in $_SESSION for
 *   display after final page redirection.
 * @return unknown_type
 */
function oaiharvester_batch_set_add($operations, $title = NULL, $init_message = NULL, $progress_message = NULL, $finishing_function = 'oaiharvester_harvest_finished') {

  if (is_null($title)) {
    $title = t('Harvesting and Processing');
  }
  if (is_null($init_message)) {
    $init_message = t('The harvesting process is beginning.');
  }
  if (is_null($progress_message)) {
    $progress_message = t('Keep on running.');
  }

  $batch = oaiharvester_batch_set_create($operations, $title, $init_message, $progress_message, $finishing_function);
  batch_set($batch);
}

/**
 * Add batch set to the batch process.
 * @param $operations (Array) An array of operation. Each operation is a simple
 *   array, with a function name, and the list of parameters. We use the
 *   oaiharvester_harvest_as_batch funtion here.
 * @param $title (String) The title of the progress page. Only safe strings
 *   should be passed.
 * @param $init_message (String) The message displayed while the processing is
 *   initialized. Defaults to t('Initializing.').
 * @param $progress_message (String) Message displayed while processing the
 *   batch. Available placeholders are @current, @remaining, @total, @percentage,
 *   @estimate and @elapsed. Defaults to t('Completed @current of @total.').
 * @param $finishing_function (String) Name of a function to be executed after
 *   the batch has completed. This should be used to perform any result
 *   massaging that may be needed, and possibly save data in $_SESSION for
 *   display after final page redirection.
 * @return (Array)
 *   An associative array containing the batch's properties (operations, title, init_message, progress_message, finished)
 */
function oaiharvester_batch_set_create($operations, $title = NULL, $init_message = NULL, $progress_message = NULL, $finishing_function = 'oaiharvester_harvest_finished') {

  if (is_null($title)) {
    $title = t('Harvesting');
  }
  if (is_null($init_message)) {
    $init_message = t('The harvesting process is beginning.');
  }
  if (is_null($progress_message)) {
    $progress_message = '';
  }

  $batch = array(
    'operations' => $operations,
    'title' => $title,
    'init_message' => $init_message,
    'progress_message' => $progress_message,
    'finished' => $finishing_function,
  );
  return $batch;
}


/**
 * Harvest the records in batch mode
 *
 * @param $launch_type (String)
 *   The type of lauch. ('manual' or 'cron')
 * @param $launcher_id (int)
 *   The id of the launcher (schedule id or queue id)
 * @param $url (String)
 *   The base URL to harvest
 * @param $metadata_prefix (String)
 *   The OAI metadataPrefix parameter
 * @param $set (String)
 *   The OAI set parameter
 * @param $from (Date String)
 *   The OAI from parameter
 * @param $until (Date String)
 *   The OAI until parameter
 * @param $parsing_mode (String)
 *   The mode of parsing of XML responses come from OAI server (possible values:
 *   'dom' or 'regex')
 * @param $saved_batch_id (int)
 *   The id of a record in oaiharvester_batch table
 * @param $operation_id (int)
 *   The id of the operation inside the current batch process
 * @param $resumption_token (string)
 *   The OAI-PMH resumptionToken
 * @param $context (Array)
 *   A context array to store different information. The $context['results'] is
 *   the same as the finished function's $result parameter.
 */
function oaiharvester_harvest_as_batch($launch_type = 'manual',
    $launcher_id = NULL, $url = NULL, $metadata_prefix = NULL, $set = NULL,
    $from = NULL, $until = NULL, $parsing_mode = 'dom', $saved_batch_id = -1,
    $operation_id = -1, $resumption_token = NULL, &$context = array()) {
  global $_oaiharvester_statistics, $_oaiharvester_is_initial_harvest, $_oaiharvester_need_debug;
  static $process_record_hooks;

  if (!isset($process_record_hooks)) {
    $process_record_hooks = array();
    $hook = 'oaiharvester_process_record';
    foreach (module_implements($hook) as $module) {
      $function = $module . '_' . $hook;
      $process_record_hooks[] = $function;
    }
  }

  drupal_set_time_limit(0);
  $current_batch = batch_get();
  $_oaiharvester_statistics = variable_get('oaiharvester_statistics', array('start' => microtime(TRUE)));

  $schedule_id = 0;
  if ($launch_type == 'manual') {
    $schedule_id = $launcher_id;
  }
  elseif ($launch_type == 'cron') {
    $queue_item = _oaiharvester_queue_get_item_by_id($launcher_id);
    $schedule_id = $queue_item->harvest_schedule_id;
  }
  $schedule = oaiharvester_schedule_load($schedule_id, TRUE);

  $saved_batch = oaiharvester_saved_batch_load($saved_batch_id);
  $sets = unserialize($saved_batch->sets);
  $is_first = FALSE;
  if (empty($context['sandbox'])) {
    $is_first = TRUE;
    $context['results'] = array(
      // storing parameters
      'parameters' => array(
        'launch_type' => $launch_type,
        'launcher_id' => $launcher_id,
        'url' => $url,
        'metadataPrefix' => $metadata_prefix,
        'set' => $set,
        'from' => $from,
        'until' => $until,
        'parsing_mode' => $parsing_mode,
        'saved_batch_id' => $saved_batch_id,
        'operation_id' => $operation_id,
        'resumptionToken' => $resumption_token,
        'schedule' => $schedule,
        'max_request' => (int) $schedule->max_request //variable_get('oaiharvester_max_request', FALSE),,
      ),
      // storing parameters
      'count' => 0,
      'request_count' => 0,
      'start_time' => microtime(TRUE),
      'total_time' => 0.0,
      'foreach_time' => 0.0,
      'request_time' => 0.0,
      'oai_build_args' => 0.0,
      'oai_response' => 0.0,
      'oai_dom' => 0.0,
      'resp_validate_verb' => 0.0,
      'resp_validate_url' => 0.0,
      'resp_curl_init' => 0.0,

      // computed values
      'saved_batch' => $saved_batch->sets,
      'schedule_id' => $schedule_id,
    );
    //
    $atomic_step = _oaiharvester_schedule_step_get_atomic($schedule_id, $metadata_prefix, $set, $schedule->type);
    $context['results']['parameters']['is_initial_harvest'] = is_null($atomic_step[0]->last_ran) ? TRUE : FALSE;

    $context['sandbox']['progress']       = 0;
    $context['sandbox']['start_time']     = time();
    $context['sandbox']['metadataPrefix'] = $metadata_prefix;

    // create HTTP response cache directory
    $context['sandbox']['http_cache'] = FALSE;
    $file_check_success = FALSE;
    if ($schedule->is_cacheable == 1) {
      $cache_dir = drupal_realpath(file_default_scheme() . '://oaiharvester_http_cache');
      $context['sandbox']['http_cache'] = $cache_dir . '/' . $schedule->harvest_schedule_id;
      if (!file_prepare_directory($cache_dir, FILE_CREATE_DIRECTORY)) {
        xc_log_error('harvester', t('The Drupal default "file" directory is not writable. Check out your Drupal settings, or turn off the caching of harvested responses. Your current file directory is %file.',
          array('%file' => $file_dir)));
      }
      else {
        $file_check_success = file_prepare_directory($context['sandbox']['http_cache'], FILE_CREATE_DIRECTORY);
        if (!$file_check_success) {
          xc_log_error('harvester', t('The OAI harvester\'s cache directory (%file) is not writable. Check out your Drupal settings, or turn off the caching of harvested responses.',
            array('%file' => $cache_dir)));
        }
      }
      if (!$file_check_success) {
        $context['sandbox']['http_cache'] = FALSE;
      }
    }

    // call hook_oaiharvester_batch_started($parameters)
    module_invoke_all('oaiharvester_batch_started', $context['results']['parameters']);
    $_oaiharvester_statistics['01 step1/01 oai/00 batch'] = 0;
  }
  else {
    xc_oaiharvester_statistics_set('01 step1/01 oai/00 batch', abs(microtime('TRUE') - $context['results']['batch_end']));
  }

  if ($is_first == TRUE && is_null($resumption_token)) {
    $params = array(
      'metadataPrefix' => $metadata_prefix,
    );
    if (!empty($set)) {
      $params['set'] = $set;
    }
    if (!empty($from)) {
      $params['from'] = $from;
    }
    if (!empty($until)) {
      $params['until'] = $until;
    }
  }
  else {
    $params = array(
      'resumptionToken' => (is_null($resumption_token)
        ? $context['results']['parameters']['resumptionToken']
        : $resumption_token),
    );
  }
  // TODO: remove it as soon as possible
  $_oaiharvester_need_debug = FALSE; // preg_match('/\|236\d000$/', $context['results']['parameters']['resumptionToken']);

  $_oaiharvester_is_initial_harvest = $context['results']['parameters']['is_initial_harvest'];
  $context['results']['request_count']++;
  $request = $url . '?' . http_build_query(array_merge_recursive(array('verb' => 'ListRecords'), $params));
  xc_log_info('harvester', 'Start downloading %url', array('%url' => $request));
  module_invoke_all('oaiharvester_request_started', $context['results']['parameters']);

  $context['message'] = ''; //$request;
  $t0 = microtime(TRUE);
  $download_phase_successfull = FALSE;
  require_once drupal_get_path('module', 'oaiharvester') . '/lib/OAIHarvester.php';
  $file = NULL;
  $fetch_mode = 'url';
  if ($schedule->type == OAIHARVESTER_PROVIDERTYPE_CACHE) {
    $fetch_mode = 'file';
    if ($is_first) {
      $file = 'initial.xml';
    }
    else {
      $file = str_replace('|', '_', $params['resumptionToken']) . '.xml';
    }
  }
  try {
    $harvester = new OAIHarvester('ListRecords', $url, $params, $context['sandbox']['http_cache']);
    $harvester->fetchContent($fetch_mode, $file);

    $i = 0;
    $max_retry = variable_get('oaiharvester_max_retry', 3);
    while ($harvester->getHttpCode() != 200 && $i < $max_retry) {
      $i++;
      $error = oaiharvester_harvest_get_fetch_error_message($harvester);
      xc_log_error('harvester', $error);

      xc_log_error('harvester', 'retrying pervious URL #' . $i);
      $harvester->fetchContent($fetch_mode, $file);
    }

    if ($harvester->getHttpCode() != 200) {
      $error = oaiharvester_harvest_get_fetch_error_message($harvester);
      $context['results']['error'] = $error;
      $context['finished'] = 1.0;
    }
    else {
      // $context['results'] = array_merge_recursive($context['results'], $rows);
      if ($harvester->getSize() == 0) {
        $msg = t('The data provider returned an empty string instead of a valid XML.');
        xc_log_error('harvester', $msg);
        xc_log_error('harvester', oaiharvester_harvest_get_fetch_error_message($harvester));
        $context['results']['error'] = $msg;
        $context['finished'] = 1;
      }
      else {
        $harvester->processContent($parsing_mode, $fetch_mode, $file);
        $t1 = microtime(TRUE);
        $context['results']['request_time']  += $t1 - $t0;
        $download_phase_successfull = TRUE;
      }
      $harvester->cacheContent();
    }
  }
  catch (Exception $e) {
    $t1 = microtime(TRUE);
    $context['results']['request_time'] += $t1 - $t0;
    $msg = t('Communication error during harvesting: %message (%request) -- Tech details: %tech_details.', array(
      '%message' => $e->getMessage(),
      '%request' => $request,
      '%tech_details' => var_export($e, TRUE),
    ));
    $context['results']['error'] = $msg;
    // drupal_set_message($msg, 'error');
    xc_log_error('harvester', $msg);
    $context['finished'] = 1;
  }

  if ($download_phase_successfull) {
    if ($harvester->hasErrors()) {
      //xc_log_info('harvester', 'has errors');
      foreach ($harvester->listErrors() as $error) {
        // TODO: check the structure of errors in both regex and DOM based methods
        // if (isset($error['attributes']['code'])) {
        // if ($error['attributes']['code'] == 'noRecordsMatch') {
        if (isset($error['code'])) {
          if (isset($error['code']) && $error['code'] == 'noRecordsMatch') {
            $msg = t("WARNING! No records match the requested criteria. Repository's message: %t. URL: %u",
              array(
                '%t' => $error['text'],
                '%u' => $request
            ));
            $context['results']['error'] = $msg;
            drupal_set_message($msg, 'warning');
            xc_log_warning('harvester', $msg);
          }
          else {
            drupal_set_message(var_export($error, TRUE), 'error');
            xc_log_error('harvester', 'error: ' . var_export($error, TRUE));
            $error_code = isset($error['code']) ? $error['code'] . ':' : '';
            $error_msg = isset($error['text']) ? $error['text'] : $error;
            $msg = t('Error while harvesting (%l). %c %t (%u)',
              array(
                // '%c' => $error['attributes']['code'],
                '%c' => $error_code,
                // '%t' => $error['text'],
                '%t' => $error_msg,
                '%u' => $request,
                '%l' => 'line: ' . __LINE__
            ));
            $context['results']['error'] = $msg;
            drupal_set_message($msg, 'error');
            xc_log_error('harvester', $msg);
          }
        }
        else {
          $msg = t('Error while harvesting (%l). %m (%u)', array('%m' => $error, '%u' => $request, '%l' => 'line: ' . __LINE__));
          $context['results']['error'] = $msg;
          drupal_set_message($msg, 'error');
          xc_log_error('harvester', $msg);
        }
      }
      $context['finished'] = 1;
    }
    // has no errors
    else {
      if ($harvester->hasRecords()) {
        $context['results']['count'] += $harvester->getRecordCount();
        $tf0 = microtime(TRUE);
        // Iterate over records
        try {
          while ($record = $harvester->getNextRecord()) {
            foreach ($process_record_hooks as $function) {
              $function($record, $schedule_id);
            }
          }
        }
        catch (Exception $e) {
          xc_log_error('harvester', 'Exception: ' . $e->getMessage() . ' ' . var_export($e, TRUE));
        }
        $context['results']['current_foreach_time'] = microtime(TRUE) - $tf0;
        $context['results']['foreach_time'] += $context['results']['current_foreach_time'];
      }
      else {
        drupal_set_message('No records!');
        xc_log_error('harvester', 'No records!');
      }

      $t_request_processed = microtime(TRUE);
      module_invoke_all('oaiharvester_request_processed', $context['results']['parameters'], $context['sandbox']);
      xc_oaiharvester_statistics_set('01 step1/01 oai/05 hook request_processed', abs(microtime(TRUE) - $t_request_processed));

      // preparing for the next request
      $context['finished'] = 0.5;
      $context['message'] .= t('Step %current of %total: Harvesting Metadata<br />',
        array(
          '%current' => $operation_id + 1, // because operation_id starts from 0
          '%total' => count($sets)
      ));

      xc_oaiharvester_statistics_set('01 step1/01 oai/01 fetch_all', $harvester->getStatistics('fetch'));
      xc_oaiharvester_statistics_set('01 step1/01 oai/03 oai_dom', $harvester->getStatistics('parse_response'));
      xc_oaiharvester_statistics_set('01 step1/01 oai/04 foreach', $context['results']['current_foreach_time']);
      xc_oaiharvester_statistics_set('01 step1/01 oai/04 foreach/01 records', $harvester->getStatistics('parse_records'));

      if ($harvester->hasResumptionToken()) {
        $resumption_token = $harvester->getResumptionToken();
        $time_elapsed = time() - (int) $context['sandbox']['start_time'];
        $current_process = (microtime(TRUE) - $t0);
        $time_remained = -1;
        $complete_list_size = t('unkown number of');
        $number_of_records = $context['results']['count'];
        if (isset($resumption_token['attributes'])) {
          $att = $resumption_token['attributes'];
          // handling cursor position if exists
          if (isset($att['completeListSize']) && $att['completeListSize'] != 0) {
            $complete_list_size = $att['completeListSize'];
            if (!empty($att['cursor'])) {
              $number_of_records = $att['cursor'] + $harvester->getRecordCount();
            }
            $percent           = $number_of_records / $complete_list_size;
            $time_remained     = (int) ($time_elapsed / $percent) - $time_elapsed;
            $context['finished'] = $percent;
          }
        }
        $context['message'] .= t('Harvested %num of %tot records.<br />' .
          'Elapsed time: %el, remained approx. %re, total approx.: %ato.<br />',
          array(
            '%num' => $number_of_records,
            '%tot' => $complete_list_size,
            '%el'  => oaiharvester_sec_to_time($time_elapsed),
            '%re'  => ($time_remained != -1)
              ? oaiharvester_sec_to_time($time_remained) : t('unknown'),
            '%ato' => ($time_remained != -1)
              ? oaiharvester_sec_to_time($time_elapsed + $time_remained)
              : t('unknown'),
        ));
        // xc_log_info('oaiharvester', $context['message']);

        // $parsing_time = $harvester->getStatistics('parse_response') + $harvester->getStatistics('parse_records');
        /*
         $message = t('Details of current request (in sec): whole process: %whl, response: %or (%per-or%), parse whole: %od (%per-od%), and individual records: %cft (%per-cft%) [mode: %mode]',
         array(
         '%mode'    => $parsing_mode,
         '%whl'     => sprintf('%.3f', $current_process),
         '%or'      => sprintf('%.3f', $harvester->getStatistics('fetch')),
         '%per-or'  => sprintf('%.1f', ($harvester->getStatistics('fetch') * 100 / $current_process)),
         '%od'      => sprintf('%.3f', $parsing_time),
         '%per-od'  => sprintf('%.1f', ($parsing_time * 100 / $current_process)),
         '%cft'     => sprintf('%.3f', $context['results']['current_foreach_time']),
         '%per-cft' => sprintf('%.1f', ($context['results']['current_foreach_time'] * 100 / $current_process)),
         ));
         xc_log_info('oaiharvester', $message);
         */
        $context['message'] .= t('Warning! Do not close this window untill harvesting is complete!') . '<br />';
        $link = l(t('Stop harvesting'), 'batch', array('query' => array(
          'op' => 'finished',
          'id' => $current_batch['id'],
        )));
        $context['message'] .= ' ' . $link . ' ' . t('(You will be able to restart or start over.)');
        xc_log_info('harvester', 'Next resumption token is: ' . $resumption_token['text']);
        $context['results']['parameters']['resumptionToken'] = $resumption_token['text'];
      }
      // no more resumptionToken
      else {
        xc_log_info('harvester', 'No more records to harvest');
        $context['finished'] = 1;
        // TODO: put an 'end of step' function to set the end date
      } // no more resumptionToken
    } // no errors
  } // download phase was successfull

  // TODO: use the following code to harvest only the first n lines.
  if ($context['results']['parameters']['max_request'] > 0) {
    xc_log_info('harvester', 'request_count: ' . $context['results']['request_count'] . ', max_request: ' . $context['results']['parameters']['max_request']);
    if ($context['results']['request_count'] >= $context['results']['parameters']['max_request']) {
      $context['finished'] = 1;
    }
  }

  if ($context['finished'] == 1) {
    $context['results']['end_time']   = microtime(TRUE);
    $context['results']['total_time'] = ($context['results']['end_time'] - $context['results']['start_time']);
    xc_log_info('harvester', 'download_phase_successfull: ' . (int) $download_phase_successfull);
    $_oaiharvester_statistics['01 step1'] = $context['results']['total_time'];
    _oaiharvester_step_processed(
      (!$download_phase_successfull || $harvester->hasErrors()),
      $context['results']['parameters'],
      $context['sandbox']['start_time']
    );
    $_oaiharvester_statistics['01 step1/01 oai'] = $_oaiharvester_statistics['01 step1/01 oai/00 batch']
      + $_oaiharvester_statistics['01 step1/01 oai/01 fetch_all']
      + $_oaiharvester_statistics['01 step1/01 oai/03 oai_dom']
      + $_oaiharvester_statistics['01 step1/01 oai/04 foreach'];

    $is_last_step = FALSE;
    $current_operation_id = -1;
    for ($i = 0, $max = count($sets); $i < $max; $i++) {
      if ($sets[$i][0] == 'oaiharvester_harvest_as_batch'
          && $sets[$i][1][9] == $operation_id) {
        $current_operation_id = $i;
        break;
      }
    }

    if ($current_operation_id > -1) {
      if ($sets[$current_operation_id + 1][0] != 'oaiharvester_harvest_as_batch') {
        $is_last_step = TRUE;
      }

      // TODO: documenting, that this runs only after the last step
      if ($is_last_step) {
        module_invoke_all('oaiharvester_batch_processed', $sets, $current_operation_id, $schedule_id);
      }
    }

    if (isset($saved_batch)) {
      $saved_batch_status = 'HARVEST_FINISHED';
    }
  }
  else {
    if (isset($saved_batch)) {
      $saved_batch_status = 'HARVEST_IN_PROGRESS';
    }
  }

  if (isset($saved_batch)) {
    oaiharvester_saved_batch_update_status($saved_batch, $operation_id, 'oaiharvester_harvest_as_batch',
      $saved_batch_status, $context);
  }
  $context['results']['batch_end'] = microtime('TRUE');
  variable_set('oaiharvester_statistics', $_oaiharvester_statistics);
  xc_log_info('harvester', 'Finished processing request %m. Processing rate: %r',
    array('%r' => $context['finished'], '%m' => $context['message']));
}

/**
 * Creates a fetch error message.
 *
 * @param $harvester (object)
 *   An OAIHarvester object
 */
function oaiharvester_harvest_get_fetch_error_message(&$harvester) {
  $output = t('The data provider had some trouble (HTTP %code) with responding the request !request.<br />', array(
    '%code' => $harvester->getHttpCode(),
    '!request' => l($harvester->getRequestUrl(), str_replace('&amp;', '&', $harvester->getRequestUrl())))
  );

  require_once('lib/class.CurlUtils.php');
  $CurlUtils = new CurlUtils();

  // add the HTTP request/response info
  $curl_info = $harvester->getCurlInfo();
  if (!empty($curl_info)) {
    $details = array();
    foreach ($curl_info as $key => $value) {
      $details[] = $CurlUtils->resolveInfoCode($key) . ': ' . $value;
    }
    $output .= t('Technical details:') . '<br />';
    $output .= t('Request header: %header', array('%header' => $harvester->getRequestHeader())) . '<br />';
    $output .= t('Response header: %header', array('%header' => $harvester->getHttpHeader())) . '<br />';
    $output .= t('cURL process details: %details', array('%details' => join(', ', $details))) . '<br />';
  }

  // add the cURL errors if any
  $fetch_error = $harvester->getFetchError();
  if (!empty($fetch_error) && !empty($fetch_error['text']) && !empty($fetch_error['code'])) {
    $resolved_error = $CurlUtils->resolveErrorCode($fetch_error['code']);
    if (!$resolved_error) {
      $fetch_error_text = $resolved_error['text'];
      if ($resolved_error['text'] != $fetch_error['text']) {
        $fetch_error_text .= ' (' . $fetch_error['text'] . ')';
      }
      $fetch_error_code = $fetch_error['code'] . ' (' . $resolved_error['code'] . ')';
    }

    $output .= ' ' . t('cURL Error message: %text, error code: %code.', array(
      '%text' => $fetch_error_text,
      '%code' => $fetch_error_code,
    ));
  }

  return $output;
}

/**
 * The function run after the main harvester batch operations.
 *
 * It invokes the hook_oaiharvester_harvest_finished() hook.
 *
 * @param $success (Boolean)
 *   TRUE if the batch operations were run correctly, FALSE, is it ran wrongly
 *   or interrupted
 * @param $results (Array)
 *   The content of the batch's $context['results'] array
 * @param $operations (Array)
 *   The remaining operations
 */
function oaiharvester_harvest_finished($success, $results, $operations) {
  global $_oaiharvester_statistics;

  $oaiharvester_statistics = variable_get('oaiharvester_statistics', array());

  // TODO: update $repository->last_harvest_end_time
  if (!isset($results['end_time'])) {
    $results['end_time'] = microtime(TRUE);
  }

  $saved_batch_id = $results['parameters']['saved_batch_id'];
  $operation_id = $results['parameters']['operation_id'];
  $saved_batch = oaiharvester_saved_batch_load($saved_batch_id);
  $saved_report = empty($saved_batch->reports) ? array() : unserialize($saved_batch->reports);

  xc_log_info('set passive', 'harvester@finished: set passive schedule #' . $saved_batch->schedule_id);
  oaiharvester_schedule_set_passive($saved_batch->schedule_id);

  $oaiharvester_statistics['end'] = microtime(TRUE);

  if ($success) {
    if (!empty($results['error'])) {
      drupal_set_message(check_plain($results['error']), 'error');
    }
    $saved_report['results'][] = t('%count records were harvested in %total_time', array(
      '%count' => $results['count'],
      '%total_time' => oaiharvester_sec_to_time($oaiharvester_statistics['end'] - $oaiharvester_statistics['start'])));
    $saved_report['tech_details'][] = l(
      t('Details of harvesting'),
      'admin/xc/harvester/schedule/' . $saved_batch->schedule_id . '/batch/' . $saved_batch_id
    );
  }
  else {
    if (!empty($results['error'])) {
      $saved_report['errors'] = $results['error'];
    }
    $saved_operations = unserialize($saved_batch->sets);
    // TODO: how to continue harvesting
    $continuable = FALSE;
    //if ($continuable) {
    $saved_report['actions'][] = l(t('Continue harvesting'),
      'admin/xc/harvester/schedule/continue/' . $saved_batch_id . '/' . $operation_id
      . '/' . base64_encode($results['parameters']['resumptionToken'])
    );
    //}
    //else {
    $saved_report['actions'][] = l(t('Restart harvesting'),
        'admin/xc/harvester/schedule/continue/' . $saved_batch_id . '/' . $operation_id);
    //}
    $c = count($saved_operations);
    for ($i = $operation_id; $i < $c; $i++) {
      $saved_report['pending_operations'][] = $saved_operations[$i][0]; // . '(' . join(', ', $saved_operations[$i][1]) . ')';
    }
  }

  $saved_report['actions'][] = t('Back to !url schedule', array(
    '!url' => l(
      $results['parameters']['schedule']->schedule_name,
      'admin/xc/harvester/schedule/' . $results['schedule_id']
    ),
  ));

  // TODO: find out how to delete by URL
  if (function_exists('_schedule_id_to_source_id')) {
    $source_id = _schedule_id_to_source_id($results['schedule_id']);
    $saved_report['actions'][] = l(t('Delete harvested records'),
      'admin/xc/metadata/delete_source/' . $source_id . '/' . (int) $results['start_time'] . '/' . (int) $results['end_time']);
  }

  $saved_report['actions'][] = t('Go to the !url', array(
    '!url' => l(t('schedule list'), 'admin/xc/harvester/schedule/list')
  ));

  // TODO: let's review what need from this, given the other statistics
  $statistics = t('results: %count  records were harvested'
    . ', the type of lauch was %launch_type'
    . ', id of laucher was %launcher_id'
    . ', total time: %total_time'
    . ', validate verb: %resp_validate_verb'
    . ', validate url: %resp_validate_url'
    . ', args time: %oai_build_args'
    . ', curl init: %resp_curl_init'
    . ', request time: %request_time'
    . ', response time: %oai_response'
    . ', dom time: %oai_dom'
    . ', foreach time: %foreach_time',
    array(
      '%count'              => $results['count'],
      '%launch_type'        => $results['parameters']['launch_type'],
      '%launcher_id'        => $results['parameters']['launcher_id'],
      '%total_time'         => sprintf("%01.2f", $results['total_time']),
      '%request_time'       => sprintf("%01.2f", $results['request_time']),
      '%oai_build_args'     => sprintf("%01.2f", $results['oai_build_args']),
      '%oai_response'       => sprintf("%01.2f", $results['oai_response']),
      '%oai_dom'            => sprintf("%01.2f", $results['oai_dom']),
      '%resp_validate_verb' => sprintf("%01.2f", $results['resp_validate_verb']),
      '%resp_validate_url'  => sprintf("%01.2f", $results['resp_validate_url']),
      '%foreach_time'       => sprintf("%01.2f", $results['foreach_time']),
      '%resp_curl_init'     => sprintf("%01.2f", $results['resp_curl_init'])
    )
  );
  $number_of_records = $results['count'];
  if ($number_of_records == 0) {
    $number_of_records = 1;
  }
  xc_log_info('oaiharvester', $statistics);

  $oaiharvester_statistics['end'] = microtime(TRUE);
  $oaiharvester_statistics['total'] = ($oaiharvester_statistics['end'] - $oaiharvester_statistics['start']);
  $oaiharvester_statistics['number_of_records'] = $number_of_records;
  variable_set('oaiharvester_statistics', $oaiharvester_statistics);
  $saved_report['oaiharvester_statistics'] = $oaiharvester_statistics;

  // TODO: let's separate the format into a different function
  if (!empty($oaiharvester_statistics)) {
    //$table = theme('oaiharvester_statistics', array('oaiharvester_statistics' => $oaiharvester_statistics));
    $table =  theme('table',  array('oaiharvester_statistics' => $oaiharvester_statistics));
    // TODO: save only the raw data here, and format on display time
    xc_log_info('oaiharvester',
      t('Statistics of harvesting @n records in %total_time', array(
      '@n' => $number_of_records,
      '%total_time' => oaiharvester_sec_to_time($results['total_time']),
    ))
      . $table);
  }
  xc_log_info('harvester', 'Harvesting has finished');

  variable_set('oaiharvester_processing_cron', 'NOT_RUNNING');

  $saved_batch->reports = serialize($saved_report);
  $ret = drupal_write_record('oaiharvester_batch', $saved_batch, 'id');
  $t_finished = microtime(TRUE);
  module_invoke_all('oaiharvester_harvest_finished', $success, $results, $operations);
  // TODO: check it out!
  $_oaiharvester_statistics['01 step1/01 oai/06 hook harvest_finished'] = abs(microtime(TRUE) - $t_finished);
}

/**
 * Load the saved batch from DB
 *
 * @param $id (int)
 *   The ID of the record
 * @param $field (string)
 *   Which field's content should be return? If NULL (default), the whole object
 *   will be returned. Fields: id, sets, reports.
 *
 * @return (mixed)
 *   If specify a field, the field's content will be returned, else (default), the whole object.
 *   The object's fields:
 *   - id
 *   - sets
 *   - reports
 *   - schedule_id
 *   - status
 *   - timestamp
 */
function oaiharvester_saved_batch_load($id, $field = NULL) {
  $query = db_select('oaiharvester_batch', 'ob');
  $data = $query->fields('ob')->condition('id', $id)->execute()->fetchObject();

  if (!empty($field) && in_array($field, array('sets', 'reports'))) {
    return unserialize($data->$field);
  }
  else {
    return $data;
  }
}

/**
 * Gets all oaiharvester batch objects.
 *
 * @return (array)
 *   The list of oaiharvester_batch records.
 */
function oaiharvester_saved_batch_get_all() {
  $result = db_query('SELECT * FROM {oaiharvester_batch}');
  $harvests = array();
  foreach ($result as $data) {
    $harvests[] = $data;
  }

  return $harvests;
}

/**
 * Gets the oaiharvester_batch objects belonging to a schedule.
 *
 * @param $schedule_id (int)
 *   The schedule's identifier
 *
 * @return (array)
 *   The list of oaiharvester_batch objects
 */
function oaiharvester_saved_batch_get_by_schedule_id($schedule_id) {
  $query = db_select('oaiharvester_batch', 'ob');
  $result = $query->fields('ob')->condition('schedule_id', $schedule_id)->execute();

  $batches = array();
  foreach ($result as $data) {
    $batches[] = $data;
  }
  return $batches;
}

/**
 * Gets the last oaiharvester_batch object belonging to a schedule.
 *
 * @param $schedule_id (int)
 *   The schedule's identifier
 *
 * @return (object)
 *   An oaiharvester_batch objects
 */
function oaiharvester_saved_batch_get_last_by_schedule_id($schedule_id) {
  return db_select('oaiharvester_batch', 'ob')
       ->fields('ob')
       ->condition('schedule_id', $schedule_id)
       ->range(0,1)
       ->orderBy('id', 'DESC')
       ->execute();
}

/**
 * Updates the status of saved batch
 *
 * @param $saved_batch (object)
 *   The save batch object
 * @param $operation_id (int)
 *   The current operation identifier
 * @param $function (String)
 *   The current function
 * @param $status (String)
 *   The status of the operation
 * @param $context (Object)
 *   The current context
 */
function oaiharvester_saved_batch_update_status($saved_batch, $operation_id, $function, $status, $context) {
  $saved_batch->status = serialize((object) array(
    'operation_id' => $operation_id,
    'function' => $function,
    'status' => $status,
    'context' => serialize($context),
  ));
  drupal_write_record('oaiharvester_batch', $saved_batch, 'id');
}

/**
 * List of reports
 * @param unknown_type $schedule_id
 */
function oaiharvester_schedule_report_list($schedule_id) {
  // TODO: check this against D7 API
  $sql = 'SELECT * FROM {oaiharvester_batch} WHERE schedule_id = %d';
  $result = db_query($sql, $schedule_id);
  $reports = array();
  while ($batch = db_fetch_object($result)) {
    $reports[] = l(format_date($batch->timestamp, 'custom', 'Y-m-d H:i'), 'admin/xc/harvester/schedule/' . check_plain($schedule_id) . '/report/' . $batch->id);
  }
  $output = theme('item_list', $reports, t('List of reports'));
  return $output;
}

/**
 * report, actions, tech_details
 *
 * @param $saved_batch (object)
 *
 * @return (string)
 *   The formatted schedule raport
 */
function oaiharvester_schedule_report($oaiharvester_saved_batch) {
  if (empty($oaiharvester_saved_batch)) {
    return '';
  }
  $report = unserialize($oaiharvester_saved_batch->reports);
  $categories = array(
    'errors' => t('Errors'),
    'pending_operations' => t('Pending operations'),
    'results' => t('Results'),
    'actions' => t('What do you want to do?'),
    'tech_details' => t('Technological details'),
  );

  $output = '';
  foreach ($categories as $category => $label) {
    if (!empty($report[$category])) {
      $output .= theme('item_list', array('items' => array_unique($report[$category]), 'title' => $label));
    }
  }

  return $output;
}
