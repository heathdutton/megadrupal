<?php
/**
 * @file
 * Add a new form config.
 */

/**
 * Check if the form config name exists.
 */
function _magic_forms_ui_config_exists($value) {
  return db_query_range('SELECT 1 FROM {magic_forms} WHERE name = :name', 0, 1, array(':name' => $value))->fetchField();
}

/**
 * The add form callback.
 */
function magic_forms_ui_add_form($form, &$form_state) {
  $form = array();

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Name'),
    '#maxlength' => 120,
    '#machine_name' => array(
      'exists' => '_magic_forms_ui_config_exists',
    ),
    '#required' => TRUE,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#maxlength' => 250,
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#maxlength' => 250,
  );

  $form['formid'] = array(
    '#type' => 'textfield',
    '#title' => t('Form ID'),
    '#maxlength' => 120,
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add config...'),
  );

  return $form;
}

/**
 * The add form submit callback.
 */
function magic_forms_ui_add_form_submit($form, &$form_state) {
  $config = (object) array(
    'name' => $form_state['values']['name'],
    'title' => $form_state['values']['title'],
    'description' => $form_state['values']['description'],
    'form_id' => $form_state['values']['formid'],
    'status' => TRUE,
    'data' => array(),
  );

  drupal_write_record('magic_forms', $config);

  drupal_set_message(t('You have successfully created a new magic form config.'));
  $form_state['redirect'] = MAGIC_FORMS_UI_BASE_PATH . "/{$config->name}/edit";
}
