<?php

/**
 * @file
 * Administrative pages and associated functions.
 */

/**
 * Page callback for overview page of all reports.
 */
function report_builder_list_reports() {

  // @todo replace with a view or use pager - this could get unweildy with a lot
  // of reports.
  // Get all of the reports.
  $reports = report_builder_fetch_all_reports();

  $table = array(
    '#theme' => 'table',
    '#header' => array(
      t('Report Title'),
      t('Author'),
      t('Edit'),
      t('Delete'),
    ),
    '#rows' => array(),
    '#attributes' => array(
      'id' => 'report-builder-admin-table',
    ),
  );

  // Set up the report table rows with title, author, edit and delete links.
  foreach ($reports as $report) {
    $table['#rows'][] = array(
      l($report->title, 'report-builder/' . $report->repid),
      l($report->name, 'user/' . $report->uid),
      l(t('Edit'), 'report-builder/' . $report->repid . '/build'),
      l(t('Delete'), 'report-builder/' . $report->repid . '/delete'),
    );
  }

  $intro_markup = "<p>" . t('Select a report below to view or edit.  You can also <a href="@new-report-url">add a new report</a>', array('@new-report-url' => url('report-builder/build')));
  $intro_markup .= ".</p>";

  $page = array(
    'intro_text' => array(
      '#markup' => $intro_markup,
    ),
    'table' => $table,
  );

  return $page;
}

/**
 * Form for creating new reports or updating an existing report's configuration.
 * The $new variable is populated first and is then used to determine whether or
 * not this is a new report, or an existing report is being updated.
 */
function report_builder_configure_report_form($form, &$form_state) {

  $new = FALSE;
  if (!isset($form_state['repid'])) {
    $new = TRUE;
    $form_state['#new'] = TRUE;
  }
  else {
    $cached_report = ctools_object_cache_get('report_builder', $form_state['repid']);
    if (!is_null($cached_report)) {
      $report = $cached_report;
    }
    else {
      $report = report_builder_load($form_state['repid']);
    }
  }
  if ($new) {
    drupal_set_title(t("Create a New Report"));
  }
  else {
    drupal_set_title(t("Update Report Configuration"));
  }

  // Load all views.
  $views = views_get_all_views();

  $report_view_keys = array();

  // Loop through all of the views and check for report_builder display types.
  // Build arrays of the report builder view titles keyed by thei view name and
  // Display name serialized.
  foreach ($views as $view) {
    foreach ($view->display as $display) {
      if ($display->display_plugin == 'report_builder') {
        $report_view_key = serialize(array($view->name, $display->id));
        if (!in_array($report_view_key, $report_view_keys)) {
          $report_view_keys[] = $report_view_key;
          $data_source_options[$report_view_key] = $view->get_title() . (($display->display_title == "Report Builder") ? "" : (" - " . $display->display_title));
        }
      }
    }
  }

  $form = array();

  if (empty($data_source_options)) {
    drupal_set_message(t('You need to create some report builder views before you can use report builder'), 'error');
  }
  else {
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Report title'),
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
      '#description' => t('The title of the new report'),
      '#default_value' => (($new) ? "" : $report->title),
    );

    if ($new) {
      $description = t("Select the 'data source' for your report.  The data source provides the information you want to display in your report.");
    }
    else {
      $description = t("Once the report has been created, the data source cannot be changed.");
    }

    // The list of views that can act as data sources for the report.
    // Use a serialized array of the view and display names as the key for the
    // view / display combination.
    $form['view_and_display'] = array(
      '#type' => 'select',
      '#title' => t('Data Source'),
      '#options' => $data_source_options,
      '#description' => $description,
      '#default_value' => (($new) ? "" : serialize(array($report->data['view_name'], $report->data['display_id']))),
      '#disabled' => (!($new)),
    );

    // How many columns should the table have? 12 is currently the arbitrary
    // max.  We don't want users creating a 10,000 column table.
    $form['num_groups'] = array(
      '#type' => 'select',
      '#title' => t('Number of Data Subgroups'),
      '#options' => array(
        1 => '1',
        2 => '2',
        3 => '3',
        4 => '4',
        5 => '5',
        6 => '6',
        7 => '7',
        8 => '8',
        9 => '9',
        10 => '10',
        11 => '11',
        12 => '12',
      ),
      '#description' => t("How many different ways to you want to group the data?"),
      '#default_value' => (($new) ? "" : $report->data['num_groups']),
    );

    $form['chart_type'] = array(
      '#type' => 'select',
      '#title' => t('Chart Type'),
      '#options' => chart_types(),
      '#required' => TRUE,
      '#default_value' => (($new) ? CHART_TYPE_BAR_V : $report->data['chart_data']['type']),
    );

    $form['chart_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Chart Width'),
      '#description' => t('Chart width in pixels'),
      '#size' => 8,
      '#required' => TRUE,
      '#default_value' => (($new) ? 500 : $report->data['chart_data']['width']),
    );

    $form['chart_height'] = array(
      '#type' => 'textfield',
      '#title' => t('Chart Height'),
      '#description' => t('Chart height in pixels'),
      '#size' => 8,
      '#required' => TRUE,
      '#default_value' => (($new) ? 200 : $report->data['chart_data']['height']),
    );

    $form['create_block_table'] = array(
      '#type' => 'checkbox',
      '#title' => t('Create Table Block'),
      '#description' => t("Check the box to create a block from this report's table"),
      '#default_value' => (($new) ? FALSE : $report->data['create_block_table']),
    );

    $form['create_block_chart'] = array(
      '#type' => 'checkbox',
      '#title' => t('Create Graph Block'),
      '#description' => t("Check the box to create a block from this report's chart"),
      '#default_value' => (($new) ? FALSE : $report->data['create_block_chart']),
    );

    // Submit button.
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => (($new) ? t('Build Report') : t('Update Configuration')),
    );
    if (!($new)) {
      $form['#report'] = $report;
    }

    return $form;
  }
}

/**
 *  Submit function for report_builder_configure_report_form_submit() form.
 */
function report_builder_configure_report_form_submit($form, &$form_state) {

  $new = FALSE;
  if (isset($form_state['#new'])) {
    $new = TRUE;
  }
  else {
    $report = $form['#report'];
  }

  ctools_include('object-cache');

  if ($new) {
    $report = entity_create('report_builder',array());
    report_builder_initialize($report, $form_state);
    $report = report_builder_save($report);
  }
  else {
    report_builder_update_configuration($report, $form_state);
  }

  $report->data['updated'] = TRUE;
  $report->data['chart_data']['type'] = $form_state['values']['chart_type'];
  $report->data['chart_data']['height'] = $form_state['values']['chart_height'];
  $report->data['chart_data']['width'] = $form_state['values']['chart_width'];

  $report->data['create_block_chart'] = $form_state['values']['create_block_chart'];
  $report->data['create_block_table'] = $form_state['values']['create_block_table'];

  ctools_object_cache_set('report_builder', $report->repid, $report);

  $form_state['redirect'] = "report-builder/" . $report->repid . "/build";
  if ($new) {
    drupal_set_message(t("New report, %title, has been created.  Click 'Save Changes' below to permenantly save your report", array('%title' => $report->title)), 'warning');
  }
  else {
    drupal_set_message(t("Report configuration updated. Click 'Save Changes' to permenantly save the configuration."), 'warning');
  }
}

/**
 * Ajax callback for existing report configuration updates.
 *
 * @param boolean $js
 *   Whether or not javascript is on.
 * @param integer $repid
 *   The report id.
 *
 * @return array
 *   Either a rendered page or a set of AJAX commands.
 *
 */
function report_builder_configure_report($js, $repid) {

  $form_state = array(
    'ajax' => TRUE,
    'title' => t('Configure Report'),
    'repid' => $repid,
  );

  if ($js) {

    ctools_include('modal');
    ctools_include('ajax');
    ctools_include('object-cache');
    $output = ctools_modal_form_wrapper('report_builder_configure_report_form', $form_state);

    if (!empty($form_state['executed'])) {
      $report = ctools_object_cache_get('report_builder', $repid);
      $output = array();
      $output[] = ajax_command_remove('.messages');
      $output[] = ctools_modal_command_dismiss();
      $table = report_builder_get_table($report);
      $table = _report_builder_table_add_edit_row($table, $repid);
      $output[] = ajax_command_replace('#report-builder-table', theme('status_messages') . render($table));
      $output[] = ajax_command_replace('#report-builder-title', '<h2 id="report-builder-title">' . check_plain($report->title) . '</h2>');
    }
    print ajax_render($output);
    exit;
  }
  else {
    $form = drupal_build_form('report_builder_configure_report_form', $form_state);
    return render($form);
  }
}

/**
 * Initializes the report array.
 *
 * @param object $report
 *   The report to initialize.
 * @param array $form_state
 *   The form state from the report configuration form.
 */
function report_builder_initialize(&$report, $form_state) {

  $view_display_name = unserialize($form_state['values']['view_and_display']);
  $view_name = $view_display_name[0];
  $display_id = $view_display_name[1];
  $num_groups = $form_state['values']['num_groups'];
  $title = $form_state['values']['title'];

  $view = views_get_view($view_name);
  $view->set_display($display_id);
  $view->preview();
  $view_result = _report_builder_extract_view_result($view);

  global $user;
  $report->title = $title;
  $report->uid = $user->uid;
  $report->data = array(
    'num_groups' => $num_groups,
    'view_name' => $view_name,
    'display_id' => $display_id,
    'form_inputs' => array(),
    'results' => array(),
    'exposed_form_keys' => array(),
    'headers' => array(),
  );

  $i = 0;

  while ($i < $num_groups) {
    $report->data['headers'][$i] = 'All';
    $report->data['results'][$i] = $view_result;
    $i++;
  }
}

/**
 * Updates a report's configuration when changes are made after the initial
 * save.
 *
 * @param object $report
 *   The report to be updated.
 * @param array $form_state
 *   The form state from the configuration form.
 */
function report_builder_update_configuration(&$report, $form_state) {
  $report->title = $form_state['values']['title'];
  $original_num_groups = $report->data['num_groups'];
  $report->data['num_groups'] = $form_state['values']['num_groups'];

  if ($original_num_groups != $report->data['num_groups']) {

    if ($original_num_groups > $report->data['num_groups']) {
      $i = $report->data['num_groups'];
      while ($i < $original_num_groups) {
        unset($report->data['form_inputs'][$i]);
        unset($report->data['results'][$i]);
        unset($report->data['headers'][$i]);
        $i++;
      }
    }
    else {
      $view_display_name = unserialize($form_state['values']['view_and_display']);
      $view_name = $view_display_name[0];
      $display_id = $view_display_name[1];
      $num_groups = $form_state['values']['num_groups'];
      $title = $form_state['values']['title'];

      $view = views_get_view($view_name);
      $view->set_display($display_id);
      $view->preview();
      $view_result = _report_builder_extract_view_result($view);

      $i = $original_num_groups;
      while ($i < $report->data['num_groups']) {
        $report->data['headers'][$i] = 'All';
        $report->data['results'][$i] = $view_result;
        $i++;
      }
    }
  }
}
