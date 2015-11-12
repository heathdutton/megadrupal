<?php
/**
 * @file
 * mirrors_example.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function mirrors_example_taxonomy_default_vocabularies() {
  return array(
    'no_fields' => array(
      'name' => 'Taxonomy no Fields',
      'machine_name' => 'no_fields',
      'description' => '',
      'hierarchy' => 1,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'with_fields' => array(
      'name' => 'Taxonomy with Fields',
      'machine_name' => 'with_fields',
      'description' => '',
      'hierarchy' => 1,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
