<?php
/**
 * @file
 * nm_activity.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function nm_activity_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'activity_access_realms';
  $strongarm->value = array(
    0 => 'comment',
    1 => 'node_author',
  );
  $export['activity_access_realms'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'activity_expire';
  $strongarm->value = '0';
  $export['activity_expire'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'activity_min_count';
  $strongarm->value = '0';
  $export['activity_min_count'] = $strongarm;

  return $export;
}
