<?php
/**
 * @file
 * Provides an administration for for the Videopublishing module.
 */


/**
 * Builds the main videopublishing admin form.
 */
function videopublishing_admin_form($form, $form_state) {
  $settings = LazyVars::get('videopublishing_settings', NULL);
  if (isset($settings['api_key']) && isset($settings['api_user']) && $settings['api_key'] && $settings['api_user']) {
    $config_collapsed = TRUE;
  }
  else {
    $config_collapsed = FALSE;
  }

  $form['config'] = array(
    '#type' => 'fieldset',
    '#title' => t('VideoPublishing.com Config'),
    '#collapsible' => TRUE,
    '#collapsed' => $config_collapsed,
    '#tree' => TRUE,
  );

  $form['config']['api_user'] = array(
    '#type' => 'textfield',
    '#title' => t('API User'),
    '#default_value' => $settings ? $settings['api_user'] : '',
    '#required' => TRUE,
  );

  $form['config']['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => $settings ? $settings['api_key'] : '',
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


/**
 * Submit handler for the videopublishing admin form.
 */
function videopublishing_admin_form_submit($form, $form_state) {
  LazyVars::set('videopublishing_settings', $form_state['values']['config']);
  drupal_set_message(t('The videopublishing settings were successfully saved.'));
}