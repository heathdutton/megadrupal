<?php
/**
 * @file
 * drushweb.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function drushweb_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'oauth register consumers in drushweb'.
  $permissions['oauth register consumers in drushweb'] = array(
    'name' => 'oauth register consumers in drushweb',
    'roles' => array(
      'Drush Webservice OAuth user' => 'Drush Webservice OAuth user',
    ),
    'module' => 'oauth_common',
  );

  return $permissions;
}
