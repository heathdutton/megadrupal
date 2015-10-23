<?php

/**
 * @file
 * Specify common apps components to be created by apps_compatible.
 */

/**
 * Implements hook_apps_compatible_info().
 */
function storiedmaps_core_apps_compatible_info() {
  return array(
    // Ensure a set of field bases is created.
    'field_base' => array(
      'field_content_image' => array(
        'machine name' => 'field_content_image',
      ),
      'field_media' => array(
        'machine name' => 'field_media',
      ),
    ),
  );
}

