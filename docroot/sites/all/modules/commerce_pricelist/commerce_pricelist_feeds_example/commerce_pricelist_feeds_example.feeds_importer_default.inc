<?php
/**
 * @file
 * commerce_pricelist_feeds_example.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function commerce_pricelist_feeds_example_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'commerce_pricelist_csv_example';
  $feeds_importer->config = array(
    'name' => 'Commerce Pricelist csv example',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
        'directory' => 'public://feeds',
        'allowed_schemes' => array(
          0 => 'public',
        ),
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsCommercePricelistItemProcessor',
      'config' => array(
        'is_active' => '1',
        'is_attached' => '0',
        'list_uuid' => NULL,
        'convert_price' => '1',
        'convert_dates' => '1',
        'mappings' => array(
          0 => array(
            'source' => 'sku',
            'target' => 'sku',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'price_amount',
            'target' => 'price_amount',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'currency_code',
            'target' => 'currency_code',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'quantity',
            'target' => 'quantity',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'valid_from',
            'target' => 'valid_from',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'valid_to',
            'target' => 'valid_to',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'pricelist_title',
            'target' => 'pricelist_title',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'update_non_existent' => 'skip',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'commerce_pricelist_item',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => 1800,
    'expire_period' => 3600,
    'import_on_create' => TRUE,
    'process_in_background' => FALSE,
  );

  $export['commerce_pricelist_csv_example'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'commerce_pricelist_csv_attached_example';
  $feeds_importer->config = array(
    'name' => 'Commerce Pricelist csv attached example',
    'description' => 'A pricelist importer attached to the pricelist form',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
        'directory' => 'public://feeds',
        'allowed_schemes' => array(
          0 => 'public',
        ),
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsCommercePricelistItemProcessor',
      'config' => array(
        'is_active' => '1',
        'is_attached' => '1',
        'list_uuid' => NULL,
        'convert_price' => '1',
        'convert_dates' => '1',
        'mappings' => array(
          0 => array(
            'source' => 'sku',
            'target' => 'sku',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'price_amount',
            'target' => 'price_amount',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'currency_code',
            'target' => 'currency_code',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'quantity',
            'target' => 'quantity',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'valid_from',
            'target' => 'valid_from',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'valid_to',
            'target' => 'valid_to',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'update_non_existent' => 'skip',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'commerce_pricelist_item',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => 1800,
    'expire_period' => 3600,
    'import_on_create' => TRUE,
    'process_in_background' => FALSE,
  );

  $export['commerce_pricelist_csv_attached_example'] = $feeds_importer;

  return $export;
}
