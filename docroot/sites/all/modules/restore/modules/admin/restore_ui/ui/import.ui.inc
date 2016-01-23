<?php
/**
 * @file
 * The import UI form.
 */

/**
 * The import form callback.
 */
function restore_ui_script_import($form, &$form_state) {
  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('The machine name'),
    '#description' => t('Leave blank to use either a name provided in the JSON or an MD5 hash.'),
  );

  $form['overwrite'] = array(
    '#type' => 'checkbox',
    '#title' => t('Overwrite'),
    '#description' => t('Overwrite a script if it exists.'),
  );

  $form['script'] = array(
    '#type' => 'textarea',
    '#title' => t('Importable JSON script'),
    '#required' => TRUE,
    '#rows' => 10,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * The form validate callback.
 */
function restore_ui_script_import_validate($form, &$form_state) {
  $name = $form_state['values']['name'];
  if (!empty($name)) {
    if (preg_match('@^_+$@', $name)) {
      form_error($form['name'], t('The machine-readable name must contain unique characters.'));
    }

    // Verify that the machine name contains no disallowed characters.
    if (preg_match('@[^a-z0-9_]+@', $name)) {
      form_error($form['name'], t('The machine-readable name must contain only lowercase letters, numbers, and underscores.'));
    }
  }

  try {
    $json = json_decode($form_state['values']['script'], TRUE);
  }
  catch (Exception $e) {
    form_error($form['script'], t('There was an error while decoding the JSON.'));
  }

  if ($form_state['values']['overwrite'] == FALSE) {
    $scripts = restore_scripts();

    if (empty($name)) {
      $name = isset($json['name']) ? $json['name'] : md5(serialize($json));
    }

    if (isset($scripts[$name])) {
      form_error($form['script'], t('A script with this name already exists.'));
    }
  }
}

/**
 * The form submit callback.
 */
function restore_ui_script_import_submit($form, &$form_state) {
  $script = json_decode($form_state['values']['script'], TRUE);

  $name = $form_state['values']['name'];
  if (empty($name)) {
    $name = isset($script['name']) ? $script['name'] : md5(serialize($script));
  }

  $script['name'] = $name;

  if (restore_ui_save((object) $script)) {
    $form_state['redirect'] = RESTORE_MENU_PATH . '/list';
    drupal_set_message(t('The script @name has been created.', array(
      '@name' => $name,
    )));
  }
}
