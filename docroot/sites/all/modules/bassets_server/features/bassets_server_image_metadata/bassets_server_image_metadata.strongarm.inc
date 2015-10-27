<?php
/**
 * @file
 * bassets_server_image_metadata.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function bassets_server_image_metadata_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'bassets_server_locked_fieldname';
  $strongarm->value = 'field_asset_lock';
  $export['bassets_server_locked_fieldname'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'exif_mediatypes';
  $strongarm->value = array(
    'image' => 'image',
    'video' => 0,
    'audio' => 0,
    'document' => 0,
  );
  $export['exif_mediatypes'] = $strongarm;

  return $export;
}
