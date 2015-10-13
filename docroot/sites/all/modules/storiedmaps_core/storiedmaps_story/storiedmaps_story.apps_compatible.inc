<?php

/**
 * @file
 * Specify common apps components to be created by apps_compatible.
 */

/**
 * Implements hook_apps_compatible_info().
 */
function storiedmaps_story_apps_compatible_info() {
  return array(
    // Ensure the 'tags' vocabulary, as defined in apps_compatible, is created.
    'vocabulary' => array(
      'tags' => array(
        'machine name' => 'tags',
      ),
    ),
    // Ensure a set of field bases is created.
    'field_base' => array(
      'body' => array(
        'machine name' => 'body',
      ),
      'field_content_image' => array(
        'machine name' => 'field_content_image',
      ),
      'field_media' => array(
        'machine name' => 'field_media',
      ),
      'field_tags' => array(
        'machine name' => 'field_tags',
      ),
    ),
  );
}

