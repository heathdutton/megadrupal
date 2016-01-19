<?php
/**
 * @file
 * drustack_core.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function drustack_core_taxonomy_default_vocabularies() {
  return array(
    'multiple_tags' => array(
      'name' => 'Multiple Tags',
      'machine_name' => 'multiple_tags',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => -8,
    ),
    'single_tags' => array(
      'name' => 'Single Tags',
      'machine_name' => 'single_tags',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => -9,
    ),
    'tags' => array(
      'name' => 'Tags',
      'machine_name' => 'tags',
      'description' => 'Use tags to group articles on similar topics into categories.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => -10,
    ),
  );
}
