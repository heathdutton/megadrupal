<?php

/**
 * @file
 * Single User Role module admin inc file.
 *
 * Contains admin configuration page callback functions.
 */

/**
 * Callback function for module configuration page.
 */
function single_user_role_config_form($form, &$form_state) {
  $form['single_user_role_field_type'] = array(
    '#title' => t('Field type'),
    '#description' => t('Set type of field to use for user role selection.'),
    '#type' => 'select',
    '#default_value' => variable_get('single_user_role_field_type', 'select'),
    '#options' => array(
      'select' => t('Select field'),
      'radios' => t('Radio field'),
    ),
  );
  $form['single_user_role_field_desc'] = array(
    '#title' => 'Role field helptext',
    '#type' => 'textarea',
    '#description' => t('This text is displayed at user role field.'),
    '#default_value' => variable_get('single_user_role_field_desc', ''),
    '#cols' => 40,
    '#rows' => 4,
  );
  return system_settings_form($form);
}
