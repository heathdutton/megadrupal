<?php

/**
 * @file
 * Additional form code for Hotfolder.
 */


/**
 * Button for manually running watches.
 */
function _hotfolder_run_watches($form = array(), &$form_state = array()) {
  if (user_access('run hotfolder watch')) {
    $form['hotfolder_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Run Watches'),
      '#submit' => array('_hotfolder_run_watches_submit'),
    );
    $query = 'SELECT COUNT(n.nid) AS watches FROM {node} n WHERE n.type = \'watch_configuration\' AND status = 1';
    $result = db_query($query)->fetch();
    $watches = $result->watches;
    $form['watches_current'] = array(
      '#type' => 'item',
      '#markup' => t('There are currently %count watches.', array('%count' => $watches)),
    );
  }
  return $form;
}

/**
 * Submit for manually running watches.
 */
function _hotfolder_run_watches_submit(&$form, &$form_values) {
  if (user_access('run hotfolder watch')) {
    _hotfolder_check_watches();
    drupal_set_message(t('Hotfolder watches have been run.'));
  }
}

/**
 * Button for manually running watches.
 */
function _hotfolder_run_jobs($form = array(), &$form_state = array()) {
  if (user_access('run hotfolder job')) {
    $form['hotfolder_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Run Jobs'),
      '#submit' => array('_hotfolder_run_jobs_submit'),
    );
    $jobs = _hotfolder_count_jobs();
    $form['jobs_current'] = array(
      '#type' => 'item',
      '#markup' => t('There are currently %count unclaimed jobs.', array('%count' => $jobs)),
    );
  }
  return $form;
}

/**
 * Submit for manually running watches.
 */
function _hotfolder_run_jobs_submit(&$form, &$form_values) {
  if (user_access('run hotfolder job')) {
    // Get the queues.
    $queues = module_invoke_all('cron_queue_info');
    drupal_alter('cron_queue_info', $queues);
    $info = !empty($queues['hotfolder_queue']) ? $queues['hotfolder_queue'] : array();
    $processed = 0;
    if (!empty($info) && empty($info['skip on cron'])) {
      drupal_set_message(t('Found job queue'));
      $function = $info['worker callback'];
      $end = time() + (isset($info['time']) ? $info['time'] : 10);
      $queue = DrupalQueue::get('hotfolder_queue');
      // Loop for each job currently in the queue. Only once each.
      $items = array();
      while (time() < $end && ($item = $queue->claimItem())) {
        $items[] = $item;
      }
      // Loop once and once only.
      foreach ($items as $item) {
        drupal_set_message(t('Running hotfolder job #%nid.', array('%nid' => $item->data->nid)));
        try {
          ++$processed;
          $queue->deleteItem($item);
          _hotfolder_process_job($item->data);
        }
        catch (Exception $e) {
          // In case of exception log it and leave the item in the queue
          // to be processed again later.
          watchdog_exception('hotfolder', $e);
        }
        // Return the memory.
        unset($item);
      }
    }
    drupal_set_message(t('Hotfolder jobs have been run.'));
    drupal_set_message(t('Attempted to process %count jobs.', array('%count' => $processed)));
  }
}

/**
 * Button for manually running watches.
 */
function _hotfolder_toggle_cron($form = array(), &$form_state = array()) {
  if (user_access('toggle hotfolder cron')) {
    if (variable_get('hotfolder_enabled', TRUE)) {
      $form['hotfolder_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Disable Watches'),
        '#submit' => array('_hotfolder_toggle_cron_submit'),
      );
      $form['threading_current'] = array(
        '#type' => 'item',
        '#markup' => t('Hotfolder cron is currently enabled. Hotfolders will run during cron runs.'),
      );
    }
    else {
      drupal_set_message('Automatic hotfolders processing is currently disabled. Watches and jobs will NOT be run on cron.', 'error');
      $form['hotfolder_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Enable Watches'),
        '#submit' => array('_hotfolder_toggle_cron_submit'),
      );
      $form['threading_current'] = array(
        '#type' => 'item',
        '#markup' => t('Hotfolder cron is NOT currently enabled. Hotfolders will run NOT during cron runs.'),
      );
    }
  }
  return $form;
}

/**
 * Submit for manually running watches.
 */
function _hotfolder_toggle_cron_submit(&$form, &$form_values) {
  if (user_access('toggle hotfolder cron')) {
    if (variable_get('hotfolder_enabled', TRUE)) {
      variable_set('hotfolder_enabled', FALSE);
      drupal_set_message(t('Automatic hotfolders processing has been disabled.'), 'error');
    }
    else {
      variable_set('hotfolder_enabled', TRUE);
      drupal_set_message(t('Automatic hotfolders processing has been enabled.'));
    }
  }
}

/**
 * Button for manually running watches.
 */
function _hotfolder_reset_threads($form = array(), &$form_state = array()) {
  if (user_access('reset hotfolder threads')) {
    $form['hotfolder_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Reset Threads'),
      '#submit' => array('_hotfolder_reset_threads_submit'),
    );
    $form['threading_current'] = array(
      '#type' => 'item',
      '#markup' => t('There are currently %count threads.', array('%count' => variable_get('hotfolder_threading_current', 0))),
    );
  }
  return $form;
}

/**
 * Submit for manually running watches.
 */
function _hotfolder_reset_threads_submit(&$form, &$form_values) {
  if (user_access('reset hotfolder threads')) {
    variable_set('hotfolder_threading_current', 0);
    drupal_set_message(t('Current threads being tracked has been set to zero.'));
  }
}

/**
 * Button for manually running watches.
 */
function _hotfolder_run_cron($form = array(), &$form_state = array()) {
  if (user_access('run hotfolder cron')) {
    $form['hotfolder_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Run Cron'),
      '#submit' => array('_hotfolder_run_cron_submit'),
    );
    $form['threading_current'] = array(
      '#type' => 'item',
      '#markup' => t('Run cron. This will pick up new watches and process jobs.'),
    );
  }
  return $form;
}

/**
 * Submit for manually running watches.
 */
function _hotfolder_run_cron_submit(&$form, &$form_values) {
  if (user_access('run hotfolder cron')) {
    drupal_cron_run();
    drupal_set_message(t('Drupal cron has been run manually.'));
  }
}