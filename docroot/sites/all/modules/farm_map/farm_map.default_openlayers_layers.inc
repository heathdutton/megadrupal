<?php
/**
 * @file
 * farm_map.default_openlayers_layers.inc
 */

/**
 * Implements hook_default_openlayers_layers().
 */
function farm_map_default_openlayers_layers() {
  $export = array();

  $ol_layer = new stdClass();
  $ol_layer->disabled = FALSE; /* Edit this to true to make a default ol_layer disabled initially */
  $ol_layer->api_version = 1;
  $ol_layer->machine_name = 'farm_map_layer_google_hybrid';
  $ol_layer->name = 'Google Hybrid';
  $ol_layer->description = '';
  $ol_layer->factory_service = 'openlayers.Layer:Tile';
  $ol_layer->options = array(
    'source' => 'farm_map_source_google_hybrid',
    'visible' => 1,
    'opacity' => 1,
    /**
     * @todo
     * https://www.drupal.org/node/2543130
     */
    'type' => 'base',
  );
  $export['farm_map_layer_google_hybrid'] = $ol_layer;

  $ol_layer = new stdClass();
  $ol_layer->disabled = FALSE; /* Edit this to true to make a default ol_layer disabled initially */
  $ol_layer->api_version = 1;
  $ol_layer->machine_name = 'farm_map_layer_google_satellite';
  $ol_layer->name = 'Google Satellite';
  $ol_layer->description = '';
  $ol_layer->factory_service = 'openlayers.Layer:Tile';
  $ol_layer->options = array(
    'source' => 'farm_map_source_google_satellite',
    'visible' => 1,
    'opacity' => 1,
    /**
     * @todo
     * https://www.drupal.org/node/2543130
     */
    'type' => 'base',
  );
  $export['farm_map_layer_google_satellite'] = $ol_layer;

  $ol_layer = new stdClass();
  $ol_layer->disabled = FALSE; /* Edit this to true to make a default ol_layer disabled initially */
  $ol_layer->api_version = 1;
  $ol_layer->machine_name = 'farm_map_layer_google_terrain';
  $ol_layer->name = 'Google Terrain';
  $ol_layer->description = '';
  $ol_layer->factory_service = 'openlayers.Layer:Tile';
  $ol_layer->options = array(
    'source' => 'farm_map_source_google_terrain',
    'visible' => 1,
    'opacity' => 1,
    /**
     * @todo
     * https://www.drupal.org/node/2543130
     */
    'type' => 'base',
  );
  $export['farm_map_layer_google_terrain'] = $ol_layer;

  $ol_layer = new stdClass();
  $ol_layer->disabled = FALSE; /* Edit this to true to make a default ol_layer disabled initially */
  $ol_layer->api_version = 1;
  $ol_layer->machine_name = 'farm_map_layer_group';
  $ol_layer->name = 'Farm Map Layer Group';
  $ol_layer->description = '';
  $ol_layer->factory_service = 'openlayers.Layer:Group';
  $ol_layer->options = array(
    'source' => '',
    'visible' => 1,
    'grouptitle' => 'Base layers',
    'grouplayers' => array(
      0 => 'farm_map_layer_google_hybrid',
      /**
       * @todo
       * https://www.drupal.org/node/2543188
       */
//      1 => 'farm_map_layer_google_satellite',
//      2 => 'farm_map_layer_google_terrain',
    ),
  );
  $export['farm_map_layer_group'] = $ol_layer;

  return $export;
}