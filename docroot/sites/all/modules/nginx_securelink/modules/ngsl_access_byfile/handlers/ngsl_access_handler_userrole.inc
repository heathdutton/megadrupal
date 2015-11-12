<?php

/**
 * @author "Ahmad Hesamzadeh" <mngafa@gmail.com>
 * @file Access handler which is based on user roles
 *
 * Provides ability to set access control according to user roles.
 * For example allow all users of a certain role to download files.
 */

/**
 * Access handler callback for handler "userrole"
 *
 * If the current user has the given role, return TRUE. Otherwise return FALSE.
 * In case of errors, it will also return FALSE.
 *
 * @param $role_name The role name to check
 *
 * @return bool
 */
function ngsl_access_byfile_handler_userrole($role_name) {
  // Get role object from given role name
  $role = user_role_load_by_name($role_name);
  if (!$role) {
    watchdog('nginx_securelink', 'ngsl_access_handler_userrole: Role @role does not exist.', array('@role' => $role_name), WATCHDOG_ERROR);

    return FALSE;
  }

  // If current user has the given role, return TRUE.
  return isset($GLOBALS['user']->roles[$role->rid]);
}
