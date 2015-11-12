<?php
/**
 * @file
 * openacademy_people.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function openacademy_people_taxonomy_default_vocabularies() {
  return array(
    'openacademy_person_type' => array(
      'name' => 'Person Type',
      'machine_name' => 'openacademy_person_type',
      'description' => 'A categorization of people',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
