<?php
/**
 * @file
 * color_tagger_example.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function color_tagger_example_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-picture-field_color'
  $field_instances['node-picture-field_color'] = array(
    'bundle' => 'picture',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_color',
    'label' => 'Color',
    'required' => 0,
    'settings' => array(
      'entity_translation_sync' => FALSE,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'color_tagger',
      'settings' => array(
        'autocomplete_path' => 'taxonomy/autocomplete',
        'hex_code_field' => 'field_hex_code',
        'image_field' => 'field_image',
        'size' => 60,
      ),
      'type' => 'color_tagger_widget',
      'weight' => -1,
    ),
  );

  // Exported field_instance: 'node-picture-field_image'
  $field_instances['node-picture-field_image'] = array(
    'bundle' => 'picture',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => '',
        ),
        'type' => 'image',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_image',
    'label' => 'Image',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'entity_translation_sync' => FALSE,
      'file_directory' => 'field_image',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'image',
      'settings' => array(
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => -2,
    ),
  );

  // Exported field_instance: 'taxonomy_term-colors-field_hex_code'
  $field_instances['taxonomy_term-colors-field_hex_code'] = array(
    'bundle' => 'colors',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'A hexadecimal code for the color of this taxonomy. Leave off any prefix such as \'#\'.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_hex_code',
    'label' => 'Hexadecimal Value',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 10,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('A hexadecimal code for the color of this taxonomy. Leave off any prefix such as \'#\'.');
  t('Color');
  t('Hexadecimal Value');
  t('Image');

  return $field_instances;
}
