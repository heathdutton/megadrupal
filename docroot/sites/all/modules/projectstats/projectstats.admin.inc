<?php

/**
 * @file
 * Administration file.
 */

/**
 * List all projects - page callback.
 */
function projectstats_list_page() {
  $output = '';
  $output .= '<h2>' . l(t('Add project'), 'admin/config/system/projectstats/add') . '</h2>';
  $data = projectstats_load_all();

  if ($data) {
    $data_table = array();
    // Prepare data for table.
    foreach ($data as $row) {
      $row = (array) $row;
      $full_url = PROJECSTATS_MODULE_DRUPAL_ORG_PREFIX . $row['short_project_name'];
      $row['url'] = l($full_url, $full_url);
      $row['update_data'] = l(t('Update data'), PROJECSTATS_MODULE_URI . '/' . $row['pid'] . '/update-data');
      $row['edit'] = l(t('Edit'), PROJECSTATS_MODULE_URI . '/' . $row['pid'] . '/edit');
      $row['delete'] = l(t('Delete'), PROJECSTATS_MODULE_URI . '/' . $row['pid'] . '/delete');
      $data_table[] = $row;
    }
    $header = projectstats_list_header();

    $output .= theme('table', array(
      'header' => $header,
      'rows' => $data_table,
      'attributes' => array('width' => '100%')
    ));
  }
  return $output;
}

/**
 * Page callback.
 */
function projectstats_update_data_page() {
  if (arg(PROJECSTATS_MODULE_URI_ARG_POSITION + 1) == 'update-data') {
    $pid = arg(PROJECSTATS_MODULE_URI_ARG_POSITION);
    projectstats_project_update_data($pid);
  }
  drupal_goto(PROJECSTATS_MODULE_URI);
}

/**
 * Get header for table.
 */
function projectstats_list_header() {
  $header = array(
    t('pid'),
    t('Full project name'),
    t('Short project name'),
    t('Reported installs'),
    t('Downloads'),
    t('Updated'),
    t('URL'),
    t('Update data'),
    t('Edit'),
    t('Delete'),
  );
  return $header;
}

/**
 * Project Form (add,edit).
 */
function projectstats_form($form, &$form_state) {
  if (arg(PROJECSTATS_MODULE_URI_ARG_POSITION + 1) == 'edit') {
    $project = projectstats_load(arg(PROJECSTATS_MODULE_URI_ARG_POSITION));
    $project_full_name = $project->full_project_name;
    $short_project_name = $project->short_project_name;
    $form['pid'] = array('#type' => 'value', '#value' => $project->pid);
  }
  else {
    $project_full_name = '';
    $short_project_name = '';
    $form['#validate'][] = 'projectstats_form_validate';
  }

  $form['full_project_name'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Full project name'),
    '#default_value' => $project_full_name,
  );

  $form['short_project_name'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Short project name'),
    '#description' => t('Used in project url http://drupal.org/project/short_project_name'),
    '#default_value' => $short_project_name,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save project'),
  );


  return $form;
}

/**
 * Form validate.
 */
function projectstats_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $short_project_name = $values['short_project_name'];
  $url_prefix = PROJECSTATS_MODULE_DRUPAL_ORG_PREFIX;

  if (preg_match('/\s/', $values['short_project_name'])) {
    form_set_error('short_project_name', t('Please remove space from Short name field'));
  }

  if (!projectstats_url_exists($url_prefix . $short_project_name)) {
    form_set_error('short_project_name', t('Project %url not exists', array('%url' => $url_prefix . $short_project_name)));
  }
}

/**
 * Form submit.
 */
function projectstats_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  if (isset($values['pid']) && is_numeric($values['pid']) && projectstats_load($values['pid'])) {
    $row = new stdClass();
    $row->pid = $values['pid'];
    $row->full_project_name = $values['full_project_name'];
    $row->short_project_name = $values['short_project_name'];
    drupal_write_record('projectstats', $row, array('pid'));
    drupal_set_message(t("Project has been updated"));
  }
  else {
    $row = new stdClass();
    $row->full_project_name = $values['full_project_name'];
    $row->short_project_name = $values['short_project_name'];

    drupal_write_record('projectstats', $row);
    drupal_set_message(t("New project has been saved"));
  }

  $form_state['redirect'] = PROJECSTATS_MODULE_URI;
}

/**
 * Delete confirmation form.
 */
function projectstats_delete_form($form, &$form_state) {
  $form['pid'] = array('#type' => 'value', '#value' => arg(PROJECSTATS_MODULE_URI_ARG_POSITION));
  $project = projectstats_load(arg(PROJECSTATS_MODULE_URI_ARG_POSITION));
  return confirm_form($form,
    t('Are you sure you want to delete project %title?', array('%title' => $project->short_project_name)),
    PROJECSTATS_MODULE_URI,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete form submit handler.
 */
function projectstats_delete_form_submit($form, &$form_state) {
  $pid = $form_state['values']['pid'];
  projectstats_project_delete($pid);

  drupal_set_message(t('Project %pid deleted.', array('%pid' => $pid )));

  $form_state['redirect'] = PROJECSTATS_MODULE_URI;
}
