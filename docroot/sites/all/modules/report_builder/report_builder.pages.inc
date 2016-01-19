<?php

/**
 * @file
 * View and edit pages and associated functions.
 */

/**
 * Page callback for viewing the report.
 *
 * @param object $report
 *   The report to be viewed.
 */
function report_builder_view_report($report) {
  if (empty($report->repid)) {
    drupal_not_found();
  }
  else {

    $report = report_builder_update_results($report);

    $table = report_builder_get_table($report);

    $chart = report_builder_get_chart($report);

    $page = array(
      'table' => $table,
      'chart' => $chart,
    );

    return $page;
  }
}

/**
 * Edit page callback for the report.
 *
 * @param object $report
 *   The report being editted.
 */
function report_builder_edit_report($report) {

  ctools_include('object-cache');
  ctools_include('modal');
  ctools_include('ajax');
  ctools_modal_add_js();

  $cached_report = ctools_object_cache_get('report_builder', $report->repid);
  if (!empty($cached_report)) {
    $report = $cached_report;
  }

  $report = report_builder_update_results($report);

  $table = report_builder_get_table($report);

  $page['table'] = _report_builder_table_add_edit_row($table, $report->repid);
  $page['report_title'] = array(
    '#markup' => '<h2 id="report-builder-title">' . check_plain($report->title) . "</h2>",
    '#weight' => -200,
  );

  $page['table']['#weight'] = -100;

  $page['configure'] = array(
    'configure_link' => array(
      '#markup' => ctools_modal_text_button(t('Configure Report'), 'report-builder/build/nojs/' . $report->repid, t('Configure Report')),
    ),
  );
  $page['save_button'] = drupal_get_form('report_builder_edit_save_button_form', $report->repid);
  if (isset($report->data['updated'])) {
    drupal_set_message(t('There are unsaved changes. Click "Save Changes" below to permenantly save your report'), 'warning');
  }
  return $page;
}

/**
 * Save button for the report edit page.
 */
function report_builder_edit_save_button_form($form, $form_state, $repid) {

  $form = array();
  $form['#repid'] = $repid;
  $form['save_button'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
    '#id' => 'report-builder-save-button',
  );

  return $form;
}

/**
 * Submit function for report_builder_edit_save_button().
 */
function report_builder_edit_save_button_form_submit($form, &$form_state) {

  $repid = $form['#repid'];

  ctools_include('object-cache');
  $report = ctools_object_cache_get('report_builder', $repid);

  if (!is_null($report)) {
    if (isset($report)) {
      unset($report->data['updated']);
    }
    report_builder_save($report);
    $new_table = report_builder_get_table($report);
  }
  else {
    $report = report_builder_load($repid);
  }
  drupal_set_message(t('Report has been saved'));

  ctools_object_cache_clear('report_builder', $repid);
}

/**
 * Uses confirm_form() to confirm the deletion of a report.
 */
function report_builder_delete_form($form, &$form_state, $report) {
  $form['#report'] = $report;
  $question = t("Are you sure you want to delete the report %title ?", array('%title' => $report->title));
  return confirm_form($form, $question, 'admin/structure/report-builder');
}

/**
 * Submit function for report_builder_delete_form(). Deletes the report.
 */
function report_builder_delete_form_submit($form, &$form_state) {
  $report = $form['#report'];
  $query = db_delete('report_builder')->condition('repid', $report->repid);
  $query->execute();
  $form_state['redirect'] = 'admin/structure/report-builder';
  drupal_set_message(t('Report %title has been deleted.', array('%title' => $report->title)));
}
