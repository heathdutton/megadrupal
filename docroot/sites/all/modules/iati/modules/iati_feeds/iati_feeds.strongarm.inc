<?php
/**
 * @file
 * iati_feeds.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function iati_feeds_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_iati_activity_importer';
  $strongarm->value = '0';
  $export['language_content_type_iati_activity_importer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_iati_activity_importer';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_iati_activity_importer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_iati_activity_importer';
  $strongarm->value = '1';
  $export['node_preview_iati_activity_importer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_iati_activity_importer';
  $strongarm->value = 1;
  $export['node_submitted_iati_activity_importer'] = $strongarm;

  return $export;
}
