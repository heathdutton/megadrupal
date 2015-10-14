<?php
/**
 * @file
 * instagram_feeds.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function instagram_feeds_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'instagram_feed';
  $feeds_importer->config = array(
    'name' => 'Instagram Feed',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'InstagramFeedsPluginsPager',
      'config' => array(
        'auto_detect_feeds' => 0,
        'use_pubsubhubbub' => 0,
        'designated_hub' => '',
        'max_pages' => '0',
        'number_of_images' => '20',
        'request_timeout' => NULL,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsJSONPathParser',
      'config' => array(
        'context' => '$.data[*]',
        'sources' => array(
          'jsonpath_parser:0' => 'id',
          'jsonpath_parser:1' => 'id',
          'jsonpath_parser:2' => 'id',
          'jsonpath_parser:3' => 'caption.text',
          'jsonpath_parser:4' => 'created_time',
          'jsonpath_parser:5' => 'link',
          'jsonpath_parser:6' => 'user.id',
          'jsonpath_parser:7' => 'user.username',
          'jsonpath_parser:9' => 'tags.0',
          'jsonpath_parser:10' => 'likes.count',
          'jsonpath_parser:11' => 'comments.count',
          'jsonpath_parser:12' => 'type',
          'jsonpath_parser:13' => 'images.standard_resolution.url',
          'jsonpath_parser:14' => 'images.thumbnail.url',
        ),
        'debug' => array(
          'options' => array(
            'context' => 0,
            'jsonpath_parser:0' => 0,
            'jsonpath_parser:1' => 0,
            'jsonpath_parser:2' => 0,
            'jsonpath_parser:3' => 0,
            'jsonpath_parser:4' => 0,
            'jsonpath_parser:5' => 0,
            'jsonpath_parser:6' => 0,
            'jsonpath_parser:7' => 0,
            'jsonpath_parser:9' => 0,
            'jsonpath_parser:10' => 0,
            'jsonpath_parser:11' => 0,
            'jsonpath_parser:12' => 0,
            'jsonpath_parser:13' => 0,
            'jsonpath_parser:14' => 0,
          ),
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'InstagramFeedsPluginsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => '1',
        'mappings' => array(
          0 => array(
            'source' => 'jsonpath_parser:0',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'jsonpath_parser:1',
            'target' => 'title',
            'unique' => 0,
          ),
          2 => array(
            'source' => 'jsonpath_parser:2',
            'target' => 'field_instf_id',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'jsonpath_parser:3',
            'target' => 'body',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'jsonpath_parser:4',
            'target' => 'field_instf_created:start',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'jsonpath_parser:5',
            'target' => 'field_instf_link_url:url',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'jsonpath_parser:6',
            'target' => 'field_instf_uid',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'jsonpath_parser:7',
            'target' => 'field_instf_user',
            'term_search' => '0',
            'autocreate' => 1,
          ),
          8 => array(
            'source' => 'jsonpath_parser:9',
            'target' => 'field_instf_hash_tags',
            'term_search' => '0',
            'autocreate' => 1,
          ),
          9 => array(
            'source' => 'jsonpath_parser:10',
            'target' => 'field_instf_like_count',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'jsonpath_parser:11',
            'target' => 'field_instf_comments_count',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'jsonpath_parser:12',
            'target' => 'field_instf_marker',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'jsonpath_parser:13',
            'target' => 'field_instf_image_url:url',
            'unique' => FALSE,
          ),
          13 => array(
            'source' => 'jsonpath_parser:14',
            'target' => 'field_instf_thumb_url:url',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '1',
        'input_format' => 'plain_text',
        'authorize' => 1,
        'skip_hash_check' => 0,
        'bundle' => 'instagram_media_item',
      ),
    ),
    'content_type' => 'instagram_url',
    'update' => 0,
    'import_period' => '900',
    'expire_period' => 3600,
    'import_on_create' => 0,
    'process_in_background' => 0,
  );
  $export['instagram_feed'] = $feeds_importer;

  return $export;
}
