<?php

/**
 * @file
 * Settings.
 */

/**
 * Page callback: Form constructor for the settings.
 *
 *
 * @return array
 *   A system_settings_form array.
 *
 * @see role_memory_limit_menu()
 */
function role_memory_limit_settings_form($form, &$form_state) {
  $form['role_memory_limit_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => variable_get('role_memory_limit_status', 0),
  );

  $form['role_memory_limit_roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Enter the memory in megabytes or -1 for unlimited.') . '<br />' . t('Leave empty for default'),
  );

  $form['role_memory_limit_roles']['role_memory_limit_role_admin'] = array(
    '#type' => 'textfield',
    '#title' => t('admin'),
    '#field_suffix' => 'MB',
    '#default_value' => variable_get('role_memory_limit_role_admin', ''),
  );

  foreach (user_roles() as $rid => $role) {
    $var_name = 'role_memory_limit_role_' . $rid;

    $form['role_memory_limit_roles'][$var_name] = array(
      '#type' => 'textfield',
      '#title' => check_plain($role),
      '#field_suffix' => 'MB',
      '#default_value' => variable_get($var_name, ''),
    );
  }

  $form['role_memory_limit_role_cli'] = array(
    '#type' => 'textfield',
    '#title' => t('command line (drush)'),
    '#field_suffix' => 'MB',
    '#default_value' => variable_get('role_memory_limit_role_cli', ''),
  );

  $form['role_memory_limit_role_image'] = array(
    '#type' => 'textfield',
    '#title' => t('Image processing'),
    '#field_suffix' => 'MB',
    '#description' => t('Increase the memory limit to at least this level for ImageCache generation.'),
    '#default_value' => variable_get('role_memory_limit_role_image', ''),
  );

  return system_settings_form($form);
}

/**
 * Form validation handler for role_memory_limit_settings().
 */
function role_memory_limit_settings_form_validate($form, &$form_state) {
  $roles = array_keys(user_roles());
  array_unshift($roles, 'admin', 'cli');

  foreach ($roles as $role) {
    $var_name = 'role_memory_limit_role_' . $role;

    if ($form_state['values'][$var_name] || $form_state['values'][$var_name] === '0') {
      if (!is_numeric($form_state['values'][$var_name])) {
        form_set_error($var_name, t('Only numbers is allowed.'));
      }
      elseif ($form_state['values'][$var_name] != -1) {
        if ($form_state['values'][$var_name] < 64) {
          form_set_error($var_name, t('Minimum allowed is 64 MB.'));
        }
        elseif ($form_state['values'][$var_name] > 2048) {
          form_set_error($var_name, t('Maximum allowed is 2048 MB.'));
        }
      }
    }
  }
}
