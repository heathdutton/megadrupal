<?php
/**
 * @file
 * oa_migrate_legacy_keywords.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function oa_migrate_legacy_keywords_taxonomy_default_vocabularies() {
  return array(
    'oa_migrate_legacy_keywords' => array(
      'name' => 'Keywords',
      'machine_name' => 'oa_migrate_legacy_keywords',
      'description' => 'Free-form tags.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
