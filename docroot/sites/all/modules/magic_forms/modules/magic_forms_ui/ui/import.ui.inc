<?php
/**
 * @file
 * Import a new form config.
 */

/**
 * Check if the form config name exists.
 */
function _magic_forms_ui_config_exists($value) {
  return db_query_range('SELECT 1 FROM {magic_forms} WHERE name = :name', 0, 1, array(':name' => $value))->fetchField();
}

/**
 * The import form callback.
 */
function magic_forms_ui_import_form($form, &$form_state) {
  $form = array();

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Name'),
    '#description' => t('If left blank the name from the exported config will be used and if available.'),
    '#maxlength' => 120,
    '#machine_name' => array(
      'exists' => '_magic_forms_ui_config_exists',
    ),
    '#required' => FALSE,
  );

  $form['input'] = array(
    '#type' => 'textarea',
    '#title' => t('The exported config'),
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import config...'),
  );

  return $form;
}

/**
 * The import form validate callback.
 */
function magic_forms_ui_import_form_validate($form, &$form_state) {
  try {
    $config = json_decode($form_state['values']['input'], TRUE);
    $config = (object) $config;

    if (!isset($config->name)) {
      form_error($form['input'], t('This is an invalid form config export!'));
    }
  }
  catch (Exception $e) {
    form_error($form['input'], $e->getMessage());
    return;
  }

  $name = empty($form_state['values']['name']) ? $config->name : $form_state['values']['name'];
  $form['name']['#value'] = $name;
  form_validate_machine_name($form['name'], $form_state);
}

/**
 * The import form submit callback.
 */
function magic_forms_ui_import_form_submit($form, &$form_state) {
  $config = (object) json_decode($form_state['values']['input']);

  drupal_write_record('magic_forms', $config);

  drupal_set_message(t('You have successfully imported a new magic form config.'));
  $form_state['redirect'] = MAGIC_FORMS_UI_BASE_PATH;
}
