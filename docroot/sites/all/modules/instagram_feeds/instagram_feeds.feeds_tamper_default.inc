<?php
/**
 * @file
 * instagram_feeds.feeds_tamper_default.inc
 */

/**
 * Implements hook_feeds_tamper_default().
 */
function instagram_feeds_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'instagram_feed-jsonpath_parser_12-find_replace';
  $feeds_tamper->importer = 'instagram_feed';
  $feeds_tamper->source = 'jsonpath_parser:12';
  $feeds_tamper->plugin_id = 'find_replace';
  $feeds_tamper->settings = array(
    'find' => 'video',
    'replace' => '1',
    'case_sensitive' => 0,
    'word_boundaries' => 0,
    'whole' => 0,
    'regex' => FALSE,
    'func' => 'str_ireplace',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Set 1 for video';
  $export['instagram_feed-jsonpath_parser_12-find_replace'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'instagram_feed-jsonpath_parser_12-find_replace_1';
  $feeds_tamper->importer = 'instagram_feed';
  $feeds_tamper->source = 'jsonpath_parser:12';
  $feeds_tamper->plugin_id = 'find_replace';
  $feeds_tamper->settings = array(
    'find' => 'image',
    'replace' => '0',
    'case_sensitive' => 0,
    'word_boundaries' => 0,
    'whole' => 0,
    'regex' => FALSE,
    'func' => 'str_ireplace',
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Set 0 for images';
  $export['instagram_feed-jsonpath_parser_12-find_replace_1'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'instagram_feed-jsonpath_parser_3-find_replace_regex';
  $feeds_tamper->importer = 'instagram_feed';
  $feeds_tamper->source = 'jsonpath_parser:3';
  $feeds_tamper->plugin_id = 'find_replace_regex';
  $feeds_tamper->settings = array(
    'find' => '/[^(\\x20-\\x7F)]*/',
    'replace' => '',
    'limit' => '',
    'real_limit' => -1,
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Find replace REGEX - Drop Japanese Smileys (see http://drupal.org/node/1824506#comment-7225408)';
  $export['instagram_feed-jsonpath_parser_3-find_replace_regex'] = $feeds_tamper;

  return $export;
}
