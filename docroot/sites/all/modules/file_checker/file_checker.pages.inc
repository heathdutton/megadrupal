<?php

/**
 * @file file_checker.pages.inc
 * Lists and configuration pages for the file checker.
 */

/**
 * Configuration options form of the file checker.
 * @return
 *   form array
 */
function file_checker_admin_settings($form, $form_state) {

  $form['file_checker_process_on_cron'] = array(
    '#type' => 'radios',
    '#title' => t('Process on cron'),
    '#default_value' => variable_get('file_checker_process_on_cron', 0),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#description' => t('Do automatic file checking on each cron run. Enable this option only on small sites with a few files.<br/>You can process the file checking manually below or run the drush command "drush file-checker-run"'),
  );

  $form['file_checker_process_batch_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Process batch size'),
    '#default_value' => variable_get('file_checker_process_batch_size', 10000),
    '#description' => t('Process file checking in junks instead of one by one preventing timeouts, memory outages etc.<br/>A higher values usually results in better performance whereas a lower value increases percentage bar accuracy.'),
  );

  $form['file_checker_process_manually'] = array(
    '#type' => 'item',
    '#title' => t('Process manually'),
  );

  $total = db_query('SELECT count(*) FROM {file_usage}')->fetchField();
  $total_files = t('There are currently @total files.', array('@total' => $total));
  if (!variable_get('file_checker_last_run', 0)) {
    $form['file_checker_process_manually']['#value'] = $total_files . ' ' . t('File verification has never been run.');
  }
  else {
    $form['file_checker_process_manually']['#value'] = $total_files . ' ' . t('File verification has been last run on @time', array('@time' => format_date(variable_get('file_checker_last_run', 0), 'short')));
  }

  $form['file_checker_process_manually_run'] = array(
    '#type' => 'submit',
    '#value' => t('Flag missing files'),
    '#suffix' => '<br/><br/>',
  );

  $form['#submit'][] = 'file_checker_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Submit handler for admin settings form.
 * @param $form
 *   form array
 * @param $form_state
 *   form values
 */
function file_checker_admin_settings_submit($form, $form_state) {
  // Run file checking manually
  if ($form_state['clicked_button']['#id'] == 'edit-file-checker-process-manually-run') {
    file_checker_process_verification(FALSE); // batch_process is triggered automatically
  }
}

/**
 * File overview page list all files including some filters and sorting.
 * See dblog.admin.inc dblog_overview() for an example.
 */
function file_checker_overview() {
  $rows = array();
  
  // filter form
  $build['file_checker_filter_form'] = drupal_get_form('file_checker_filter_form');

  // define table headers here
  $header = array(
    array(
      'data' => t('File ID'),
      'field' => 'f.fid',
    ),
    array(
      'data' => t('File name'),
      'field' => 'f.filename',
    ),
    array(
      'data' => t('File path'),
      'field' => 'f.uri',
    ),
    array(
      'data' => t('File Mime'),
      'field' => 'f.filemime',
    ),
    array(
      'data' => t('File size'),
      'field' => 'f.filesize',
    ),
    array(
      'data' => t('Timestamp'),
      'field' => 'f.timestamp',
      'sort' => 'desc',
    ),
    array(
      'data' => t('User'),
      'field' => 'f.uid',
    ),
    array(
      'data' => t('Status'),
      'field' => 'f.status',
    ),
    array(
      'data' => t('Type'),
      'field' => 'u.type',
    ),
    array(
      'data' => t('Used ID'),
      'field' => 'u.id',
    ),
  );
  $select = db_select('file_managed', 'f')
    ->extend('TableSort')
    ->extend('PagerDefault');
  $select->join('file_usage', 'u', 'f.fid = u.fid');
  $select->addField('u', 'type', 'used_type');
  $select->addField('u', 'id', 'used_type_id');
  $select->fields('f');
  $results = $select;

  // apply filters
  $filters = $build['file_checker_filter_form']['filters'];
  if (!empty($filters['filter_filename']['#value'])) {
    $results = $results
      ->condition('f.status', $filters['filter_filename']['#value'], 'LIKE');
  }
  if (!empty($filters['filter_filemime']['#value'])) {
    $results = $results
      ->condition('f.filemime', $filters['filter_filemime']['#value']);
  }
  if (isset($_SESSION['file_checker_overview_filter_status'])) {
    $results = $results
      ->condition('f.status', $_SESSION['file_checker_overview_filter_status'], '=');
  }
  if (!empty($filters['filter_uid']['#value'])) {
    $results = $results
      ->condition('f.uid', $filters['filter_uid']['#value'], 'IN');
  }

  $results = $results
    ->limit(100)
    ->orderByHeader($header)
    ->execute();
  foreach ($results as $file) {
    $rows[] = array(
      'data' => array(
        $file->fid,
        $file->filename,
        $file->uri,
        $file->filemime,
        $file->filesize,
        format_date($file->timestamp, 'short'),
        l(user_load($file->uid)->name, 'user/' . $file->uid),
        $build['file_checker_filter_form']['filters']['filter_status']['#options'][$file->status],
        $file->used_type,
        $file->used_type_id,
      ),
    );
  }

  $build['file_checker_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No files available.'),
  );
  $build['file_checker_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Filter form for overview page.
 *
 * @param $form_state
 *   array of form values
 * @return
 *   form array
 */
function file_checker_filter_form($form, $form_state) {

  $filter_filename = 
    isset($_SESSION['file_checker_overview_filter_filename']) ?
    $_SESSION['file_checker_overview_filter_filename'] : '';
  $filter_filemime =
    isset($_SESSION['file_checker_overview_filter_filemime']) ?
    $_SESSION['file_checker_overview_filter_filemime'] : '';
  $filter_uid =
    isset($_SESSION['file_checker_overview_filter_uid']) ?
    $_SESSION['file_checker_overview_filter_uid'] : NULL;
  $filter_status =
    isset($_SESSION['file_checker_overview_filter_status']) ?
    $_SESSION['file_checker_overview_filter_status'] : '';
    
  $expand = !empty($filter_filename) || !empty($filter_filemime) || !empty($filter_uid) || $filter_status != 2;
    
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#collapsible' => TRUE,
    '#collapsed' => !$expand,
  );

  $form['filters']['filter_filename'] = array(
    '#type' => 'textfield',
    '#title' => t('File name'),
    '#default_value' => $filter_filename,
  );

  $names[''] = t('All mime types');
  foreach (_file_checker_get_file_mime_types() as $type) {
    $names[$type] = t('!type', array('!type' => t($type)));
  }
  $form['filters']['filter_filemime'] = array(
    '#type' => 'select',
    '#title' => t('File Mime type'),
    '#options' => $names,
    '#default_value' => $filter_filemime,
  );
  $form['filters']['filter_uid'] = array(
    '#type' => 'textfield',
    '#title' => t('User Id'),
    '#default_value' => $filter_uid,
  );
  $form['filters']['filter_status'] = array(
    '#type' => 'select',
    '#title' => t('File status'),
    '#options' => array(
      '' => t('Any'),
      0 => t('Temporary'),
      1 => t('Permanent'),
      2 => t('Missing'),
    ),
    '#default_value' => $filter_status,
  );

  $form['filters']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['actions'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  $form['filters']['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
  );

  return $form;
}

/**
 * Submit handler for filter form.
 * @param $form
 *   form array
 * @param $form_state
 *   form values
 */
function file_checker_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  switch ($op) {
    case t('Reset'): // reset form
      foreach ($form_state['values'] as $form_value_id => $form_value) {
        if (strpos($form_value_id, 'filter_') === 0) {
          unset($_SESSION['file_checker_overview_$form_value_id']);
        }
      }
      break;
    case t('Filter'): // filter form
      foreach ($form_state['values'] as $form_value_id => $form_value) {
        if (strpos($form_value_id, 'filter_') === 0) {
          if (!empty($form_value) || $form_value === '0') {
            $_SESSION['file_checker_overview_'.$form_value_id] = $form_value;
          }
          else {
            unset($_SESSION['file_checker_overview_'.$form_value_id]);
          }
        }
      }
      break;
  }
  return 'admin/reports/file_checker';
}

/**
 * Get a list of file mime types.
 *
 * @return
 *   array of file mime types
 */
function _file_checker_get_file_mime_types() {
  $types = array();
  $result = db_query('SELECT DISTINCT(filemime) FROM {file_managed} ORDER BY filemime');
  foreach ($result as $object) {
    $types[] = $object->filemime;
  }
  return $types;
}