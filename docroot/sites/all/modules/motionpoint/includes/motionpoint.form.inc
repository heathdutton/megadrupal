<?php

/**
 * @file Form alters and other work with forms.
 */

/**
 * Form with motionpoint settings.
 */
function motionpoint_settings_form($form, &$form_state) {
  $form['motionpoint_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Motionpoint settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['motionpoint_settings']['prefix'] = array(
    '#type' => 'fieldset',
    '#title' => t('Prefixes'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $languages = language_list();
  $motionpoint_languages = variable_get('motionpoint_settings_languages', array());

  foreach ($languages as $language_key => $language) {
    $form['motionpoint_settings']['prefix'][$language_key] = array(
      '#type' => 'textfield',
      '#title' => t("{$language->name} prefix"),
      '#size' => 60,
      '#default_value' => $motionpoint_languages[$language_key],
      '#required' => TRUE,
    );
  }

  $form['motionpoint_settings']['resend_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Resend time'),
    '#description' => t('Time after which second request will be sent.'),
    '#size' => 10,
    '#default_value' => variable_get('motionpoint_settings_resend_time'),
  );

  $form['motionpoint_settings']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validate for motionpoint_settings_form.
 */
function motionpoint_settings_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['resend_time'])) {
    form_set_error($form_state['values']['resend_time'], 'Resend time is not a number. Please fill valid data.');
  }
}

/**
 * Submit for motionpoint_settings_form.
 */
function motionpoint_settings_form_submit($form, &$form_state) {
  $languages = language_list();

  foreach ($languages as $language_key => $input) {
    foreach ($form_state['values'] as $form_language_key => $form_input) {
      if ($form_language_key == $language_key) {
        $languages_var[$language_key] = $form_input;
      }
    }
  }

  if (!empty($languages_var)) {
    variable_set("motionpoint_settings_languages", $languages_var);
  }

  if (isset($form_state['values']['resend_time'])) {
    variable_set('motionpoint_settings_resend_time', $form_state['values']['resend_time']);
  }

  drupal_set_message('Your changes have been saved successfully.');
}
