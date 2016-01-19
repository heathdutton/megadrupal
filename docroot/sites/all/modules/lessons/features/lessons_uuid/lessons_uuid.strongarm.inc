<?php
/**
 * @file
 * lessons_uuid.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function lessons_uuid_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_ladder_pattern';
  $strongarm->value = 'ladder/[node:uuid]';
  $export['pathauto_node_ladder_pattern'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_lesson_pattern';
  $strongarm->value = 'lesson/[node:uuid]';
  $export['pathauto_node_lesson_pattern'] = $strongarm;

  return $export;
}
