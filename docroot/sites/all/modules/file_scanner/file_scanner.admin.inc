<?php

/**
 * @file
 * File Scanner admin functions.
 *
 * This file contains admin interface functions for File Scanner.
 * It contains ca menu callback functions and theme functions.
 */

/**
 * Overview of created analyses.
 * The form allows to run selected analyses.
 */
function file_scanner_admin_overview_form($form, &$form_state) {
  $form = array();
  $form['#tree'] = TRUE;

  // Get all analyses
  $form['analyses'] = array();
  foreach (file_scanner_get_analyses() as $analysis) {
    $form['analyses'][$analysis->aid]['#item'] = $analysis;
    $form['analyses'][$analysis->aid]['analysis-run-' . $analysis->aid] = array(
      '#type' => 'checkbox',
    );
  }
  if (!empty($form)) {
    $form['actions'] = array(
      '#type' => 'actions',
      '#attributes' => array(),
    );
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Run selected analyses now'),
    );
  }
  else {
    $form['#empty_text'] = t('There are no analyses yet. <a href="@link">Create a new analysis</a>.', array('@link' => url('admin/config/development/file_scanner/add')));
  }
  return $form;
}

/**
 * Form submission handler for file_scanner_admin_overview_form().
 */
function file_scanner_admin_overview_form_submit($form, &$form_state) {
  $analyses_to_run = array();
  // Check if there are analyses to run
  if (!empty($form_state['values']['analyses'])) {
    foreach ($form_state['values']['analyses'] as $aid => $analysis_checkbox) {
      if ($analysis_checkbox['analysis-run-' . $aid] == 1) {
        $analyses_to_run[] = $aid;
      }
    }
  }
  if (!empty($analyses_to_run)) {
    batch_set(file_scanner_analysis_build_batch($analyses_to_run));
  }
  else {
    drupal_set_message(t('No analysis selected.'), 'warning');
  }
}

/**
 * Form constructor for the analysis form.
 *
 * @param object $analysis Analysis object
 * @param string $action String defining the action.
 *    Supported values : add|edit|clone.
 *    Actually only the clone value is used.
 *
 * @see file_scanner_admin_analysis_form_validate()
 * @see file_scanner_admin_analysis_form_submit()
 * @ingroup forms
 */
function file_scanner_admin_analysis_form($form, &$form_state, $analysis = NULL, $action = '') {
  $form = array();

  // If no analysis is provided, initailise analysis object
  if (empty($analysis)) {
    $analysis = new stdClass();
    $analysis->label = '';
    $analysis->description = '';
    $analysis->folders = array();
    $analysis->extensions = array();
    $analysis->selector = '';
    // Set page title
    drupal_set_title(t('Create a new analysis'));
  }
  else {
    // Set page title
    drupal_set_title(t('Edit %label', array('%label' => check_plain($analysis->label))), PASS_THROUGH);
    // If cloning an anlysis, reset some values
    if ($action == 'clone') {
      $analysis->aid = NULL;
      $analysis->label = t('Clone of') . ' ' . $analysis->label;
      $analysis->created = NULL;
      $analysis->changed = NULL;
      $analysis->last_run = NULL;
      $analysis->last_run_files_parsed = NULL;
    }
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $analysis->label,
    '#description' => t('Label of the analysis.'),
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('Please describe the aim of the analysis.'),
    '#default_value' => $analysis->description,
    '#maxlength' => 1024,
  );
  $form['selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Selector'),
    '#description' => t('CSS3 selector.'),
    '#default_value' => $analysis->selector,
    '#maxlength' => 1024,
  );
  $form['folders'] = array(
    '#type' => 'textarea',
    '#title' => t('Folders'),
    '#description' => t('Folders containing the files to parse. Relative to %root. One folder per line.', array('%root' => _file_scanner_get_root_folder())),
    '#default_value' => implode(PHP_EOL, $analysis->folders),
  );
  $form['extensions'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed extensions'),
    '#description' => t("Filter files to aforementioned file extensions. One per line. Leave blank if you don't want to filter files."),
    '#default_value' => implode(PHP_EOL, $analysis->extensions),
    '#maxlength' => 1024,
  );

  $form['analysis'] = array(
    '#type' => 'value',
    '#value' => $analysis,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['submit-and-run'] = array(
    '#type' => 'submit',
    '#value' => t('Save and run now'),
  );
  return $form;
}

/**
 * Form validation handler for file_scanner_admin_analysis_form().
 *
 * @see file_scanner_admin_analysis_form_submit()
 */
function file_scanner_admin_analysis_form_validate($form, &$form_state) {
  $folders = array_map('trim', explode(PHP_EOL, $form_state['values']['folders']));
  foreach ($folders as $folder) {
    $folder_name = _file_scanner_get_root_folder() . $folder;
    // Check if folder exists
    if (!is_dir($folder_name)) {
      form_set_error('folder', t("Folder %folder doesn't exist", array('%folder' => $folder_name)));
    }
    else {
      // Check if folder is readable
      if (!is_readable($folder_name)) {
        form_set_error('folder', t("Folder %folder is not readable", array('%folder' => $folder_name)));
      }
    }
  }
}

/**
 * Form submission handler for file_scanner_admin_analysis_form().
 *
 * @see file_scanner_admin_analysis_form_validate()
 */
function file_scanner_admin_analysis_form_submit($form, &$form_state) {
  $analysis = $form_state['values']['analysis'];

  // If this is a new analysis, set default values
  if (empty($analysis->aid)) {
    $analysis->created = REQUEST_TIME;
    $analysis->last_run = NULL;
    $analysis->last_run_files_parsed = NULL;
  }
  // If this is an existing analysis
  else {
    // If scope has changed (selector or folder), reset result values
    $folders = implode(PHP_EOL, $analysis->folders);
    if ($analysis->selector != $form_state['values']['selector'] || $folders != $form_state['values']['folders']) {
      _file_scanner_clean_analysis_results($analysis->aid);
      $analysis->last_run = NULL;
      $analysis->last_run_files_parsed = NULL;
    }
  }

  $analysis->label = $form_state['values']['label'];
  $analysis->description = $form_state['values']['description'];
  $analysis->selector = $form_state['values']['selector'];
  $analysis->folders = array_map('trim', explode(PHP_EOL, $form_state['values']['folders']));
  $analysis->extensions = array_map('trim', explode(PHP_EOL, $form_state['values']['extensions']));
  $analysis->changed = REQUEST_TIME;

  $result = file_scanner_analysis_save($analysis);
  switch ($result) {
    case SAVED_NEW :
      drupal_set_message(t('Analysis successfully created.'));
      break;
    case SAVED_UPDATED :
      drupal_set_message(t('Analysis successfully updated.'));
      break;
    default :
      drupal_set_message(t('An error occured. Please check the logs.'), 'error');
      break;
  }

  // Check if analysis has to be run now
  if ($form_state['clicked_button']['#id'] == 'edit-submit-and-run') {
    batch_set(file_scanner_analysis_build_batch(array($analysis->aid)));
  }

  // Redirect user to analyses overview page
  $form_state['redirect'][] = 'admin/config/development/file_scanner';
}

/**
 * Form constructor for the analysis deletion confirmation form.
 *
 * @see file_scanner_analysis_delete_confirm_submit()
 */
function file_scanner_analysis_delete_confirm($form, &$form_state, $analysis) {
  $form['#analysis'] = $analysis;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['aid'] = array('#type' => 'value', '#value' => $analysis->aid);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $analysis->label)),
    'admin/config/development/file_scanner',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Executes analysis deletion.
 *
 * @see file_scanner_analysis_delete_confirm()
 */
function file_scanner_analysis_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $analysis = file_scanner_analysis_load($form_state['values']['aid']);
    file_scanner_analysis_delete($form_state['values']['aid']);
    watchdog('file_scanner', 'Deleted analysis %title.', array('%title' => $analysis->label));
    drupal_set_message(t('Analysis %title has been deleted.', array('%title' => $analysis->label)));
  }

  // Redirect user to analyses overview page
  $form_state['redirect'] = 'admin/config/development/file_scanner';
}

/**
 * Analysis view page
 */
function file_scanner_analysis_view($analysis) {
  drupal_add_css(drupal_get_path('module', 'file_scanner') . '/css/file_scanner.admin.css');
  $output = '';
  $results = file_scanner_get_analysis_results($analysis->aid);
  $analysis_stats = file_scanner_get_analysis_statistics($analysis);

  // Analysis summary
  $output .= theme('file_scanner_analysis_summary', array(
    'analysis' => $analysis,
    'analysis_results' => $results,
    'analysis_stats' => $analysis_stats,
  ));

  // Analysis results display
  $output .= theme('file_scanner_analysis_results_display', array(
    'analysis' => $analysis,
    'analysis_results' => $results,
  ));

  return $output;
}
