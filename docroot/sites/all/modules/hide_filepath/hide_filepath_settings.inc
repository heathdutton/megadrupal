<?php
/**
 * @file
 * Configuration settings for Hide Filepath module.
 */

/**
 * Implements hook_settings().
 */
function hide_filepath_settings($form, &$form_state) {
  $form = array();
  $form['hide_filepath_anonymous'] = array(
    '#type' => 'select',
    '#title' => t('Anonymous User'),
    '#description' => t('Choose what an Anonymous User should receive when clicking a file path.'),
    '#options' => array(
      '403' => t('Access Denied'),
      '404' => t('Page Not Found'),
      'login' => t('Login challenge'),
      'custom' => t('Use custom hook'),
      'bypass' => t('Allow Normal Access'),
    ),
    '#default_value' => variable_get('hide_filepath_anonymous', 'login'),
    '#required' => TRUE,
  );

  $form['hide_filepath_authenticated'] = array(
    '#type' => 'select',
    '#title' => t('Authenticated User'),
    '#description' => t('Choose what an Authenticated User should receive when clicking a file path.'),
    '#options' => array(
      '403' => t('Access Denied'),
      '404' => t('Page Not Found'),
      'custom' => t('Use custom hook'),
      'bypass' => t('Allow Normal Access'),
    ),
    '#default_value' => variable_get('hide_filepath_authenticated', 'bypass'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
