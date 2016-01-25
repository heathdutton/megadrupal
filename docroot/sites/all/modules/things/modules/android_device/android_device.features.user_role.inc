<?php
/**
 * @file
 * android_device.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function android_device_user_default_roles() {
  $roles = array();

  // Exported role: mobile4social.
  $roles['mobile4social'] = array(
    'name' => 'mobile4social',
    'weight' => '2',
  );

  return $roles;
}
