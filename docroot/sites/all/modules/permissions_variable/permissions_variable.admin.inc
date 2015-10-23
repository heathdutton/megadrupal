<?php
/**
 * @file
 * Administration page callbacks for permissions_variable.module
 */

/**
 * Admin configuration form.
 */
function permissions_variable_admin_form() {
  $form = array();

  $form['permissions_variable_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable permissions importing and UI locking'),
    '#description' => t('Any permission definition found in the global $permissions variable will <em>permanently</em> overwrite that permission in the database and the relevant checkboxes in the permissions UI will be disabled.'),
    '#default_value' => permissions_variable_variable('permissions_variable_enabled'),
  );
  $enabled_state = array(
    'invisible' => array(
      ':checkbox[name="permissions_variable_enabled"]' => array('checked' => FALSE),
    ),
  );

  $form['permissions_variable_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enforce permissions when the cache is cleared'),
    '#description' => t('Permissions in the database will be re-synced with $permissions on every cache clear. See the README.txt file for more information about how and when permissions are enforced.'),
    '#default_value' => permissions_variable_variable('permissions_variable_cache'),
    '#states' => $enabled_state,
  );

  $form['permissions_variable_create_roles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create new user roles found in $permissions'),
    '#description' => t('When roles are referenced in $permissions that do not exist in the current site, these new roles will be created automatically as required.'),
    '#default_value' => permissions_variable_variable('permissions_variable_create_roles'),
    '#states' => $enabled_state,
  );

  $form['permissions_variable_lock_roles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Lock user roles UI'),
    '#description' => t('User roles cannot be created, modified or deleted through the UI.'),
    '#default_value' => permissions_variable_variable('permissions_variable_lock_roles'),
  );

  $form['permissions_variable_verbose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display status messages on locked forms'),
    '#default_value' => permissions_variable_variable('permissions_variable_verbose'),
  );

  $form = system_settings_form($form);

  // Import permissions after the system settings submit handler.
  $form['#submit'][] = 'permissions_variable_import';

  return $form;
}

/**
 * Form to export $permissions variable.
 */
function permissions_variable_export_form() {
  $form = array();

  $export = permissions_variable_export();
  $form['export'] = array(
    '#type' => 'textarea',
    '#rows' => 30,
    '#default_value' => $export,
    '#value' => $export,
  );

  return $form;
}
