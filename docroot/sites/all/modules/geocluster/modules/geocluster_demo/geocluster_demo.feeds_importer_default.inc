<?php
/**
 * @file
 * geocluster_demo.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function geocluster_demo_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'geonames_csv';
  $feeds_importer->config = array(
    'name' => 'geonames_csv',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => 'TAB',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'article',
        'expire' => -1,
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'GeoNameId',
            'target' => 'guid',
            'unique' => 0,
          ),
          1 => array(
            'source' => 'Name',
            'target' => 'title',
            'unique' => 0,
          ),
          2 => array(
            'source' => 'Latitude',
            'target' => 'field_place:lat',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Longitude',
            'target' => 'field_place:lon',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => 0,
        'input_format' => NULL,
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['geonames_csv'] = $feeds_importer;

  return $export;
}
