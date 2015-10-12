<?php

/**
 * @file
 * Module settings UI.
 */

function chamilo_admin_settings() {
  /**
   * Space construct aditional variables, do validation etc.
   */
  return drupal_get_form('chamilo_admin_settings_form');
}

function chamilo_admin_settings_form() {
  $form['chamilo_sso_protocol'] = array(
    '#type' => 'radios',
    '#title' => t('SSO Server Protocol'),
    '#options' => array('http://', 'https://'),
    '#default_value' => variable_get('chamilo_sso_protocol', 0),
  );
  $form['chamilo_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Application URL and Path'),
    '#description' => t('NOTICE: Do NOT use trailing slash nor http://. i.e: yourchamilosite.com'),
    '#default_value' => variable_get('chamilo_server', ''),
  );
  $form['chamilo_accounts'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync with chamilo accounts.'),
    '#default_value' => variable_get('chamilo_accounts'),
    '#description' => t('If enabled, system will create or override chamilo accounts with same username, email and password.')
  );
  $form['chamilo_courses_block_items'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Trainings Visibilities'),
    '#description' => t('If this site displays a block with a list of your courses, will be filtered by criterias above.'),
    '#default_value' => variable_get('chamilo_courses_block_items', array('public')),
    '#options' => chamilo_course_visibility(),
  );
  $form['chamilo_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('User ID (example: user1).'),
    '#size' => 15,
    '#default_value' => variable_get('chamilo_user', ''),
  );
  $form['account']['chamilo_appkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Security Key'),
    '#default_value' => variable_get('chamilo_appkey', ''),
    '#size' => 30,
  );
  return system_settings_form($form);
}
