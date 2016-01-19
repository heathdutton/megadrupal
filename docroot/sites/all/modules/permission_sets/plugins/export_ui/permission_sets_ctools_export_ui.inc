<?php

/**
 * The permission form requires a role id. We use 1 (anonymous user) purely for
 * convenience and because its guaranteed to exist. Its use has no bearing on
 * the anonymous user role.
 */
define('PERMISSION_SETS_RID', 1);

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'permission_sets',
  'access' => 'administer permissions',

  // Define the menu item.
  'menu' => array(
    'menu item' => 'permission_sets',
    'menu title' => 'Permission Sets',
    'menu description' => 'Administer permission sets.',
  ),

  // Define user interface texts.
  'title singular' => t('permission set'),
  'title plural' => t('permission sets'),
  'title singular proper' => t('Permission set'),
  'title plural proper' => t('Permission sets'),

  // Define the class to use as a handler for DFP ad tags.
  'handler' => array(
    'class' => 'permission_sets_ui',
    'parent' => 'ctools_export_ui',
  ),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'permission_set_ctools_export_ui_form',
    'submit' => 'permission_set_ctools_export_ui_form_submit',
  ),
);

/**
 * Define the permission_set add/edit form.
 */
function permission_set_ctools_export_ui_form(&$form, &$form_state) {
  $permission_set = $form_state['item'];

  // ctools tries to be smart by adding its own "machinename" field. We remove
  // it and add our own.
  unset($form['info']['machinename']);

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Permission Set Name'),
    '#description' => t('The human readable name of this permission set.'),
    '#default_value' => $permission_set->name,
    '#required' => true,
  );
  $form['machinename'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine Name'),
    '#maxlength' => 128,
    '#default_value' => $permission_set->machinename,
    '#description' => t('Only use letters, numbers and underscores. Examples: super-admin, basic-content-authoring'),
    '#machine_name' => array(
      'exists' => 'permission_set_name_exists',
      'source' => array('name'),
    ),
  );

  module_load_include('inc', 'user', 'user.admin');
  $form['permissions'] = user_admin_permissions(array(), array(), PERMISSION_SETS_RID);
  $form['permissions']['role_names'][PERMISSION_SETS_RID]['#markup'] = t('Granted');
  $form['permissions']['#theme'] = 'user_admin_permissions';
  $form['permissions']['checkboxes'][PERMISSION_SETS_RID]['#default_value'] = is_array($permission_set->permissions) ? $permission_set->permissions : array();

  // Remove the "save permissions" button.
  unset($form['permissions']['actions']);
}

/**
 * Submit function for the permission_set add/edit form.
 */
function permission_set_ctools_export_ui_form_submit($form, &$form_state) {
  $permissions = array_values(array_filter($form_state['values'][PERMISSION_SETS_RID]));
  $form_state['values']['permissions'] = !empty($permissions) ? array_combine($permissions, $permissions) : array();
}

/**
 * Check if the given machinename is unique in the dfp_tags table.
 *
 * @param string $machinename
 *   The machinename to check for uniqueness.
 *
 * @return bool or int
 */
function permission_set_name_exists($machinename) {
  $select = db_select('permission_sets', 'ps');
  $select->addExpression('COUNT(*)');
  $select->condition('ps.machinename', $machinename);

  return $select->execute()->fetchField();
}

