<?php
/**
 * @file
 * one_import.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function one_import_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'one_site_content_import';
  $feeds_importer->config = array(
    'name' => 'One Site Content Import',
    'description' => 'Import site content',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => 1,
        'use_pubsubhubbub' => 0,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsSyndicationParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'one_site_content',
        'update_existing' => '2',
        'expire' => '-1',
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'guid',
            'target' => 'guid',
            'unique' => 1,
          ),
          2 => array(
            'source' => 'description',
            'target' => 'body',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'timestamp',
            'target' => 'created',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'url',
            'target' => 'field_one_site_link:url',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'Blank source 1',
            'target' => 'field_one_site_link:title',
            'unique' => FALSE,
          ),
        ),
        'input_format' => 'full_html',
        'author' => '1',
        'authorize' => 1,
        'skip_hash_check' => 0,
        'inherit' => 1,
        'inherit_fields' => array(
          'language' => 'language',
          'status' => 'status',
          'promote' => 'promote',
          'sticky' => 'sticky',
          'uid' => 'uid',
          'comment' => 'comment',
          'field_one_site_grouped_by' => 'field_one_site_grouped_by',
          'field_one_site_keywords' => 'field_one_site_keywords',
        ),
      ),
    ),
    'content_type' => 'one_site_content',
    'update' => 0,
    'import_period' => '21600',
    'expire_period' => 3600,
    'import_on_create' => 0,
    'process_in_background' => 1,
  );
  $export['one_site_content_import'] = $feeds_importer;

  return $export;
}
