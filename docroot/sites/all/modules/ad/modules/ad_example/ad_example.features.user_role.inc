<?php
/**
 * @file
 * ad_example.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function ad_example_user_default_roles() {
  $roles = array();

  // Exported role: adsadmin.
  $roles['adsadmin'] = array(
    'name' => 'adsadmin',
    'weight' => 3,
  );

  // Exported role: advertiser.
  $roles['advertiser'] = array(
    'name' => 'advertiser',
    'weight' => 2,
  );

  return $roles;
}
