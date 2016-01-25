<?php
/**
 * @file
 * makefile.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function makefile_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_drush_api_version'
  $field_bases['field_drush_api_version'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_drush_api_version',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(
      'profile2_private' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'number_integer',
  );

  // Exported field_base: 'field_drush_core_api_version'
  $field_bases['field_drush_core_api_version'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_drush_core_api_version',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 12,
      'profile2_private' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  // Exported field_base: 'field_drush_make_name'
  $field_bases['field_drush_make_name'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_drush_make_name',
    'foreign keys' => array(),
    'indexes' => array(
      'machine' => array(
        0 => 'machine',
      ),
    ),
    'locked' => 0,
    'module' => 'safeword',
    'settings' => array(
      'allow_machine_changes' => 1,
      'machine_description' => 'A machine name version of the text. It may only contain lowercase letters, numbers and underscores. Leave blank to re-generate.',
      'machine_label' => 'Machine Name',
      'max_length' => 255,
      'profile2_private' => FALSE,
      'replace_pattern' => '(--|<[^<>]+>|[^a-z0-9-])+',
      'replace_value' => '-',
      'show_complete_path' => 0,
      'unique' => 1,
    ),
    'translatable' => 0,
    'type' => 'safeword',
  );

  return $field_bases;
}
