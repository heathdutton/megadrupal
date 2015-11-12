<?php

/**
 * @file
 * Administration interface of
 * @see cron_control.module
 *
 * @author
 *   Markus Kalkbrenner | bio.logis GmbH
 */

/**
 * Returns an ordered list of all cron jobs and their attributes
 *
 * @param $order
 *   Order of cron jobs returned. Possible values: 'module', 'server_addr'
 *
 * @return
 *  array of cron jobs
 */
function cron_control_get_jobs($order = 'module') {
  $order_detail = array(
    'module' => array('module', 'server_addr'),
    'server_addr' => array('server_addr', 'module'),
  );
  $jobs = array();
  $query = db_select('cron_control_jobs', 'ccj')
    ->fields('ccj', array('module', 'server_addr', 'active'));
  foreach ($order_detail[$order] as $criterion) {
    $query->orderBy('ccj.' . $criterion, 'asc');
  }
  if ($result = $query->execute()) {
    while ($job = $result->fetchObject()) {
      $jobs[$job->$order][] = $job;
    }
    if (empty($jobs)) {
      $link = l(t('click here to run cron on this server (@server_addr)', array('@server_addr' => $_SERVER['SERVER_ADDR'])),
        'admin/reports/status/run-cron', array('query' => drupal_get_destination()));
      drupal_set_message(t("Cron Control isn't aware of any cron jobs yet. You need to run Drupal's cron on every server. If you're only running one webserver or if you're using sticky sessions, you can !action.",
        array('!action' => filter_xss($link))), 'error');
    }
  }
  return $jobs;
}

/**
 * Form builder for cron job configuration.
 *
 * @ingroup forms
 */
function cron_control_jobs_form($form, $form_state, $order) {
  $form = array();
  $active_jobs = array();
  $all_jobs = array();
  $jobs = cron_control_get_jobs($order);

  $form['cron_control_temp_disable_global'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable all cron jobs temporarily'),
    '#default_value' => variable_get('cron_control_temp_disable_global', FALSE),
    '#description' => t('Disable all cron jobs on every server temporarily.'),
  );

  foreach (array_keys($jobs) as $key) {
    $form['cron_control_jobs_' . $key] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($key),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $title = '';
    switch ($order) {
      case 'module':
        $form['cron_control_jobs_' . $key]['#description'] = t('Configure hosts to execute cron job of !module module.', array('!module' => $key));
        $title = 'server_addr';
        break;

      case 'server_addr':
        $form['cron_control_jobs_' . $key]['#description'] = t('Enable cron jobs on host !server_addr.', array('!server_addr' => $key));
        $title = 'module';
        break;
    }

    foreach ($jobs[$key] as $job) {
      $form['cron_control_jobs_' . $key]['cron_control_' . $job->module . '/' . str_replace('.', '_', $job->server_addr)] = array(
        '#type' => 'checkbox',
        '#title' => check_plain($job->$title),
        '#default_value' => $job->active,
        '#description' => t('Enable cron job of %module module on host %server_addr.', array('%module' => $job->module, '%server_addr' => $job->server_addr)),
      );

      $all_jobs[] = $job->module;

      if ($job->active) {
        $active_jobs[] = $job->module;
      }
    }
  }

  $form['cron_control_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $active_jobs = array_unique($active_jobs);
  $all_jobs = array_unique($all_jobs);
  $inactive_jobs = array_diff($all_jobs, $active_jobs);
  if (!empty($inactive_jobs)) {
    drupal_set_message(t("These jobs aren't enabled on any server: %job_list", array('%job_list' => implode(', ', $inactive_jobs))), 'warning');
  }

  return $form;
}


/**
 * Form submission handler for cron_control_jobs_form().
 *
 * @see cron_control_jobs_form()
 */
function cron_control_jobs_form_submit($form, &$form_state) {
  variable_set('cron_control_temp_disable_global', $form_state['values']['cron_control_temp_disable_global']);
  unset($form_state['values']['cron_control_temp_disable_global']);

  foreach ($form_state['values'] as $key => $value) {
    if (strpos($key, 'cron_control_') === 0 && strpos($key, '/') !== FALSE) {
      list($module, $server_addr) = explode('/', str_replace('cron_control_', '', $key));
      $job = array(
        'module' => $module,
        'server_addr' => str_replace('_', '.', $server_addr),
        'active' => $value,
      );
      drupal_write_record('cron_control_jobs', $job, array('module', 'server_addr'));
    }
  }
}
