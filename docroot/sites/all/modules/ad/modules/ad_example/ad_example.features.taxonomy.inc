<?php
/**
 * @file
 * ad_example.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function ad_example_taxonomy_default_vocabularies() {
  return array(
    'advertisement_size' => array(
      'name' => 'Advertisement size',
      'machine_name' => 'advertisement_size',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
