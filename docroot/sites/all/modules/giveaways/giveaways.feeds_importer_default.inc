<?php
/**
 * @file
 * giveaways.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function giveaways_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'key_giveaway_importer';
  $feeds_importer->config = array(
    'name' => 'Key Giveaway Importer',
    'description' => 'This will import a csv of keys into a custom keys table.',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv',
        'direct' => 0,
        'directory' => 'public://feeds',
        'allowed_schemes' => array(
          'public' => 'public',
        ),
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 1,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsEntityProcessorData_giveaway_keys',
      'config' => array(
        'mappings' => array(
          0 => array(
            'source' => '0',
            'target' => 'game_key',
            'unique' => 0,
          ),
          1 => array(
            'source' => 'feed_nid',
            'target' => 'gid',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'url',
            'target' => 'url',
            'unique' => 1,
          ),
        ),
        'update_existing' => '2',
        'update_non_existent' => 'skip',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'data_giveaway_keys',
        'values' => array(
          'gkid' => '0',
          'gid' => '',
          'game_key' => '',
          'uid' => '',
          'ip_address' => '',
          'claimed_on' => '',
        ),
        'authorize' => TRUE,
      ),
    ),
    'content_type' => 'giveaway',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['key_giveaway_importer'] = $feeds_importer;

  return $export;
}
