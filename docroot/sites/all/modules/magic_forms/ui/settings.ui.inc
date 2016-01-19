<?php
/**
 * @file
 * The magic forms settings form.
 */

/**
 * Settings form callback.
 */
function magic_forms_settings_form($form, &$form_state) {
  $form = array();

  $form[MAGIC_FORMS_VARS_APPLYTO] = array(
    '#type' => 'textarea',
    '#title' => t("A list of form id's to apply this configuration to"),
    '#description' => t('A form id per line.'),
    '#default_value' => variable_get(MAGIC_FORMS_VARS_APPLYTO, MAGIC_FORMS_VARS_APPLYTO_DEFAULT),
  );

  $form['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debug'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['debug'][MAGIC_FORMS_VARS_DEBUGMODE] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),
    '#description' => t('When debug mode is on, any problems with fields and validators is reported in the watchdog.'),
    '#default_value' => variable_get(MAGIC_FORMS_VARS_DEBUGMODE, MAGIC_FORMS_VARS_DEBUGMODE_DEFAULT),
  );

  $form['debug'][MAGIC_FORMS_VARS_DEBUG_FORMID] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the form id'),
    '#description' => t('Display the form id, this will use dpm if available otherwise drupal_set_message.'),
    '#default_value' => variable_get(MAGIC_FORMS_VARS_DEBUG_FORMID, MAGIC_FORMS_VARS_DEBUG_FORMID_DEFAULT),
  );

  if (module_exists('devel')) {
    $form['debug'][MAGIC_FORMS_VARS_DEBUG_FORM] = array(
      '#type' => 'checkbox',
      '#title' => t('Output the form contents'),
      '#description' => t('Display the form array, this will use dpm.'),
      '#default_value' => variable_get(MAGIC_FORMS_VARS_DEBUG_FORM, MAGIC_FORMS_VARS_DEBUG_FORM_DEFAULT),
    );
  }

  $form['errors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Error handling'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['errors'][MAGIC_FORMS_VARS_ERROR_HANDLING] = array(
    '#type' => 'checkbox',
    '#title' => t('Override the core error handling'),
    '#description' => t('Use magic forms error handling rather than core Drupal.'),
    '#default_value' => variable_get(MAGIC_FORMS_VARS_ERROR_HANDLING, MAGIC_FORMS_VARS_ERROR_HANDLING_DEFAULT),
  );

  $form['errors']['grouped_errors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Grouped error messages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['errors']['grouped_errors'][MAGIC_FORMS_VARS_GROUP_ERRORS] = array(
    '#type' => 'checkbox',
    '#title' => t('Group error messages'),
    '#description' => t('All field errors will be grouped (like core drupal) instead of individual errors.'),
    '#default_value' => variable_get(MAGIC_FORMS_VARS_GROUP_ERRORS, MAGIC_FORMS_VARS_GROUP_ERRORS_DEFAULT),
  );

  $form['errors']['grouped_errors'][MAGIC_FORMS_VARS_GROUP_ERRORS_ANCHOR] = array(
    '#type' => 'checkbox',
    '#title' => t('Anchor grouped error messages'),
    '#description' => t('The error messages will provide an anchor link to the relevant field.'),
    '#default_value' => variable_get(MAGIC_FORMS_VARS_GROUP_ERRORS_ANCHOR, MAGIC_FORMS_VARS_GROUP_ERRORS_ANCHOR_DEFAULT),
  );

  $form['errors']['field_errors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field error messages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['errors']['field_errors'][MAGIC_FORMS_VARS_FIELD_ERRORS] = array(
    '#type' => 'checkbox',
    '#title' => t('Field error messages'),
    '#description' => t('With this checked, all errors will be displayed next to the field itself.'),
    '#default_value' => variable_get(MAGIC_FORMS_VARS_FIELD_ERRORS, MAGIC_FORMS_VARS_FIELD_ERRORS_DEFAULT),
  );

  $form['errors']['field_errors'][MAGIC_FORMS_VARS_FIELD_ERROR_ASPREFIX] = array(
    '#type' => 'checkbox',
    '#title' => t('Prefix the error messages to the field'),
    '#description' => t('This will place the error messages before the input, otherwise they will be after the input.'),
    '#default_value' => variable_get(MAGIC_FORMS_VARS_FIELD_ERROR_ASPREFIX, MAGIC_FORMS_VARS_FIELD_ERROR_ASPREFIX_DEFAULT),
  );

  $form['errors']['field_errors'][MAGIC_FORMS_VARS_FIELD_ERRORS_WRAPPER] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply an error class to the field wrapper'),
    '#description' => t('This will put the defined error class on the fields wrapper.'),
    '#default_value' => variable_get(MAGIC_FORMS_VARS_FIELD_ERRORS_WRAPPER, MAGIC_FORMS_VARS_FIELD_ERRORS_WRAPPER_DEFAULT),
  );

  $form['errors']['field_errors'][MAGIC_FORMS_VARS_FIELD_ERRORS_WRAPPER_CLASS] = array(
    '#type' => 'textfield',
    '#title' => t('The field wrapper error class'),
    '#description' => t('This will css class will be applied to the wrapper.'),
    '#default_value' => variable_get(MAGIC_FORMS_VARS_FIELD_ERRORS_WRAPPER_CLASS, MAGIC_FORMS_VARS_FIELD_ERRORS_WRAPPER_CLASS_DEFAULT),
  );

  $form['skip_validation_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields that skip form validation'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['skip_validation_fields'][MAGIC_FORMS_VARS_PREVENT_VALIDATION_FIELDS] = array(
    '#type' => 'textarea',
    '#title' => t('A list of fields that skip form validation'),
    '#description' => t('This is useful for cancel and delete buttons which you do not want to validate the form on submission. Add a field per line, and separated by | for parent structures, e.g. "actions|cancel".'),
    '#default_value' => variable_get(MAGIC_FORMS_VARS_PREVENT_VALIDATION_FIELDS, MAGIC_FORMS_VARS_PREVENT_VALIDATION_FIELDS_DEFAULT),
  );

  return system_settings_form($form);
}
