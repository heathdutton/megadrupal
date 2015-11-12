<?php
/**
 * @file
 * fancy_content_headline.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function fancy_content_headline_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_fancy_style'
  $field_bases['field_fancy_style'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_fancy_style',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'headline-bold' => 'Bold',
        'headline-3d' => '3D',
        'headline-letterpress' => 'Letterpress',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_fancy_textcolor'
  $field_bases['field_fancy_textcolor'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_fancy_textcolor',
    'foreign keys' => array(),
    'indexes' => array(
      'rgb' => array(
        0 => 'rgb',
      ),
    ),
    'locked' => 0,
    'module' => 'color_field',
    'settings' => array(
      'allow_empty' => 1,
      'palette' => '',
      'show_buttons' => 1,
      'show_input' => 1,
      'show_palette' => 0,
      'show_palette_only' => 0,
    ),
    'translatable' => 0,
    'type' => 'color_field_rgb',
  );

  // Exported field_base: 'field_headline'
  $field_bases['field_headline'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_headline',
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
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  return $field_bases;
}
