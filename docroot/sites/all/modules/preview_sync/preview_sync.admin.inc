<?php

/**
 * @file
 * Code for administrative menu callbacks.
 */

/**
 * Menu callback form for admin/reports/bulk-preview.
 */
function preview_sync_overview($form, &$form_state) {
  // Chuck toys when there is no Drush aliases file.
  if (!file_exists(PREVIEW_SYNC_DRUSH_FILE)) {
    drupal_set_message(t('Missing Drush alias file, please create one at %location, and view the <a href="!url">status report</a> page to ensure it is working as expected.', array(
      '%location' => PREVIEW_SYNC_DRUSH_FILE,
      '!url' => url('admin/reports/status'),
    )), 'error');
    return $form;
  }

  $form['overview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Overview'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Custom CSS and JavaScript for the page.
  $form['#attached']['css'][] = drupal_get_path('module', 'preview_sync') . '/css/preview_sync.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'preview_sync') . '/js/preview_sync.js';

  // Preview sync is idle.
  if (preview_sync_is_empty()) {

    list($from, $to) = preview_sync_get_aliases();
    list($from_alias) = array_keys($from);
    list($to_alias) = array_keys($to);

    // Get the status report for the Preview site, this calls PHP exec() as this
    // is the only way to execute Drush through Drupal. The web server is
    // required to have the Drush alias file in it's path in order to execute
    // this.
    exec("drush @$to_alias core-status", $output, $ret_val);

    // Before the DB is imported, the response is a filthy array, and the first
    // element contains a sudo error string.
    if (is_array($output) && isset($output[0]) && strpos($output[0], 'Base table or view not found') > 0) {
      $to_env = reset($to);
      $status['uri'] = $to_env['uri'];
      $status['bootstrap'] = t('Failed');
      $status['db-status'] = $output[0];
    }
    else {
      $status = drupal_json_decode(reset($output));
    }

    // Get a list of content in review.
    $content = preview_sync_content_in_review();

    // The DB dump file.
    $dump_file = $from[$from_alias]['command-specific']['sql-dump']['result-file'];

    $modified_date = t('never');
    if (file_exists($dump_file)) {
      $modified_date = format_date(filemtime($dump_file)); //  date('d/m/Y H:i', filemtime($dump_file));
    }

    $dump_file_info = t('!file (Last modified: !date)', array('!file' => $dump_file, '!date' => $modified_date));

    $form['overview']['info']['#markup'] = theme('preview_sync_idle', array(
      'preview_uri' => $status['uri'],
      'preview_bootstrap' => $status['bootstrap'],
      'preview_db_status' => $status['db-status'],
      'preview_row_class' => ($status['bootstrap'] === 'Successful' && $status['db-status'] === 'Connected') ? 'ok' : 'error',
      'dump_file' => $dump_file_info,
      'dump_file_exists' => file_exists($dump_file),
      'dump_file_writable' => is_writable($dump_file),
      'dump_file_row_class' => (file_exists($dump_file) && is_writable($dump_file) || !file_exists($dump_file)) ? 'ok' : 'error',
      'last_run' => variable_get('preview_sync_last_run', 0),
      'last_run_user' => user_load(variable_get('preview_sync_last_run_uid', 0)),
      'review_total' => $content['total'],
      'review_nodes' => $content['nodes'],
    ));

    // Custom submit button for rebuilding the party and candidate tables.
    $form['overview']['preview_sync_start'] = array(
      '#type' => 'submit',
      '#value' => t('Synchronise Preview now'),
      '#submit' => array('preview_sync_start_submit'),
    );
  }

  // Preview Sync is active.
  else {
    $form['overview']['info']['#markup'] = theme('preview_sync_progress', array(
      'tasks' => preview_sync_get_latest_batch(),
      'last_run' => variable_get('preview_sync_last_run', 0),
      'last_run_user' => user_load(variable_get('preview_sync_last_run_uid', 0)),
    ));

    // Custom submit button for rebuilding the party and candidate tables.
    $form['overview']['preview_sync_stop'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel remaining tasks'),
      '#submit' => array('preview_sync_stop_submit'),
    );
  }

  // History panel down below.
  $form['history'] = array(
    '#type' => 'fieldset',
    '#title' => t('History'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $view = views_get_view('preview_sync_history');
  $arguments = array();
  $form['history']['table'] = array(
    '#markup' => $view->preview('master', $arguments),
  );

  // Custom submit button for rebuilding the party and candidate tables.
  if (user_access('clear logs preview_sync')) {
    $form['history']['preview_sync_clear_logs'] = array(
      '#type' => 'submit',
      '#value' => t('Clear logs'),
      '#submit' => array('preview_sync_clear_logs_submit'),
    );
  }

  return $form;
}

/**
 * Redirect to the confirm form.
 */
function preview_sync_start_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/reports/bulk-preview/start';
}

/**
 * Confirm form.
 */
function preview_sync_start_confirm($form, &$form_state) {
  // Choose whether or not to include Solr in the steps.
  $form['solr'] = array(
    '#type' => 'radios',
    '#title' => t('Drop and re-index preview Solr'),
    '#description' => t('This will add several minutes to the duration, but will be required if you are looking to get an accurate picture of how the newly published content looks like in the search results (e.g. site search, Solr powered views).'),
    '#options' => array(
      0 => t('Do not re-index (faster), I am happy with the search not being perfect'),
      1 => t('Re-index (slower), I need search to be perfect'),
    ),
    '#default_value' => 0,
  );

  return confirm_form($form, t('Are you sure you want to start a Preview Sync?'),
    'admin/reports/bulk-preview', t('Once the preview process has started, you can opt to cancel it at any time.'),
    t('Synchronise Preview now'), t('Cancel')
  );
}

/**
 * Add the Preview Sync items on to the queue. This should be added in such a
 * fashion that the items gets pulled off in the correct order. Ordinary the
 * queue system is FIFO, so the first item you add is the first item you
 * process.
 */
function preview_sync_start_confirm_submit($form, &$form_state) {
  global $user;

  $solr_reindex = (bool) $form_state['values']['solr'];

  $queue = DrupalQueue::get(PREVIEW_SYNC_QUEUE);
  $queue->createQueue();

  // Work out the next available batch id.
  $next_pid = preview_sync_max_pid() + 1;

  list($from, $to) = preview_sync_get_aliases();
  list($from_alias) = array_keys($from);
  list($to_alias) = array_keys($to);
  $from_alias = 'preview.' . $from_alias;
  $to_alias = 'preview.' . $to_alias;

  $tasks = preview_sync_tasks();
  foreach ($tasks as $key => $task) {
    // Optionally skip solr tasks if the user elects to do this.
    if (!$solr_reindex && strpos($key, 'solr_') === 0) {
      continue;
    }

    $sucessful_queue = $queue->createItem(array(
      'pid' => $next_pid,
      'uid' => $user->uid,
      'title' => $task['title'],
      'type' => $key,
      'command' => $task['command'],
      'arguments' => isset($task['arguments']) ? $task['arguments'] : array(),
      'options' => isset($task['options']) ? $task['options'] : array(),
      'site' => $task['site'],
      // No leading '@' here.
      'from' => $from_alias,
      'to' => $to_alias,
    ));
    if (!$sucessful_queue) {
      throw new Exception('Error queueing the Preview Sync tasks.');
    }
  }

  // Log a timestamp.
  variable_set('preview_sync_last_run', REQUEST_TIME);
  variable_set('preview_sync_last_run_uid', $user->uid);

  // Tell the user.
  drupal_set_message(format_plural($queue->numberOfItems(), '1 task was added to the Preview Sync queue', '@count tasks were added to the Preview Sync queue'), 'status');

  // Send the user back to the overview page.
  $form_state['redirect'] = 'admin/reports/bulk-preview';
}

/**
 * Removes the remaining queued items from the Preview Sync. This will not stop
 * existing in progress items, and could leave the preview environment in a
 * broken state.
 */
function preview_sync_stop_submit($form, &$form_state) {
  $queue = DrupalQueue::get(PREVIEW_SYNC_QUEUE);
  $queue->createQueue();
  $queue->deleteQueue();
}

/**
 * Redirect to the confirm form.
 */
function preview_sync_clear_logs_submit($form, &$form_state) {
  db_truncate('preview_sync_log')->execute();
  drupal_set_message(t('Cleared logs'));
}
