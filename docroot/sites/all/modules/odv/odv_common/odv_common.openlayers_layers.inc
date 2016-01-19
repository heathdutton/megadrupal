<?php
/**
 * @file
 * odv_common.openlayers_layers.inc
 */

/**
 * Implements hook_openlayers_layers().
 */
function odv_common_openlayers_layers() {
  $export = array();

  $openlayers_layers = new stdClass;
  $openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
  $openlayers_layers->api_version = 1;
  $openlayers_layers->name = 'odv_kartenwerkstadt_austria_wien_ogd_osm';
  $openlayers_layers->title = 'Kartenwerkstadt Austria Wien OGD OSM';
  $openlayers_layers->description = '';
  $openlayers_layers->data = array(
    'base_url' => 'http://www.kartenwerkstatt.at/',
    'layername' => 'austria-wien-ogd-osm-01',
    'baselayer' => 1,
    'type' => 'png',
    'resolutions' => array(
      0 => 76.4370282715,
      1 => 38.2185141357,
      2 => 19.1092570679,
      3 => 9.55462853394,
      4 => 4.77731426697,
      5 => 2.38865713348,
      6 => 1.19432856674,
      7 => 0.597164283371,
    ),
    'wrapDateLine' => 0,
    'layer_type' => 'openlayers_layer_type_tms',
  );
  $export['odv_kartenwerkstadt_austria_wien_ogd_osm'] = $openlayers_layers;

  $openlayers_layers = new stdClass;
  $openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
  $openlayers_layers->api_version = 1;
  $openlayers_layers->name = 'odv_kartenwerkstadt_austria_wien_osm_sotmeu';
  $openlayers_layers->title = 'Kartenwerkstadt Austria Wien SotmEU';
  $openlayers_layers->description = '';
  $openlayers_layers->data = array(
    'base_url' => 'http://www.kartenwerkstatt.at/',
    'layername' => 'austria-wien-osm-sotmeu',
    'baselayer' => 1,
    'type' => 'png',
    'resolutions' => array(
      0 => 76.4370282715,
      1 => 38.2185141357,
      2 => 19.1092570679,
      3 => 9.55462853394,
      4 => 4.77731426697,
      5 => 2.38865713348,
      6 => 1.19432856674,
    ),
    'wrapDateLine' => 0,
    'layer_type' => 'openlayers_layer_type_tms',
  );
  $export['odv_kartenwerkstadt_austria_wien_osm_sotmeu'] = $openlayers_layers;

  $openlayers_layers = new stdClass;
  $openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
  $openlayers_layers->api_version = 1;
  $openlayers_layers->name = 'odv_mapproxy_ogd_wien_ortho';
  $openlayers_layers->title = 'MapProxy OGD_WIEN_ORTHO';
  $openlayers_layers->description = '';
  $openlayers_layers->data = array(
    'base_url' => 'http://tms.kartenwerkstatt.at/tiles/',
    'layername' => 'OGD_WIEN_ORTHO_EPSG3857',
    'baselayer' => 1,
    'type' => 'png',
    'resolutions' => array(
      0 => 76.4370282715,
      1 => 38.2185141357,
      2 => 19.1092570679,
      3 => 9.55462853394,
      4 => 4.77731426697,
      5 => 2.38865713348,
      6 => 1.19432856674,
      7 => 0.597164283371,
    ),
    'wrapDateLine' => 0,
    'layer_type' => 'openlayers_layer_type_tms',
  );
  $export['odv_mapproxy_ogd_wien_ortho'] = $openlayers_layers;

  $openlayers_layers = new stdClass;
  $openlayers_layers->disabled = FALSE; /* Edit this to true to make a default openlayers_layers disabled initially */
  $openlayers_layers->api_version = 1;
  $openlayers_layers->name = 'odv_mapproxy_ogd_wien_stadtplan';
  $openlayers_layers->title = 'MapProxy OGD_WIEN_STADTPLAN';
  $openlayers_layers->description = '';
  $openlayers_layers->data = array(
    'base_url' => 'http://tms.kartenwerkstatt.at/tiles/',
    'layername' => 'OGD_WIEN_STADTPLAN_EPSG3857',
    'baselayer' => 1,
    'type' => 'png',
    'resolutions' => array(
      0 => 76.4370282715,
      1 => 38.2185141357,
      2 => 19.1092570679,
      3 => 9.55462853394,
      4 => 4.77731426697,
      5 => 2.38865713348,
      6 => 1.19432856674,
      7 => 0.597164283371,
    ),
    'wrapDateLine' => 0,
    'layer_type' => 'openlayers_layer_type_tms',
  );
  $export['odv_mapproxy_ogd_wien_stadtplan'] = $openlayers_layers;

  return $export;
}
