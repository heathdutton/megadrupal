<?php
/**
 * @file
 * Provides administrative callbacks and forms for the Queue Runner module.
 */

/**
 * Page callback: Generates the Queue Runner settings page.
 */
function queue_runner_settings($form, &$form_state) {
  $form = array();
  $form['queue_runner_pause_between_tasks'] = array(
    '#type' => 'textfield',
    '#title' => t('Task interval'),
    '#description' => t('How long should a task wait after success until a new task is taken from the queue?'),
    '#default_value' => variable_get('queue_runner_pause_between_tasks', '5'),
  );
  $form['queue_runner_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Execution length'),
    '#description' => t('How long should Queue Runner run for?'),
    '#default_value' => variable_get('queue_runner_time', '55'),
  );
  $form['queue_runner_log_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Log file directory'),
    '#description' => t('Enter a system path where the logs will be written.'),
    '#default_value' => variable_get('queue_runner_log_path', file_directory_temp() . '/queue_runner'),
  );
  $form['queue_runner_reports_oldest'] = array(
    '#type' => 'textfield',
    '#title' => t('Oldest stats record'),
    '#description' => t('Age in seconds of the oldest stats record to keep.'),
    '#default_value' => variable_get('queue_runner_reports_oldest', 1209600),
  );
  $form['queue_runner_reports_show_hours'] = array(
    '#type' => 'textfield',
    '#title' => t('Hours to display'),
    '#description' => t('Number of hours to display stats for in the overview report.'),
    '#default_value' => variable_get('queue_runner_reports_num_hours', 10),
  );
  return system_settings_form($form);
}

/**
 * Page callback: Provides a form to make debugging easier.
 */
function queue_runner_debug() {
  $build = array();
  $build[] = drupal_get_form('queue_runner_run_form');
  $build[] = drupal_get_form('queue_runner_item_form');
  $build[] = drupal_get_form('queue_runner_coll_form');
  return $build;
}

/**
 * Form constructor for running the Queue Runner queue.
 *
 * @see queue_runner_run_form_submit()
 *
 * @ingroup forms
 */
function queue_runner_run_form($form, &$form_state) {
  $form = array();

  $form['top'] = array(
    '#type' => 'fieldset',
    '#title' => t('Process one item'),
  );
  $form['top']['item_run'] = array(
    '#type' => 'submit',
    '#value' => t('Run queue'),
  );

  return $form;
}

/**
 * Form submission handler for queue_runner_item_form().
 *
 * Runs the queue.
 */
function queue_runner_run_form_submit($form, &$form_state) {
  $processed = queue_runner_do();
  if ($processed) {
    drupal_set_message(t('Ran one queue entry'));
  }
  else {
    drupal_set_message(t('It seems there are no more items in the queue'));
  }
}

/**
 * Form constructor for creating test Queue Runner items.
 *
 * @see queue_runner_item_form_submit()
 *
 * @ingroup forms
 */
function queue_runner_item_form($form, &$form_state) {
  $form = array();

  $form['item'] = array(
    '#type' => 'fieldset',
    '#title' => t('Submit test item'),
    '#tree' => TRUE,
  );
  $options = array(
    'success' => t('Success'),
    'failed' => t('Failed'),
  );
  $form['item']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for the test item'),
    '#required' => TRUE,
  );
  $form['item']['state'] = array(
    '#type' => 'select',
    '#title' => t('Final state'),
    '#options' => $options,
    '#default_value' => 'success',
  );
  $form['item']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Form submission handler for queue_runner_item_form().
 */
function queue_runner_item_form_submit($form, &$form_state) {
  $data = array();
  $values = $form_state['values']['item'];

  $data['title'] = $values['title'];
  $data['worker']['callback'] = 'queue_runner_test_worker';
  $data['state'] = $values['state'];

  queue_runner_add($data);
  drupal_set_message(t('Created queue item with title %title', array('%title' => $data['title'])));
}

/**
 * Form constructor for creating test Queue Runner collector.
 *
 * @see queue_runner_coll_form_submit()
 *
 * @ingroup forms
 */
function queue_runner_coll_form($form, &$form_state) {
  $form['coll'] = array(
    '#type' => 'fieldset',
    '#title' => t('Submit test parallel collector'),
    '#tree' => TRUE,
  );
  $form['coll']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for the test collector'),
    '#required' => TRUE,
  );
  $form['coll']['item_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for the sub tasks'),
    '#required' => TRUE,
  );
  $form['coll']['count'] = array(
    '#type' => 'select',
    '#title' => t('Number of sub tasks'),
    '#options' => drupal_map_assoc(range(1, 10)),
    '#default_value' => 2,
  );
  $form['coll']['type'] = array(
    '#type' => 'select',
    '#options' => array(
      'parallel_collector' => t('Parallel collector'),
      'serial_collector' => t('Serial collector'),
      'bounded_collector' => t('Bounded collector'),
    ),
  );
  $form['coll']['number'] = array(
    '#type' => 'select',
    '#title' => t('Number of sub tasks to keep simultaneously in the queue'),
    '#options' => drupal_map_assoc(range(1, 10)),
    '#default_value' => 2,
    '#states' => array(
      'visible' => array(
        'select[id=edit-coll-type]' => array('value' => 'bounded_collector')
      ),
    ),
  );
  $form['coll']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Form submission handler for queue_runner_coll_form().
 */
function queue_runner_coll_form_submit($form, &$form_state) {
  $values = $form_state['values']['coll'];
  $data = $values;

  for ($id = 1; $id <= $values['count']; $id++) {
    $task = array();
    $task['data']['title'] = "Subtask $id: " . $values['item_title'];
    // Make use of a static function as the worker callable.
    $task['data']['worker']['callback'] = 'queue_runner_test_worker';
    $data['subtasks'][] = $task;
  }

  queue_runner_add($data, $values['type']);
  drupal_set_message(t('Created collector with title %title', array('%title' => $values['title'])));
}

/**
 * Page callback: Displays an individual log event.
 */
function queue_runner_event($dblog) {
  $severity = watchdog_severity_levels();
  $rows = array(
    array(
      array(
        'data' => t('Date'),
        'header' => TRUE,
      ),
      format_date($dblog->timestamp, 'long'),
    ),
    array(
      array(
        'data' => t('Message'),
        'header' => TRUE,
      ),
      theme('queue_runner_event', array('event' => $dblog)),
    ),
    array(
      array(
        'data' => t('Severity'),
        'header' => TRUE,
      ),
      $severity[$dblog->severity],
    ),
    array(
      array(
        'data' => t('Hostname'),
        'header' => TRUE,
      ),
      check_plain($dblog->hostname),
    ),
    array(
      array(
        'data' => t('Operations'),
        'header' => TRUE,
      ),
      $dblog->link,
    ),
  );
  $build['dblog_table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#attributes' => array('class' => array('dblog-event')),
  );
  return $build;
}

/**
 * Form constructor: Confirms an action for a particular task.
 *
 * @param string $action
 *   The action to perform.
 * @param object $item
 *   The item object for the task to perform the action on.
 *
 * @see queue_runner_task_action_confirm_form_submit()
 *
 * @ingroup forms
 */
function queue_runner_task_action_confirm_form($form, &$form_state, $action, $item) {
  // Stash the item and action to perform in the form.
  $form['#item'] = $item;
  $form['#task_action'] = $action;

  return confirm_form($form, t('Are you sure you want to @action this failed task?', array('@action' => $action)), 'admin/reports/queue-runner');
}

/**
 * Form submission handler for queue_runner_task_action_comfirm_form().
 */
function queue_runner_task_action_confirm_form_submit($form, $form_state) {
  $item = $form['#item'];
  $action = $form['#task_action'];

  switch ($action) {
    case 'retry':
      $item_id = queue_runner_add($item->data);
      if ($item->coll_id) {
        // Associate the retried item with the original collector
        _queue_runner_collector_relate($item->coll_id, $item_id);
      }
      // Fall through.
    case 'cancel':
      db_delete('queue_runner_failures')
        ->condition('item_id', $item->item_id)
        ->execute();
        // @todo log something here?
      break;
  }
}

/**
 * Form constructor: Confirms whether a given collector should be canceled.
 *
 * @param object $collector
 *   The collector object to cancel.
 *
 * @see queue_runner_collector_cancel_confirm_form_submit()
 *
 * @ingroup forms
 */
function queue_runner_collector_cancel_confirm_form($form, &$form_state, $collector) {
  $form['#collector'] = $collector;

  return confirm_form($form, t('Are you sure you want to cancel collector @coll_id?', array('@coll_id' => $collector->coll_id)), 'admin/reports/queue-runner');
}

/**
 * Form submission handler for queue_runner_collector_cancel_confirm_form().
 */
function queue_runner_collector_cancel_confirm_form_submit($form, $form_state) {
  queue_runner_collector_cancel_recursive($form['#collector']);
}

/**
 * Page callback for overview report.
 */
function queue_runner_overview_report() {
  $queue = queue_runner_queue();
  $count = $queue->numberOfItems();

  $build['count']['#markup'] = t("@count items in the Queue Runner queue", array('@count' => $count));

  date_default_timezone_set('UTC');
  $header = array(t('Hour starting'), t('Completed tasks'), t('Max duration (sec)'), t('Average duration (sec)'), t('Max retries'), t('Num failures'));

  $current_hour = _queue_runner_get_current_hour();
  $hours_to_show = variable_get('queue_runner_reports_num_hours', 10);

  // Show the hourly stats for each hour today.
  $result = db_query_range("SELECT * FROM {queue_runner_overview} WHERE hour_ts <= :hour ORDER BY hour_ts DESC", 0, $hours_to_show, array(':hour' => $current_hour), array('fetch' => PDO::FETCH_ASSOC));
  $rows = array();
  foreach ($result as $row) {
    // Show a formatted date instead of the timestamp.
    $date = date('d M Y H:i:s', $row['hour_ts']);
    unset($row['hour_ts'], $row['period_seconds']);
    $rows[] = array('date' => $date) + $row;
  }
  // @todo add specific class.
  $build['hourly_table']['#attributes'] = array();
  if ($rows) {
    $build['hourly_table']['#markup'] = theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $build['hourly_table']['#markup'] = t('No task data available for the past @hours hours', array('@hours' => $hours_to_show));
    $build['hourly_table']['#theme_wrappers'] = array('container');
  }

  // Now the daily averages.
  $ts = time();
  $midnight = $ts - ($ts % 86400);
  $records = db_query("SELECT * FROM {queue_runner_overview} WHERE hour_ts < :hour ORDER BY hour_ts DESC", array(":hour" => $midnight), array('fetch' => PDO::FETCH_ASSOC))->fetchAll();
  // Create day records for this time period.
  $rows = _queue_runner_reaverage_daily($records, 'd M Y');
  // @todo add specific class.
  $build['daily_table']['#attributes'] = array();
  if ($rows) {
    $header[0] = t('Date');
    $build['daily_table']['#markup'] = theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $build['daily_table']['#markup'] = t('No task data available for prior days');
    $build['daily_table']['#theme_wrappers'] = array('container');
  }
  return $build;
}

/**
 * Finds all valid options for accessing file logs.
 *
 * @return array
 *   An array of date options with days, months, and years as keys.
 */
function queue_runner_file_log_dates_get() {
  $dates = array();

  $path = variable_get('queue_runner_log_path', file_directory_temp() . '/queue_runner');

  $directories = file_scan_directory($path, '/\d{4}-\d{2}-\d{2}/', array('recurse' => FALSE));

  foreach ($directories as $directory) {
    $timestamp = strtotime($directory->name);
    $day = date('d', $timestamp);
    $month = date('m', $timestamp);
    $year = date('Y', $timestamp);
    $dates['days'][$day] = $day;
    $dates['months'][$month] = $month;
    $dates['years'][$year] = $year;
  }

  return $dates;
}

/**
 * Form constructor function for selecting logs by date.
 *
 * @param string $method
 *   The log viewing method; either "recent", "paged", or "collector".
 * @param string $date
 *   A date string in the format YYYY-MM-DD.
 * @param string $type
 *   The type of log to view (can be "queue" or "error").
 *
 * @see queue_runner_file_log_date_form_submit()
 *
 * @ingroup forms
 */
function queue_runner_file_log_date_form($form, &$form_state, $method, $date, $type, $show_date = TRUE) {
  $form = array();

  $date_parts = explode('-', queue_runner_file_log_date_get($date));
  $dates = queue_runner_file_log_dates_get();

  if (!empty($dates['days'])) {
    $form['#attributes']['class'] = array('container-inline');
    $form['day'] = array(
      '#type' => 'select',
      '#title' => t('Day'),
      '#options' => $dates['days'],
      '#access' => $show_date,
      '#default_value' => $date_parts[2],
    );
    $form['month'] = array(
      '#type' => 'select',
      '#title' => t('Month'),
      '#options' => $dates['months'],
      '#access' => $show_date,
      '#default_value' => $date_parts[1],
    );
    $form['year'] = array(
      '#type' => 'select',
      '#title' => t('Year'),
      '#options' => $dates['years'],
      '#access' => $show_date,
      '#default_value' => $date_parts[0],
    );
    $form['type'] = array(
      '#type' => 'select',
      '#title' => ('Type'),
      '#options' => array(
        'queue' => t('Main log'),
        'error' => t('Error log'),
      ),
      '#default_value' => $type,
    );
    $form['method'] = array(
      '#type' => 'value',
      '#value' => $method,
    );
    if ($method == 'collector') {
      $form['coll_id'] = array(
        '#type' => 'value',
        '#value' => arg(5),
      );
    }
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
  }

  return $form;
}

/**
 * Form submission handler for queue_runner_file_log_date_form().
 */
function queue_runner_file_log_date_form_submit($form, &$form_state) {
  switch ($form_state['values']['method']) {
    case 'paged':
      // paged/YYYY-MM-DD/(queue|error)
      $fragments = array(
        $form_state['values']['method'],
        implode('-', array(
          $form_state['values']['year'],
          $form_state['values']['month'],
          $form_state['values']['day'],
        )),
        $form_state['values']['type'],
      );
      break;

    case 'recent':
      // recent/(queue|error)
      $fragments = array(
        $form_state['values']['method'],
        $form_state['values']['type'],
      );
      break;

    case 'collector':
      // collector/[coll_id]/(queue|error)
      $fragments = array(
        $form_state['values']['method'],
        $form_state['values']['coll_id'],
        $form_state['values']['type'],
      );
      break;
  }

  $form_state['redirect'] = QUEUE_RUNNER_ADMIN_PATH . '/file-log/' . implode('/', $fragments);
}

/**
 * Form constructor for paging through the log files.
 *
 * @param string $method
 *   The log viewing method; either "recent", "paged", or "collector".
 * @param string $date
 *   A date string in the format YYYY-MM-DD.
 * @param string $type
 *   The type of log. Can be "queue" for the main log file, or "error" for the
 *   error log file.
 * @param int $page
 *   The current page number.
 * @param int $items
 *   The number of items to display per page.
 * @param bool $page_controls
 *   Allow access to page controls
 *
 * @see queue_runner_file_log_page_form_submit()
 *
 * @ingroup forms
 */
function queue_runner_file_log_page_form($form, &$form_state, $method, $date, $type, $page, $items, $page_controls = TRUE) {
  $form = array();

  $date = queue_runner_file_log_date_get($date);

  $page = !empty($page) ? $page : 1;
  $previous = $page - 1;
  $next = $page + 1;

  if ($method == 'collector') {
    $coll_id = arg(5);
    $path_prefix = QUEUE_RUNNER_ADMIN_PATH . "/file-log/$method/$coll_id/$type";
  }
  else {
    $path_prefix = QUEUE_RUNNER_ADMIN_PATH . "/file-log/$method/$date/$type";
  }

  $form['#attributes']['class'] = array('container-inline');
  $form['page'] = array(
    '#type' => 'textfield',
    '#title' => t('Page'),
    '#size' => 4,
    '#access' => $page_controls,
    '#default_value' => $page,
  );
  $form['items'] = array(
    '#type' => 'textfield',
    '#title' => t('Items'),
    '#size' => 4,
    '#maxlength' => 4,
    '#default_value' => $items,
  );
  if ($page > 1) {
    $form['previous'] = array(
      '#type' => 'link',
      '#href' => "$path_prefix/$previous/$items",
      '#title' => t('‹ Previous'),
      '#access' => $page_controls,
      '#attributes' => array('class' => array('queue-runner-file-log-previous-link')),
    );
  }
  $form['next'] = array(
    '#type' => 'link',
    '#href' => "$path_prefix/$next/$items",
    '#title' => t('Next ›'),
    '#access' => $page_controls,
    '#attributes' => array('class' => array('queue-runner-file-log-next-link')),
  );
  $form['date'] = array(
    '#type' => 'value',
    '#value' => $date,
  );
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  $form['method'] = array(
    '#type' => 'value',
    '#value' => $method,
  );
  if ($method == 'collector') {
    $form['coll_id'] = array(
      '#type' => 'value',
      '#value' => $coll_id,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
  );

  return $form;
}

/**
 * Form submission handler for queue_runner_file_log_page_form().
 */
function queue_runner_file_log_page_form_submit($form, &$form_state) {
  switch ($form_state['values']['method']) {
    case 'paged':
      // paged/YYYY-MM-DD/(queue|error)/[page]/[items]
      $fragments = array(
        $form_state['values']['method'],
        $form_state['values']['date'],
        $form_state['values']['type'],
        $form_state['values']['page'],
      );
      break;

    case 'recent':
      // recent/(queue|error)/[items]
      $fragments = array(
        $form_state['values']['method'],
        $form_state['values']['type'],
      );
      break;

    case 'collector':
      // collector/[coll_id]/(queue|error)/[page]/[items]
      $fragments = array(
        $form_state['values']['method'],
        $form_state['values']['coll_id'],
        $form_state['values']['type'],
        $form_state['values']['page'],
      );
      break;
  }

  if (!empty($form_state['values']['items'])) {
    $fragments[] = $form_state['values']['items'];
  }

  $form_state['redirect'] = QUEUE_RUNNER_ADMIN_PATH . '/file-log/' . implode('/', $fragments);
}

/**
 * Page callback: Displays the most recent log entries.
 *
 * @param string $type
 *   The type of log. Can be "queue" for the main log file, or "error" for the
 *   error log file.
 * @param int $items
 *   An optional number of items to display per page.
 *
 * @return array
 *   A renderable array of content for the page.
 */
function queue_runner_file_log_most_recent_page($type = 'queue', $items = NULL) {
  $build = array();

  // Make sure type is an expected value to prevent file traversal.
  $type = ($type == 'error') ? 'error' : 'queue';

  $build['#attached']['css'] = array(
    drupal_get_path('module', 'queue_runner') . '/css/queue-runner.css',
  );

  // Only allow items to be less than 1000.
  if (empty($items) || $items > 1000) {
    $items = QUEUE_RUNNER_FILE_LOG_DEFAULT_RESULTS_PER_PAGE;
  }

  $date = queue_runner_file_log_date_get();
  $path = variable_get('queue_runner_log_path', file_directory_temp() . '/queue_runner');
  $filepath = $path . '/' . $date . '/' . $type . '.log';

  $build['toolbar'] = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array('class' => array('queue-runner-file-log-toolbar', 'clearfix')),
  );
  $build['toolbar']['date_form'] = drupal_get_form('queue_runner_file_log_date_form', 'recent', $date, $type, FALSE);

  if (file_exists($filepath)) {
    $build['toolbar']['page_form'] = drupal_get_form('queue_runner_file_log_page_form', 'recent', $date, $type, 1, $items, FALSE);

    $rows = queue_runner_file_log_last_n_csv_get($type, $items);
    if (!empty($rows)) {
      $build['table'] = queue_runner_file_log_table($rows);
    }
  }
  else {
    $type = ($type == 'queue') ? 'main' : 'error';
    $build['no_results'] = array(
      '#theme_wrappers' => array('container'),
      '#attributes' => array('class' => array('queue-runner-file-log-no-results')),
      '#markup' => t('There are no entries in the @type log file so far today.', array('@type' => $type)),
    );
  }

  return $build;
}

/**
 * Page callback: Displays a paged file log for a given date.
 *
 * @param string $date
 *   A date string in the format YYYY-MM-DD.
 * @param string $type
 *   The type of log. Can be "queue" for the main log file, or "error" for the
 *   error log file.
 * @param int $page
 *   The current page number.
 * @param int $num_items
 *   An optional number of items to display per page.
 *
 * @return array
 *   A renderable array of content for the page.
 */
function queue_runner_file_log_paged_page($date = NULL, $type = 'queue', $page = 1, $num_items = NULL) {
  $build = array();

  // Make sure type is an expected value to prevent file traversal.
  $type = ($type == 'error') ? 'error' : 'queue';

  $build['#attached']['css'] = array(
    drupal_get_path('module', 'queue_runner') . '/css/queue-runner.css',
  );

  // Only allow items to be less than 1000.
  if (empty($num_items) || $num_items > 1000) {
    $num_items = QUEUE_RUNNER_FILE_LOG_DEFAULT_RESULTS_PER_PAGE;
  }

  $date = queue_runner_file_log_date_get($date);
  $path = variable_get('queue_runner_log_path', file_directory_temp() . '/queue_runner');
  $filepath = $path . '/' . $date . '/' . $type . '.log';

  $build['toolbar'] = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array('class' => array('queue-runner-file-log-toolbar', 'clearfix')),
  );
  $build['toolbar']['date_form'] = drupal_get_form('queue_runner_file_log_date_form', 'paged', $date, $type);

  if (file_exists($filepath)) {

    $rows = queue_runner_file_log_paged_csv_get($filepath, $num_items, $page);
    $build['toolbar']['page_form'] = drupal_get_form('queue_runner_file_log_page_form', 'paged', $date, $type, $page, $num_items);

    if (!empty($rows)) {
      $build['table'] = queue_runner_file_log_table($rows);
    }
  }
  else {
    $type = ($type == 'queue') ? 'main' : 'error';
    $build['no_results'] = array(
      '#theme_wrappers' => array('container'),
      '#attributes' => array('class' => array('queue-runner-file-log-no-results')),
      '#markup' => t('There is no @type log file for @date.', array('@type' => $type, '@date' => $date)),
    );
  }

  return $build;
}

/**
 * Generates a file log table from an array of rows.
 *
 * @param array $rows
 *   An array of rows for passing to theme_table().
 *
 * @return array
 *   A renderable array respresenting a table of log data.
 */
function queue_runner_file_log_table($rows) {
  $nids = array();
  foreach ($rows as $index => &$cells) {
    // Split the ISO 8601 date into separate date and time colums.
    $iso_date = $cells[0];
    $timestamp = strtotime($iso_date);
    $date = date('Y-m-d', $timestamp);
    $time = date('H:i', $timestamp);
    $cells[0] = $time;
    array_unshift($cells, $date);

    // Link the collector to its associated log.
    if (!empty($cells[6])) {
      $cells['6'] = l($cells['6'], QUEUE_RUNNER_ADMIN_PATH . '/file-log/collector/' . $cells['6']);
    }
    if (!empty($cells[7])) {
      $nids[$index] = $cells[7];
    }
    else {
      // Some of the nid columns contain "0" - reset these to empty.
      $cells[7] = '';
    }
  }

  if (!empty($nids)) {
    $titles = _queue_runner_node_titles($nids);
    foreach ($nids as $index => $nid) {
      if (isset($titles[$nid])) {
        $rows[$index][7] = l($titles[$nid], "node/$nid");
      }
    }
  }

  return array(
    '#theme' => 'table',
    '#header' => array(
      t('Date'),
      t('Time'),
      t('Message'),
      t('Severity'),
      t('Hostname'),
      t('Item'),
      t('Collector'),
      t('Node'),
      t('Link'),
    ),
    '#rows' => $rows,
  );
}

/**
 * Parses the last N lines from potentially large CSV files to rows.
 *
 * @param string $filepath
 *   The system path to the log file to parse.
 * @param int $lines
 *   The number of lines from the end of the file to retrieve.
 * @param string $date_string
 *   An optional date string to use for recursion.
 *
 * @return array
 *   An array of rows for passing to theme_table().
 */
function queue_runner_file_log_last_n_csv_get($type, $items, $date_string = NULL) {
  $rows = array();

  // Make sure type is an expected value to prevent file traversal.
  $type = ($type == 'error') ? 'error' : 'queue';

  // Assume a large maximum line length. This should be way above what we
  // require as we need to ensure we get enough lines.
  $bytes = $items * QUEUE_RUNNER_FILE_LOG_MAX_LINE_LENGTH;


  $date_string = queue_runner_file_log_date_get($date_string);
  $path = variable_get('queue_runner_log_path', file_directory_temp() . '/queue_runner');
  $filepath = $path . '/' . $date_string . '/' . $type . '.log';

  $rows = array();

  if (file_exists($filepath)) {
    if (($file = fopen($filepath, 'r')) !== FALSE) {
      // Attempt to move the file pointer to somewhere before the number of
      // lines we're trying to retrieve.
      fseek($file, -$bytes, SEEK_END);

      while ($row = fgetcsv($file, QUEUE_RUNNER_FILE_LOG_MAX_LINE_LENGTH, QUEUE_RUNNER_FILE_LOG_CSV_DELIMITER)) {
        $rows[] = array_pad($row, 8, '');
      }

      // Keep only the last N lines.
      $rows = array_slice($rows, -$items);

      fclose($file);

      // If the file was smaller than we initially needed, get more rows.
      if (filesize($filepath) < $bytes) {
        // See how many further items we need.
        $items = $items - count($rows);

        // If we don't need at least 1 more item, return the rows.
        if ($items < 1) {
          return $rows;
        }

        // Get the previous date.
        $date_string = gmdate('Y-m-d', strtotime('-1 day', strtotime($date_string)));

        // Get some rows from the previous day and prepend them to the rows
        // we've already collected.
        $previous_rows = queue_runner_file_log_last_n_csv_get($type, $items, $date_string);
        $rows = array_merge($previous_rows, $rows);
      }
    }
  }

  return $rows;
}

/**
 * Page callback: Displays a collector's log file.
 *
 * @param int $coll_id
 *   The collector ID.
 * @param string $type
 *   The type of log. Can be "queue" for the main log file, or "error" for the
 *   error log file.
 * @param int $page
 *   The current page number.
 * @param int $num_items
 *   The number of items per page.
 *
 * @return array
 *   A renderable array representing the log data for the given collector.
 */
function queue_runner_file_log_collectors($coll_id, $type = 'queue', $page = 1, $num_items = NULL) {
  $build = array();
  $build['#attached']['css'] = array(
    drupal_get_path('module', 'queue_runner') . '/css/queue-runner.css',
  );

  $build['toolbar'] = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array('class' => array('queue-runner-file-log-toolbar', 'clearfix')),
  );
  $build['toolbar']['date_form'] = drupal_get_form('queue_runner_file_log_date_form', 'collector', NULL, $type, FALSE);
  $build['toolbar']['page_form'] = drupal_get_form('queue_runner_file_log_page_form', 'collector', NULL, $type, $page, $num_items);

  $rows = queue_runner_file_get_log_collectors($coll_id, $type = 'queue', $page = 1, $num_items = NULL);
  if (!empty($rows)) {
    $build['toolbar']['download_link'] = array(
      '#theme_wrappers' => array('container'),
      '#type' => 'link',
      '#href' => QUEUE_RUNNER_ADMIN_PATH . '/file-log-download/collector/' . $coll_id,
      '#title' => t('Download full log file'),
      '#attributes' => array('class' => array('queue-runner-file-download')),
    );
    $build['table'] = queue_runner_file_log_table($rows);
  }
  else {
    $type = ($type == 'queue') ? 'main' : 'error';
    $build['no_results'] = array(
      '#theme_wrappers' => array('container'),
      '#attributes' => array('class' => array('queue-runner-file-log-no-results')),
      '#markup' => t('There is no @type log file for this collector.', array('@type' => $type)),
    );
  }

  return $build;
}

/**
 * Page callback: Download a collector's log file.
 *
 * @param int $coll_id
 *   The collector ID.
 * @param string $type
 *   The type of log. Can be "queue" for the main log file, or "error" for the
 *   error log file..
 */
function queue_runner_file_log_collectors_download($coll_id, $type = 'queue') {
  // Make sure type is an expected value to prevent file traversal.
  $type = ($type == 'error') ? 'error' : 'queue';

  $timestamp = db_query('SELECT created from {queue_runner_collector} WHERE coll_id = :coll_id', array(':coll_id' => $coll_id))->fetchField();
  // Get the date this collector was created in YYYY-MM-DD format,
  $date = !empty($timestamp) ? gmdate('Y-m-d', $timestamp) : NULL;
  $date = queue_runner_file_log_date_get($date);
  // Generate the filepath to the collector's log file.
  $path = variable_get('queue_runner_log_path', file_directory_temp() . '/queue_runner');
  $filepath = "$path/collectors/$coll_id/$type.log";

  if (file_exists($filepath)) {
    $name = "{$date}_collector_{$coll_id}_{$type}.log";
    header('Content-Type: text/plain');
    header('Content-Disposition: attachment; filename='.$name);
    header('Expires: 0');
    header('Cache-Control: must-revalidate');
    header('Pragma: public');
    header('Content-Length: ' . filesize($filepath));
    ob_clean();
    flush();
    readfile($filepath);
    drupal_exit();
  }
  else {
    return MENU_NOT_FOUND;
  }
}

