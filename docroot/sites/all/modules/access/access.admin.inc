<?php
/**
 * @file
 * The access control kit module configuration pages.
 */

/**
 * Form constructor for the module configuration page.
 */
function access_admin_settings() {
  $form = array();

  // Select roles for use in realms.
  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  $form['access_realm_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles available within access realms'),
    '#options' => $roles,
    '#default_value' => variable_get('access_realm_roles', array()),
    '#description' => t('Users with the "administer access grants" permission will be able to assign these roles to users within one or more access realms.'),
  );

  return system_settings_form($form);
}
