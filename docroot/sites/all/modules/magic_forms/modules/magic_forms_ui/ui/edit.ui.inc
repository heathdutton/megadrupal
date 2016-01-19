<?php
/**
 * @file
 * Edit a form config.
 */

/**
 * The edit form callback.
 */
function magic_forms_ui_edit_form($form, &$form_state, $name = '') {
  $form = array();

  if (!isset($form_state['config'])) {
    $config = magic_forms_ui_config($name);
    if (!$config) {
      drupal_set_message(t('Failed to find the form configuration "@name".', array('@name' => $name)), 'error');
      return drupal_goto(MAGIC_FORMS_BASE_PATH);
    }

    $form_state['config'] = $config;
  }
  else {
    $config = $form_state['config'];
  }

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General details'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $form['general']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#disabled' => TRUE,
    '#default_value' => $name,
  );

  $form['general']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#maxlength' => 250,
    '#default_value' => $config->title,
  );

  $form['general']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#maxlength' => 250,
    '#default_value' => $config->description,
  );

  $form['general']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Status'),
    '#default_value' => $config->status,
  );

  $form['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $form['form']['formid'] = array(
    '#type' => 'textfield',
    '#title' => t('Form ID'),
    '#maxlength' => 120,
    '#required' => TRUE,
    '#default_value' => $config->form_id,
  );

  $form['form']['formphp'] = array(
    '#type' => 'textarea',
    '#title' => t('Selection PHP'),
    '#description' => filter_xss(t('Some PHP code to allow you to further granuality in selecting the form, do not use <?php tags. The available variables are $config, $form, $form_state and $form_id.')),
    '#default_value' => isset($config->data['formphp']) ? $config->data['formphp'] : '',
  );

  $form['config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Config'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $form['config']['errors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Error handling'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['config']['errors'][MAGIC_FORMS_ERROR_HANDLING] = array(
    '#type' => 'checkbox',
    '#title' => t('Override the core error handling'),
    '#description' => t('Use magic forms error handling rather than core Drupal.'),
    '#default_value' => isset($config->data['config'][MAGIC_FORMS_ERROR_HANDLING]) ? $config->data['config'][MAGIC_FORMS_ERROR_HANDLING] : NULL,
  );

  $form['config']['errors']['grouped_errors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Grouped error messages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['config']['errors']['grouped_errors'][MAGIC_FORMS_GROUP_ERRORS] = array(
    '#type' => 'checkbox',
    '#title' => t('Group error messages'),
    '#description' => t('All field errors will be grouped (like core drupal) instead of individual errors.'),
    '#default_value' => isset($config->data['config'][MAGIC_FORMS_GROUP_ERRORS]) ? $config->data['config'][MAGIC_FORMS_GROUP_ERRORS] : NULL,
  );

  $form['config']['errors']['grouped_errors'][MAGIC_FORMS_GROUP_ERRORS_ANCHOR] = array(
    '#type' => 'checkbox',
    '#title' => t('Anchor grouped error messages'),
    '#description' => t('The error messages will provide an anchor link to the relevant field.'),
    '#default_value' => isset($config->data['config'][MAGIC_FORMS_GROUP_ERRORS_ANCHOR]) ? $config->data['config'][MAGIC_FORMS_GROUP_ERRORS_ANCHOR] : NULL,
  );

  $form['config']['errors']['field_errors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field error messages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['config']['errors']['field_errors'][MAGIC_FORMS_FIELD_ERRORS] = array(
    '#type' => 'checkbox',
    '#title' => t('Field error messages'),
    '#description' => t('With this checked, all errors will be displayed next to the field itself.'),
    '#default_value' => isset($config->data['config'][MAGIC_FORMS_FIELD_ERRORS]) ? $config->data['config'][MAGIC_FORMS_FIELD_ERRORS] : NULL,
  );

  $form['config']['errors']['field_errors'][MAGIC_FORMS_FIELD_ERROR_ASPREFIX] = array(
    '#type' => 'checkbox',
    '#title' => t('Prefix the error messages to the field'),
    '#description' => t('This will place the error messages before the input, otherwise they will be after the input.'),
    '#default_value' => isset($config->data['config'][MAGIC_FORMS_FIELD_ERROR_ASPREFIX]) ? $config->data['config'][MAGIC_FORMS_FIELD_ERROR_ASPREFIX] : NULL,
  );

  $form['config']['errors']['field_errors'][MAGIC_FORMS_FIELD_ERRORS_WRAPPER] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply an error class to the field wrapper'),
    '#description' => t('This will put the defined error class on the fields wrapper.'),
    '#default_value' => isset($config->data['config'][MAGIC_FORMS_FIELD_ERRORS_WRAPPER]) ? $config->data['config'][MAGIC_FORMS_FIELD_ERRORS_WRAPPER] : NULL,
  );

  $form['config']['errors']['field_errors'][MAGIC_FORMS_FIELD_ERRORS_WRAPPER_CLASS] = array(
    '#type' => 'textfield',
    '#title' => t('The field wrapper error class'),
    '#description' => t('This will css class will be applied to the wrapper.'),
    '#default_value' => isset($config->data['config'][MAGIC_FORMS_FIELD_ERRORS_WRAPPER_CLASS]) ? $config->data['config'][MAGIC_FORMS_FIELD_ERRORS_WRAPPER_CLASS] : NULL,
  );

  $form['fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field configs'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $form['fields']['fields'] = array(
    '#prefix' => '<div id="magic-form-fields">',
    '#suffix' => '</div>',
  );

  if (isset($config->data['fields']) && count($config->data['fields'])) {
    foreach ($config->data['fields'] as $xpath => $field_config) {
      $form['fields']['fields'][$xpath] = array(
        '#type' => 'fieldset',
        '#title' => t('Field: @xpath', array('@xpath' => $xpath)),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#tree' => TRUE,
      );

      $form['fields']['fields'][$xpath]['delete'] = array(
        '#type' => 'checkbox',
        '#title' => t('Delete'),
      );

      $form['fields']['fields'][$xpath]['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Override the title'),
        '#default_value' => isset($field_config['title']) ? $field_config['title'] : NULL,
      );

      $form['fields']['fields'][$xpath]['description'] = array(
        '#type' => 'textfield',
        '#title' => t('Override the description'),
        '#default_value' => isset($field_config['description']) ? $field_config['description'] : NULL,
      );

      $form['fields']['fields'][$xpath]['required-error'] = array(
        '#type' => 'textfield',
        '#title' => t('Override the required error message'),
        '#default_value' => isset($field_config['required-error']) ? $field_config['required-error'] : NULL,
      );

      $form['fields']['fields'][$xpath]['placeholder'] = array(
        '#type' => 'textfield',
        '#title' => t('Override the placeholder text'),
        '#default_value' => isset($field_config['placeholder']) ? $field_config['placeholder'] : NULL,
      );
    }
  }

  $form['fields']['fields']['__add'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add a new field config'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $form['fields']['fields']['__add']['xpath'] = array(
    '#type' => 'textfield',
    '#title' => t('Field xpath'),
    '#description' => t('The path to the field using | as a delimited, e.g. "actions|cancel".'),
  );

  $form['fields']['fields']['__add']['button'] = array(
    '#type' => 'submit',
    '#value' => t('Add field config'),
    '#submit' => array('magic_forms_ui_edit_form_add_field_config'),
    '#ajax' => array(
      'wrapper' => 'magic-form-fields',
      'callback' => 'magic_forms_ui_edit_form_add_field_config_ajax',
    ),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes...'),
  );

  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#limit_validation_errors' => array(),
    '#submit' => array('magic_forms_ui_edit_form_delete'),
  );

  return $form;
}

/**
 * The add field config submit callback.
 */
function magic_forms_ui_edit_form_add_field_config($form, &$form_state) {
  $xpath = $form_state['values']['fields']['fields']['__add']['xpath'];
  $form_state['config']->data['fields'][$xpath] = array();
  $form_state['rebuild'] = TRUE;
}

/**
 * The add field config ajax callback.
 */
function magic_forms_ui_edit_form_add_field_config_ajax($form, &$form_state) {
  return $form['fields']['fields'];
}

/**
 * The edit form submit callback.
 */
function magic_forms_ui_edit_form_submit($form, &$form_state) {
  $config = $form_state['config'];
  if (!empty($form_state['values'])) {
    $config->title = $form_state['values']['general']['title'];
    $config->description = $form_state['values']['general']['description'];
    $config->status = $form_state['values']['general']['status'];

    $config->form_id = $form_state['values']['form']['formid'];
    $config->data['formphp'] = $form_state['values']['form']['formphp'];
    $config->data['config'] = $form_state['values']['config'];
    $config->data['fields'] = array();

    foreach ($form_state['values']['fields']['fields'] as $xpath => $field_config) {
      if ($xpath == '__add' || (isset($field_config['delete']) && $field_config['delete'])) {
        continue;
      }

      $config->data['fields'][$xpath] = $field_config;
    }

    drupal_write_record('magic_forms', $config, array('name'));
    drupal_set_message(t('Successfully saved the form config.'));
  }
}

/**
 * The edit form delete callback.
 */
function magic_forms_ui_edit_form_delete($form, &$form_state) {
  $config = $form_state['config'];
  $form_state['redirect'] = MAGIC_FORMS_UI_BASE_PATH . "/{$config->name}/delete";
}
