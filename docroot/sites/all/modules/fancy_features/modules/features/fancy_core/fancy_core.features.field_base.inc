<?php
/**
 * @file
 * fancy_core.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function fancy_core_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_fancy_rel_contents'
  $field_bases['field_fancy_rel_contents'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_fancy_rel_contents',
    'foreign keys' => array(
      'eck_fancy_content' => array(
        'columns' => array(
          'target_id' => 'id',
        ),
        'table' => 'eck_fancy_content',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'direction' => 'ASC',
          'property' => 'title',
          'type' => 'property',
        ),
        'target_bundles' => array(),
      ),
      'target_type' => 'fancy_content',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_base: 'field_fancy_rel_sections'
  $field_bases['field_fancy_rel_sections'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_fancy_rel_sections',
    'foreign keys' => array(
      'eck_fancy_section' => array(
        'columns' => array(
          'target_id' => 'id',
        ),
        'table' => 'eck_fancy_section',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'direction' => 'ASC',
          'property' => 'title',
          'type' => 'property',
        ),
        'target_bundles' => array(),
      ),
      'target_type' => 'fancy_section',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  return $field_bases;
}
