<?php

/**
 * @file
 * Ajax callbacks for edit page and associated functions.
 */

/**
 * Ajax callback for displaying the data group edit form.
 *
 * @param report $report
 *   The report being edited.
 * @param boolean $js
 *   Whether or not javascript is in use.
 * @param integer $group_num
 *   The index of the data subgroup.
 *
 * @return array
 *   Either a the pages rendered HTML or the ajax commands.
 */
function report_builder_edit_report_group($report, $js, $group_num) {

  ctools_include('object-cache');

  $cached_report = ctools_object_cache_get('report_builder', $report->repid);
  if ($cached_report != NULL) {
    $report = $cached_report;
  }

  $report = report_builder_update_results($report);

  $form_state = array(
    'title' => t('Edit Report Subgroup'),
    'report' => $report,
    'group_num' => $group_num,
  );

  if ($js) {
    $form_state['ajax'] = TRUE;
    ctools_include('modal');
    ctools_include('ajax');

    $output = ctools_modal_form_wrapper('report_builder_edit_subgroup_form', $form_state);

    if (!empty($form_state['executed'])) {

      drupal_set_message(t('Changes have been made.  Click "Save Changes" below to permenantly save them.'), 'warning');
      $report = $form_state['report'];

      $ouptut = array();
      $output[] = ctools_modal_command_dismiss();
      $output[] = ajax_command_remove('.messages');
      $table = report_builder_get_table($report);
      $table = _report_builder_table_add_edit_row($table, $report->repid);
      $output[] = ajax_command_replace('#report-builder-table', theme('status_messages') . render($table));
    }
    print ajax_render($output);
  }
  else {
    $form = drupal_build_form('report_builder_edit_subgroup_form', $form_state);

    return render($form);
  }
}

/**
 * Form for subgroup edit.
 */
function report_builder_edit_subgroup_form($form, &$form_state) {

  $report = $form_state['report'];
  $group_num = $form_state['group_num'];
  $table = report_builder_get_table($report);

  $table['table']['#rows'][0][$group_num] = array(
    'data' => $table['table']['#rows'][0][$group_num],
    'class' => 'report-builder-current-group',
  );

  $form['table'] = array(
    '#markup' => render($table),
    '#weight' => -100,
  );

  $form['group_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Heading'),
    '#description' => t('The heading for this table column'),
    '#weight' => -99,
  );

  $views_form_elements = _report_builder_extract_views_form_elements($report->data['view_name'], $report->data['display_id']);
  $form = array_merge($form, $views_form_elements);

  foreach ($views_form_elements as $key => $element) {
    if (isset($report->data['form_inputs'][$group_num][$key])) {
      $form[$key]['#value'] = $report->data['form_inputs'][$group_num][$key];
    }
  }

  $form['group_title']['#value'] = $report->data['headers'][$group_num];

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * Submit function for report_builder_edit_subgroup_form().
 */
function report_builder_edit_subgroup_form_submit($form, &$form_state) {

  $report = $form_state['report'];
  $group_num = $form_state['group_num'];

  $report->data['exposed_form_keys'] = report_builder_get_exposed_form_keys($report);

  foreach ($report->data['exposed_form_keys'] as $input_key) {
    $report->data['form_inputs'][$group_num][$input_key] = $form_state['input'][$input_key];
  }

  // Newest data is only in 'input' here, not values.
  $report->data['headers'][$group_num] = $form_state['input']['group_title'];
  $report->data['results'][$group_num] = report_builder_get_view_result($report, $group_num);
  $report->data['updated'] = TRUE;
  ctools_include('object-cache');
  ctools_object_cache_set('report_builder', $report->repid, $report);
  $form_state['report'] = $report;
}

/**
 * Extracts the form elements from the view's exposed filter.
 *
 * @param string $view_name
 *   The name of the view where the filter form can be extracted.
 * @param string $display_id
 *   The diplsay id of the views display.
 *
 * @return array
 *   An array of exposed form filters.
 */
function _report_builder_extract_views_form_elements($view_name, $display_id) {
  $view = views_get_view($view_name);
  $view->set_display($display_id);
  $view->execute();
  $views_form_state = array(
    'view' => $view,
    'display' => $display_id,
  );

  $exposed_form = drupal_build_form('views_exposed_form', $views_form_state);

  $exposed_form_keys = element_children($exposed_form);
  $generic_elements = _report_builder_get_generic_elements();

  $form_elements = array();

  foreach ($exposed_form_keys as $index => $form_key) {
    if (!in_array($form_key, $generic_elements)) {

      $form_elements[$form_key] = $exposed_form[$form_key];
      $form_elements[$form_key]['#title'] = check_plain($exposed_form['#info']['filter-' . $form_key]['label']);
      // Sepcial case for date api form elements.
      if (isset($form_elements[$form_key]['value'])) {
        $form_elements[$form_key]['value']['#title'] = check_plain($exposed_form['#info']['filter-' . $form_key]['label']);
      }
    }
  }

  return $form_elements;
}
