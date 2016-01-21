<?php
/**
 * @file
 * drustack_performance.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function drustack_performance_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access files on CDN when in testing mode'.
  $permissions['access files on CDN when in testing mode'] = array(
    'name' => 'access files on CDN when in testing mode',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'cdn',
  );

  // Exported permission: 'access memcache statistics'.
  $permissions['access memcache statistics'] = array(
    'name' => 'access memcache statistics',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'memcache_admin',
  );

  // Exported permission: 'access per-page statistics'.
  $permissions['access per-page statistics'] = array(
    'name' => 'access per-page statistics',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'cdn',
  );

  // Exported permission: 'access slab cachedump'.
  $permissions['access slab cachedump'] = array(
    'name' => 'access slab cachedump',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'memcache_admin',
  );

  // Exported permission: 'administer CDN configuration'.
  $permissions['administer CDN configuration'] = array(
    'name' => 'administer CDN configuration',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'cdn',
  );

  // Exported permission: 'administer varnish'.
  $permissions['administer varnish'] = array(
    'name' => 'administer varnish',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'varnish',
  );

  // Exported permission: 'touch files'.
  $permissions['touch files'] = array(
    'name' => 'touch files',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'cdn',
  );

  return $permissions;
}
