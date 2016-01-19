<?php
/**
 * @file
 * bassets_client_entity_view_mode.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function bassets_client_entity_view_mode_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'entity_view_modes';
  $strongarm->value = array(
    'file' => array(
      'thumbnail' => array(
        'label' => 'Thumbnail',
        'custom settings' => 1,
      ),
      'medium' => array(
        'label' => 'Medium',
        'custom settings' => 1,
      ),
      'large' => array(
        'label' => 'Large',
        'custom settings' => 1,
      ),
      'medium_copyright' => array(
        'label' => 'Medium with copyright',
        'custom settings' => 1,
      ),
      'large_copyright' => array(
        'label' => 'Large with copyright',
        'custom settings' => 1,
      ),
      'thumbnail_copyright' => array(
        'label' => 'Thumbnail with copyright',
        'custom settings' => 1,
      ),
    ),
  );
  $export['entity_view_modes'] = $strongarm;

  return $export;
}
