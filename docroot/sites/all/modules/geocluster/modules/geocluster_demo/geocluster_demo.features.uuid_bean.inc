<?php
/**
 * @file
 * geocluster_demo.features.uuid_bean.inc
 */

/**
 * Implements hook_uuid_features_default_beans().
 */
function geocluster_demo_uuid_features_default_beans() {
  $beans = array();

  $beans[] = array(
    'label' => 'Geocluster test default json map',
    'description' => NULL,
    'title' => '',
    'type' => 'leaflet_geojson_bean',
    'data' => array(
      'view_mode' => 'default',
      'settings' => array(
        'source' => 'geocluster_test_default_json',
        'map' => 'mapbox streets',
        'height' => '400',
        'override_map_settings' => 1,
        'map_settings' => array(
          'zoom' => '2',
          'center' => array(
            'lon' => '14.56667',
            'lat' => '48.03333',
          ),
        ),
      ),
    ),
    'delta' => 'geocluster-test-default-json-map',
    'view_mode' => 'default',
    'created' => '1361706353',
    'log' => '',
    'uid' => '1',
    'default_revision' => '1',
    'revisions' => array(),
    'uuid' => '38755ac5-2d5d-4b22-9c73-7ae3edf0c338',
    'settings' => array(
      'source' => 'geocluster_test_default_json',
      'map' => 'mapbox streets',
      'height' => '400',
      'override_map_settings' => 1,
      'map_settings' => array(
        'zoom' => '2',
        'center' => array(
          'lon' => '14.56667',
          'lat' => '48.03333',
        ),
      ),
    ),
  );
  $beans[] = array(
    'label' => 'Geocluster test mysql json map',
    'description' => NULL,
    'title' => '',
    'type' => 'leaflet_geojson_bean',
    'data' => array(
      'view_mode' => 'default',
      'settings' => array(
        'source' => 'geocluster_test_mysql_json',
        'map' => 'mapbox streets',
        'height' => '400',
        'override_map_settings' => 1,
        'map_settings' => array(
          'zoom' => '2',
          'center' => array(
            'lon' => '14.56667',
            'lat' => '48.03333',
          ),
        ),
      ),
    ),
    'delta' => 'geocluster-test-mysql-json-map',
    'view_mode' => 'default',
    'created' => '1361702942',
    'log' => NULL,
    'uid' => '0',
    'default_revision' => '1',
    'revisions' => array(),
    'uuid' => '5fd0c588-70ed-fc94-61b8-188e05450406',
    'settings' => array(
      'source' => 'geocluster_test_mysql_json',
      'map' => 'mapbox streets',
      'height' => '400',
      'override_map_settings' => 1,
      'map_settings' => array(
        'zoom' => '2',
        'center' => array(
          'lon' => '14.56667',
          'lat' => '48.03333',
        ),
      ),
    ),
  );
  $beans[] = array(
    'label' => 'Geocluster test php json map',
    'description' => NULL,
    'title' => '',
    'type' => 'leaflet_geojson_bean',
    'data' => array(
      'view_mode' => 'default',
      'settings' => array(
        'source' => 'geocluster_test_php_json',
        'map' => 'mapbox streets',
        'height' => '400',
        'override_map_settings' => 1,
        'map_settings' => array(
          'zoom' => '2',
          'center' => array(
            'lon' => '14.56667',
            'lat' => '48.03333',
          ),
        ),
      ),
    ),
    'delta' => 'geocluster-test-php-json-map',
    'view_mode' => 'default',
    'created' => '1361702942',
    'log' => NULL,
    'uid' => '0',
    'default_revision' => '1',
    'revisions' => array(),
    'uuid' => 'e7767fe6-3b15-4b9c-b24e-b9fdf9ada963',
    'settings' => array(
      'source' => 'geocluster_test_php_json',
      'map' => 'mapbox streets',
      'height' => '400',
      'override_map_settings' => 1,
      'map_settings' => array(
        'zoom' => '2',
        'center' => array(
          'lon' => '14.56667',
          'lat' => '48.03333',
        ),
      ),
    ),
  );
  return $beans;
}
