<?php
/**
 * @file
 * The add script UI form.
 */

/**
 * The add form callback.
 */
function restore_ui_script_add($form, &$form_state) {
  $form = array();

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#description' => t('The restore scripts machine name.'),
    '#machine_name' => array(
      'exists' => 'restore_ui_exists',
    ),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The restore scripts title.'),
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('The restore scripts description.'),
  );

  $form['group'] = array(
    '#type' => 'textfield',
    '#title' => t('Group'),
    '#description' => t('The restore scripts group.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );

  return $form;
}

/**
 * The validate form callback.
 */
function restore_ui_script_add_validate($form, &$form_state) {
  $scripts = restore_scripts();
  if (isset($scripts[$form_state['values']['name']])) {
    form_error($form['name'], t('This name is already in use.'));
  }
}

/**
 * The submit form callback.
 */
function restore_ui_script_add_submit($form, &$form_state) {
  $record = (object) array(
    'name' => $form_state['values']['name'],
    'title' => $form_state['values']['title'],
    'description' => $form_state['values']['description'],
    'script_group' => $form_state['values']['group'],
    'status' => TRUE,
    'operations' => array(),
  );

  if (restore_ui_save($record)) {
    drupal_set_message(t('Restore script @name has been created.', array(
      '@name' => $record->name,
    )));

    $form_state['redirect'] = RESTORE_MENU_PATH . "/script/{$record->name}/edit";
  }
}
