<?php
/**
 * @file
 * slc_base.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function slc_base_user_default_roles() {
  $roles = array();

  // Exported role: SLC Student.
  $roles['SLC Student'] = array(
    'name' => 'SLC Student',
    'weight' => '3',
  );

  return $roles;
}
