<?php
/**
 * @file
 * showoff_feeds.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function showoff_feeds_taxonomy_default_vocabularies() {
  return array(
    'feeds' => array(
      'name' => 'Feed Category',
      'machine_name' => 'feeds',
      'description' => 'Tag items with the category so you know to which feeds they belong.',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
  );
}
