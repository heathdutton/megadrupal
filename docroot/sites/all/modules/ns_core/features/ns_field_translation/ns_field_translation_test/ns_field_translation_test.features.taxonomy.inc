<?php
/**
 * @file
 * ns_field_translation_test.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function ns_field_translation_test_taxonomy_default_vocabularies() {
  return array(
    'ns_field_translation_test' => array(
      'name' => 'ns_field_translation_test',
      'machine_name' => 'ns_field_translation_test',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
  );
}
