<?php
/**
 * @file
 * geocluster_demo.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function geocluster_demo_user_default_permissions() {
  $permissions = array();

  // Exported permission: create any leaflet_geojson_bean bean.
  $permissions['create any leaflet_geojson_bean bean'] = array(
    'name' => 'create any leaflet_geojson_bean bean',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: delete any leaflet_geojson_bean bean.
  $permissions['delete any leaflet_geojson_bean bean'] = array(
    'name' => 'delete any leaflet_geojson_bean bean',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: edit any leaflet_geojson_bean bean.
  $permissions['edit any leaflet_geojson_bean bean'] = array(
    'name' => 'edit any leaflet_geojson_bean bean',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: view any leaflet_geojson_bean bean.
  $permissions['view any leaflet_geojson_bean bean'] = array(
    'name' => 'view any leaflet_geojson_bean bean',
    'roles' => array(
      0 => 'anonymous user',
      1 => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: view bean page.
  $permissions['view bean page'] = array(
    'name' => 'view bean page',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
    ),
    'module' => 'bean',
  );

  return $permissions;
}
