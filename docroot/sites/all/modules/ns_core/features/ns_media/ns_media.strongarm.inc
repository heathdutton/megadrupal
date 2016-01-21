<?php
/**
 * @file
 * ns_media.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function ns_media_defaultconfig_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'file_submitted_image';
  $strongarm->value = FALSE;
  $export['file_submitted_image'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'file_submitted_video';
  $strongarm->value = FALSE;
  $export['file_submitted_video'] = $strongarm;

  return $export;
}
