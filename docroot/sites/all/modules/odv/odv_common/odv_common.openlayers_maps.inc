<?php
/**
 * @file
 * odv_common.openlayers_maps.inc
 */

/**
 * Implements hook_openlayers_maps().
 */
function odv_common_openlayers_maps() {
  $export = array();

  $openlayers_maps = new stdClass;
  $openlayers_maps->disabled = FALSE; /* Edit this to true to make a default openlayers_maps disabled initially */
  $openlayers_maps->api_version = 1;
  $openlayers_maps->name = 'odv_field_formatter_map';
  $openlayers_maps->title = 'ODV Geofield Formatter Map';
  $openlayers_maps->description = 'A Map Used for Geofield Output';
  $openlayers_maps->data = array(
    'width' => '600px',
    'height' => '400px',
    'image_path' => '',
    'css_path' => '',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '0,0',
        'zoom' => '2',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_layerswitcher' => array(
        'ascending' => 0,
      ),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 0,
        'documentDrag' => 0,
      ),
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_zoomtolayer' => array(
        'zoomtolayer' => 'geofield_formatter',
        'point_zoom_level' => '13',
      ),
    ),
    'default_layer' => 'odv_kartenwerkstadt_austria_wien_ogd_osm',
    'layers' => array(
      'odv_kartenwerkstadt_austria_wien_ogd_osm' => 'odv_kartenwerkstadt_austria_wien_ogd_osm',
      'odv_kartenwerkstadt_austria_wien_osm_sotmeu' => 'odv_kartenwerkstadt_austria_wien_osm_sotmeu',
      'odv_mapproxy_ogd_wien_ortho' => 'odv_mapproxy_ogd_wien_ortho',
      'odv_mapproxy_ogd_wien_stadtplan' => 'odv_mapproxy_ogd_wien_stadtplan',
      'osm_mapnik' => 'osm_mapnik',
      'geofield_formatter' => 'geofield_formatter',
    ),
    'layer_weight' => array(
      'geofield_formatter' => '0',
      'odl_map_openlayers_1' => '0',
      'odv_map_openlayers_1' => '0',
    ),
    'layer_styles' => array(
      'odv_map_openlayers_1' => 'odv_map_default_style',
      'odl_map_openlayers_1' => '0',
      'geofield_formatter' => 'odv_map_default_style',
    ),
    'layer_activated' => array(
      'geofield_formatter' => 'geofield_formatter',
      'odv_map_openlayers_1' => 0,
      'odl_map_openlayers_1' => 0,
    ),
    'layer_switcher' => array(
      'geofield_formatter' => 0,
      'odv_map_openlayers_1' => 0,
      'odl_map_openlayers_1' => 0,
    ),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'odv_map_default_style',
      'select' => 'odv_map_default_style',
      'temporary' => 'odv_map_default_style',
    ),
  );
  $export['odv_field_formatter_map'] = $openlayers_maps;

  $openlayers_maps = new stdClass;
  $openlayers_maps->disabled = FALSE; /* Edit this to true to make a default openlayers_maps disabled initially */
  $openlayers_maps->api_version = 1;
  $openlayers_maps->name = 'odv_field_formatter_map_teaser';
  $openlayers_maps->title = 'ODV Geofield Formatter Map Teaser';
  $openlayers_maps->description = 'A Map Used for Geofield Output';
  $openlayers_maps->data = array(
    'width' => '200px',
    'height' => '150px',
    'image_path' => '',
    'css_path' => '',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '0,0',
        'zoom' => '2',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_zoomtolayer' => array(
        'zoomtolayer' => 'geofield_formatter',
        'point_zoom_level' => '15',
      ),
    ),
    'default_layer' => 'odv_kartenwerkstadt_austria_wien_ogd_osm',
    'layers' => array(
      'odv_kartenwerkstadt_austria_wien_ogd_osm' => 'odv_kartenwerkstadt_austria_wien_ogd_osm',
      'odv_kartenwerkstadt_austria_wien_osm_sotmeu' => 'odv_kartenwerkstadt_austria_wien_osm_sotmeu',
      'odv_mapproxy_ogd_wien_ortho' => 'odv_mapproxy_ogd_wien_ortho',
      'odv_mapproxy_ogd_wien_stadtplan' => 'odv_mapproxy_ogd_wien_stadtplan',
      'osm_mapnik' => 'osm_mapnik',
      'geofield_formatter' => 'geofield_formatter',
    ),
    'layer_weight' => array(
      'geofield_formatter' => '0',
      'odl_map_openlayers_1' => '0',
      'odv_map_openlayers_1' => '0',
    ),
    'layer_styles' => array(
      'odv_map_openlayers_1' => 'odv_map_default_style',
      'odl_map_openlayers_1' => '0',
      'geofield_formatter' => 'odv_map_default_style',
    ),
    'layer_activated' => array(
      'geofield_formatter' => 'geofield_formatter',
      'odv_map_openlayers_1' => 0,
      'odl_map_openlayers_1' => 0,
    ),
    'layer_switcher' => array(
      'geofield_formatter' => 0,
      'odv_map_openlayers_1' => 0,
      'odl_map_openlayers_1' => 0,
    ),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'odv_map_default_style',
      'select' => 'odv_map_default_style',
      'temporary' => 'odv_map_default_style',
    ),
  );
  $export['odv_field_formatter_map_teaser'] = $openlayers_maps;

  $openlayers_maps = new stdClass;
  $openlayers_maps->disabled = FALSE; /* Edit this to true to make a default openlayers_maps disabled initially */
  $openlayers_maps->api_version = 1;
  $openlayers_maps->name = 'odv_map';
  $openlayers_maps->title = 'ODV Map';
  $openlayers_maps->description = 'Map für Open Data Vienna';
  $openlayers_maps->data = array(
    'width' => 'auto',
    'height' => '400px',
    'image_path' => '',
    'css_path' => '',
    'proxy_host' => '',
    'hide_empty_map' => 0,
    'center' => array(
      'initial' => array(
        'centerpoint' => '16.37271881038, 48.207972960953',
        'zoom' => '13',
      ),
      'restrict' => array(
        'restrictextent' => 0,
        'restrictedExtent' => '',
      ),
    ),
    'behaviors' => array(
      'openlayers_behavior_popup' => array(
        'layers' => array(
          'odv_map_openlayers_1' => 'odv_map_openlayers_1',
        ),
      ),
      'openlayers_behavior_keyboarddefaults' => array(),
      'openlayers_behavior_layerswitcher' => array(
        'ascending' => 0,
      ),
      'openlayers_behavior_navigation' => array(
        'zoomWheelEnabled' => 0,
        'documentDrag' => 0,
      ),
      'openlayers_behavior_panzoombar' => array(),
      'openlayers_behavior_zoomtolayer' => array(
        'zoomtolayer' => 'odv_map_openlayers_1',
        'point_zoom_level' => '11',
      ),
    ),
    'default_layer' => 'odv_kartenwerkstadt_austria_wien_ogd_osm',
    'layers' => array(
      'odv_kartenwerkstadt_austria_wien_ogd_osm' => 'odv_kartenwerkstadt_austria_wien_ogd_osm',
      'odv_kartenwerkstadt_austria_wien_osm_sotmeu' => 'odv_kartenwerkstadt_austria_wien_osm_sotmeu',
      'odv_mapproxy_ogd_wien_ortho' => 'odv_mapproxy_ogd_wien_ortho',
      'odv_mapproxy_ogd_wien_stadtplan' => 'odv_mapproxy_ogd_wien_stadtplan',
      'osm_tah' => 'osm_tah',
      'odv_map_openlayers_1' => 'odv_map_openlayers_1',
    ),
    'layer_weight' => array(
      'odv_map_openlayers_1' => '0',
      'odl_map_openlayers_1' => '0',
      'geofield_formatter' => '0',
    ),
    'layer_styles' => array(
      'geofield_formatter' => '0',
      'odl_map_openlayers_1' => '0',
      'odv_map_openlayers_1' => 'odv_map_default_style',
    ),
    'layer_activated' => array(
      'odv_map_openlayers_1' => 'odv_map_openlayers_1',
      'geofield_formatter' => 0,
      'odl_map_openlayers_1' => 0,
    ),
    'layer_switcher' => array(
      'geofield_formatter' => 0,
      'odv_map_openlayers_1' => 0,
      'odl_map_openlayers_1' => 0,
    ),
    'projection' => '900913',
    'displayProjection' => '4326',
    'styles' => array(
      'default' => 'odv_map_default_style',
      'select' => 'odv_map_default_style',
      'temporary' => 'odv_map_default_style',
    ),
  );
  $export['odv_map'] = $openlayers_maps;

  return $export;
}
