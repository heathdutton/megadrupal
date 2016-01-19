<?php
/**
 * @file
 * kcpd.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function kcpd_taxonomy_default_vocabularies() {
  return array(
    'genre' => array(
      'name' => 'Genre',
      'machine_name' => 'genre',
      'description' => 'Movie genres.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
