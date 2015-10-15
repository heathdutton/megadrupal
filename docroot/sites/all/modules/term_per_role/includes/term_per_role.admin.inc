<?php

/**
 * @file
 * Administration pages provided by term_per_role module.
 */

/**
 * Menu callback for admin/config/content/term-per-role.
 */
function term_per_role_admin_settings() {
  $form = array();
  $form['term_per_role_access_behavior'] = array(
    '#title' => t('Access is denied behavior'),
    '#description' => t('An error which will be shown when access is denied.'),
    '#type' => 'radios',
    '#options' => array(
      '403' => t('Show page 403'),
      '404' => t('Show page 404')
    ),
    '#default_value' => variable_get('term_per_role_access_behavior', '403')
  );

  $form = system_settings_form($form);
  return $form;
}