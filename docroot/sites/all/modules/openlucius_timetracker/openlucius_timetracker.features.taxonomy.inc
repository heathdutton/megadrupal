<?php
/**
 * @file
 * openlucius_timetracker.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function openlucius_timetracker_taxonomy_default_vocabularies() {
  return array(
    'timetracker_type' => array(
      'name' => 'Timetracker type',
      'machine_name' => 'timetracker_type',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
