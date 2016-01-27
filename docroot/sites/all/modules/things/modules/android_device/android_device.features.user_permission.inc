<?php
/**
 * @file
 * android_device.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function android_device_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer things_ids.
  $permissions['administer things_ids'] = array(
    'name' => 'administer things_ids',
    'roles' => array(),
    'module' => 'things_ids',
  );

  // Exported permission: create things services.
  $permissions['create things services'] = array(
    'name' => 'create things services',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'things_services',
  );

  // Exported permission: delete things services.
  $permissions['delete things services'] = array(
    'name' => 'delete things services',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'things_services',
  );

  // Exported permission: edit any android_device things.
  $permissions['edit any android_device things'] = array(
    'name' => 'edit any android_device things',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'things',
  );

  // Exported permission: edit any field_device_id things_ids.
  $permissions['edit any field_device_id things_ids'] = array(
    'name' => 'edit any field_device_id things_ids',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'things_ids',
  );

  // Exported permission: edit any field_gcm_id things_ids.
  $permissions['edit any field_gcm_id things_ids'] = array(
    'name' => 'edit any field_gcm_id things_ids',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'things_ids',
  );

  // Exported permission: get a system variable.
  $permissions['get a system variable'] = array(
    'name' => 'get a system variable',
    'roles' => array(
      0 => 'anonymous user',
      1 => 'mobile4social',
    ),
    'module' => 'services',
  );

  // Exported permission: read privatemsg.
  $permissions['read privatemsg'] = array(
    'name' => 'read privatemsg',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: retrieve things services.
  $permissions['retrieve things services'] = array(
    'name' => 'retrieve things services',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'things_services',
  );

  // Exported permission: update things services.
  $permissions['update things services'] = array(
    'name' => 'update things services',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'things_services',
  );

  // Exported permission: use tokens in privatemsg.
  $permissions['use tokens in privatemsg'] = array(
    'name' => 'use tokens in privatemsg',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: view any android_device things.
  $permissions['view any android_device things'] = array(
    'name' => 'view any android_device things',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'things',
  );

  // Exported permission: view any field_device_id things_ids.
  $permissions['view any field_device_id things_ids'] = array(
    'name' => 'view any field_device_id things_ids',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'things_ids',
  );

  // Exported permission: view any field_gcm_id things_ids.
  $permissions['view any field_gcm_id things_ids'] = array(
    'name' => 'view any field_gcm_id things_ids',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'things_ids',
  );

  // Exported permission: write privatemsg.
  $permissions['write privatemsg'] = array(
    'name' => 'write privatemsg',
    'roles' => array(
      0 => 'mobile4social',
    ),
    'module' => 'privatemsg',
  );

  return $permissions;
}
