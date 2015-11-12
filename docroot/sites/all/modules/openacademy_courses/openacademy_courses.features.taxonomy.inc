<?php
/**
 * @file
 * openacademy_courses.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function openacademy_courses_taxonomy_default_vocabularies() {
  return array(
    'openacademy_course_type' => array(
      'name' => 'Course Type',
      'machine_name' => 'openacademy_course_type',
      'description' => 'A categorization of courses',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
