<?php

/**
 * Filedepot Multiupload settings form.
 */
function filedepot_multiupload_admin_form($form, &$form_state) {
  $form['filedepot_multiupload_multibuttonname'] = array(
    '#type' => 'textfield',
    '#title' => t('Display name for Multi-upload button'),
    '#default_value' => variable_get('filedepot_multiupload_multibuttonname', 'Multiple Upload'),
    '#description' => t('The user facing name that the multi-upload button gets.'),
    '#required' => TRUE,
  );
  $form['filedepot_multiupload_shownewfilebutton'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Single Upload Button'),
    '#default_value' => variable_get('filedepot_multiupload_shownewfilebutton', TRUE),
    '#description' => t('Whether or not to display the non-multiupload button.'),
  );
  return system_settings_form($form);
}
