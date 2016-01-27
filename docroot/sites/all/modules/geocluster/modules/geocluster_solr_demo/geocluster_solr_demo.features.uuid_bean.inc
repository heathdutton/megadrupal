<?php
/**
 * @file
 * geocluster_solr_demo.features.uuid_bean.inc
 */

/**
 * Implements hook_uuid_features_default_beans().
 */
function geocluster_solr_demo_uuid_features_default_beans() {
  $beans = array();

  $beans[] = array(
    'label' => 'Geocluster test solr json map',
    'description' => NULL,
    'title' => '',
    'type' => 'leaflet_geojson_bean',
    'data' => array(
      'view_mode' => 'default',
      'settings' => array(
        'source' => 'geocluster_test_solr_json',
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
    'delta' => 'geocluster-test-solr-json-map',
    'view_mode' => 'default',
    'created' => '1361492666',
    'log' => NULL,
    'uid' => '0',
    'default_revision' => '1',
    'revisions' => array(),
    'uuid' => '82422f96-b1d3-43f8-b536-43e36d9db72c',
    'settings' => array(
      'source' => 'geocluster_test_solr_json',
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
