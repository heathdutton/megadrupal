<?php
/**
 * @file
 * cod_session.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function cod_session_taxonomy_default_vocabularies() {
  return array(
    'schedule_item_tags' => array(
      'name' => 'Schedule Item tags',
      'machine_name' => 'schedule_item_tags',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
