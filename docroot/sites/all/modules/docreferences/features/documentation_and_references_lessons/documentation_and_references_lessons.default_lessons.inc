<?php
/**
 * @file
 * documentation_and_references_lessons.default_lessons.inc
 */

/**
 * Implements hook_default_lessons().
 */
function documentation_and_references_lessons_default_lessons() {
  $export = array();

  $lessons = new stdClass;
  $lessons->disabled = FALSE; /* Edit this to true to make a default lessons disabled initially */
  $lessons->api_version = 1;
  $lessons->name = 'docreferences_create_docreferences';
  $lessons->title = 'Add documentation and references (Documentation and References module)';
  $lessons->author = '';
  $lessons->project = 'strongarm';
  $lessons->project_type = 'contrib';
  $lessons->lesson_type = 'configuration';
  $lessons->description = '';
  $lessons->version = '';
  $lessons->steps = 'TODO write this lesson';
  $lessons->permission = '';
  $export['docreferences_create_docreferences'] = $lessons;

  return $export;
}
