<?php
/**
 * @file
 * one_import.feeds_tamper_default.inc
 */

/**
 * Implements hook_feeds_tamper_default().
 */
function one_import_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'one_site_content_import-blank_source_1-rewrite';
  $feeds_tamper->importer = 'one_site_content_import';
  $feeds_tamper->source = 'Blank source 1';
  $feeds_tamper->plugin_id = 'rewrite';
  $feeds_tamper->settings = array(
    'text' => 'more..',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Rewrite';
  $export['one_site_content_import-blank_source_1-rewrite'] = $feeds_tamper;

  return $export;
}
