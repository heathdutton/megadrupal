<?php
/**
 * @file
 * openacademy_core.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function openacademy_core_taxonomy_default_vocabularies() {
  return array(
    'openacademy_academic_term' => array(
      'name' => 'Academic Term',
      'machine_name' => 'openacademy_academic_term',
      'description' => 'A categorization of academic terms',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'openacademy_tags' => array(
      'name' => 'Tags',
      'machine_name' => 'openacademy_tags',
      'description' => 'A general vocabulary of tags to use throughout the site.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
