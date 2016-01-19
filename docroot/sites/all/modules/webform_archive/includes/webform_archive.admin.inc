<?php
/**
 * @file
 * Provides the webform archive interface for admins for archiving webforms.
 */

/**
 * Page callback to add a new view.
 *
 * @param string $webform_archive_type
 *   Expected values are 'active' & 'closed' for page title.
 */
function webform_archive_ui_page($webform_archive_type = 'active') {
  $title = t('Archive @archive_type webforms', array('@archive_type' => $webform_archive_type));
  // Return webform_archive_form.
  drupal_set_title($title);
  return drupal_get_form('webform_archive_form', $webform_archive_type);
}

/**
 * Form builder for the Archive webforms.
 */
function webform_archive_form($form, &$form_state, $webform_archive_type) {
  $form = array();

  // Get all the webforms according to status.
  if ($webform_archive_type == 'closed') {
    $webforms = webform_archive_get_all_webforms(0);
  }
  else {
    $webforms = webform_archive_get_all_webforms(1);
  }

  if (!empty($webforms)) {
    $form_state['storage']['webforms'] = $webforms;
    $form['webforms_archive_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Archive all webforms'),
      '#description' => t('Select this checkbox to archive all webforms'),
    );
    $form['weform_archive_nid'] = array(
      '#type' => 'select',
      '#title' => t('Choose Webform(s)'),
      '#description' => t('Choose webform(s) for which submissions are to be archived.'),
      '#options' => $webforms,
      '#multiple' => TRUE,
      '#states' => array(
        'invisible' => array(
          ':input["webforms_archive_all"]' => array('value' => 1),
        ),
      ),
    );
    if ($webform_archive_type != 'closed') {
      $form['weform_archive_time'] = array(
        '#type' => module_exists('date_popup') ? 'date_popup' : 'date',
        '#title' => t('Archive Date'),
        '#date_format' => 'Y-m-d',
        '#description' => t('Enter Date for archiving all submission before this date (not including the mentioned date)'),
        '#required' => TRUE,
      );
    }
    $form['webform_archive_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Archive'),
    );
  }
  else {
    $message = t('There are no @type webforms.', array('@type' => $webform_archive_type));
    drupal_set_message($message);
  }
  return $form;
}

/**
 * Validation Callback for archive form.
 */
function webform_archive_form_validate($form, $form_state) {
  if ($form_state['values']['webforms_archive_all'] == 0) {
    if (empty($form_state['values']['weform_archive_nid'])) {
      form_set_error('weform_archive_nid', t('Please Select a Webform for archive'));
    }
  }
}

/**
 * Submit callback for archive form.
 */
function webform_archive_form_submit($form, $form_state) {
  $archive_date_timestamp = 0;
  if ($form_state['values']['webforms_archive_all'] == 1) {
    $webforms = $form_state['storage']['webforms'];
  }
  else {
    $webforms = $form_state['values']['weform_archive_nid'];
  }
  if (isset($form_state['values']['weform_archive_time'])) {
    $archive_date = $form_state['values']['weform_archive_time'];
    if (!module_exists('date_popup')) {
      $archive_date = $archive_date['year'] . '-' . $archive_date['month'] . '-' . $archive_date['day'];
    }
    $archive_date_timestamp = strtotime($archive_date);
  }
  // Extracting the nid(s) of the webforms.
  $webforms_nid = array_keys($webforms);
  $operations = array(
    array('webform_archive_batch_process_start',
      array($webforms_nid, $archive_date_timestamp)),
  );
  $batch = array(
    'title' => t('Archive Webform Submissions'),
    'operations' => $operations,
    'file' => drupal_get_path('module', 'webform_archive') . '/includes/webform_archive.batch.inc',
    'finished' => 'webform_archive_batch_finished',
    'init_message' => t('Archive Batch is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Archive Batch has encountered an error.'),
  );
  batch_set($batch);
}

/**
 * Settings for the batch process, define the number of submissions for batch.
 *
 * @return array
 *   $form.
 */
function webform_archive_admin_settings() {
  $form = array();
  $form['webform_archive_batch_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Batch Limit'),
    '#description' => t('No. of webform submissions for every batch process. The default of 10 is kept low keeping in mind that PHP should not time out. If you have more memory and CPU, you could choose a larger number to save some time while archiving all the data.'),
    '#default_value' => variable_get('webform_archive_batch_limit', WEBFORM_ARCHIVE_BATCH_LIMIT),
    '#element_validate' => array('element_validate_integer_positive'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
