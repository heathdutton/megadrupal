<?php
/**
 * @file
 * one_import.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function one_import_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer feeds.
  $permissions['administer feeds'] = array(
    'name' => 'administer feeds',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'feeds',
  );

  // Exported permission: administer feeds_tamper.
  $permissions['administer feeds_tamper'] = array(
    'name' => 'administer feeds_tamper',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'feeds_tamper',
  );

  // Exported permission: clear one_site_content_import feeds.
  $permissions['clear one_site_content_import feeds'] = array(
    'name' => 'clear one_site_content_import feeds',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'feeds',
  );

  // Exported permission: import one_site_content_import feeds.
  $permissions['import one_site_content_import feeds'] = array(
    'name' => 'import one_site_content_import feeds',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'feeds',
  );

  // Exported permission: tamper one_site_content_import.
  $permissions['tamper one_site_content_import'] = array(
    'name' => 'tamper one_site_content_import',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'feeds_tamper',
  );

  // Exported permission: unlock one_site_content_import feeds.
  $permissions['unlock one_site_content_import feeds'] = array(
    'name' => 'unlock one_site_content_import feeds',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'feeds',
  );

  return $permissions;
}
