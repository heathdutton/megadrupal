<?php
/**
 * @file
 * mm_media.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function mm_media_taxonomy_default_vocabularies() {
  return array(
    'vocabulary_1' => array(
      'name' => 'Tags',
      'machine_name' => 'vocabulary_1',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
