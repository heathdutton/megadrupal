<?php
/**
 * @file
 * bassets_server_image_metadata_rules.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function bassets_server_image_metadata_rules_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'entity_rules_file__image_create';
  $strongarm->value = array(
    'rules_bassets_insert_collection' => array(
      'args' => array(),
    ),
  );
  $export['entity_rules_file__image_create'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'entity_rules_file__image_update';
  $strongarm->value = array(
    'rules_bassets_update_collection' => array(
      'args' => array(),
    ),
  );
  $export['entity_rules_file__image_update'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'entity_rules_settings';
  $strongarm->value = array(
    'entity_types' => array(
      'file' => 'file',
      'node' => 0,
      'taxonomy_term' => 0,
      'user' => 0,
      'rules_config' => 0,
    ),
  );
  $export['entity_rules_settings'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_file__image';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'preview' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'filename' => array(
          'weight' => '0',
        ),
        'preview' => array(
          'weight' => '1',
        ),
      ),
      'display' => array(
        'file' => array(
          'default' => array(
            'weight' => '0',
            'visible' => TRUE,
          ),
          'media_small' => array(
            'weight' => 0,
            'visible' => FALSE,
          ),
          'full' => array(
            'weight' => '0',
            'visible' => TRUE,
          ),
        ),
      ),
    ),
  );
  $export['field_bundle_settings_file__image'] = $strongarm;

  return $export;
}
