<?php
/**
 * @file
 * wsclient_feeds_demo.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function wsclient_feeds_demo_taxonomy_default_vocabularies() {
  return array(
    'venues' => array(
      'name' => 'Venues',
      'machine_name' => 'venues',
      'description' => 'Locations for events',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
  );
}
