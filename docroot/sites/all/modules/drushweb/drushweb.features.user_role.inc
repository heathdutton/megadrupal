<?php
/**
 * @file
 * drushweb.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function drushweb_user_default_roles() {
  $roles = array();

  // Exported role: Drush Webservice OAuth user.
  $roles['Drush Webservice OAuth user'] = array(
    'name' => 'Drush Webservice OAuth user',
    'weight' => 5,
  );

  return $roles;
}
