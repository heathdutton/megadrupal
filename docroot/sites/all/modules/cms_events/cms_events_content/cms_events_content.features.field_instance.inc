<?php
/**
 * @file
 * cms_events_content.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function cms_events_content_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-event-field_glazed_header_image'
  $field_instances['node-event-field_glazed_header_image'] = array(
    'bundle' => 'event',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'bg_image_formatter',
        'settings' => array(
          'css_settings' => array(
            'bg_image_attachment' => 'scroll',
            'bg_image_background_size' => 'cover',
            'bg_image_background_size_ie8' => 1,
            'bg_image_color' => 'silver',
            'bg_image_important' => 1,
            'bg_image_media_query' => 'all',
            'bg_image_repeat' => 'no-repeat',
            'bg_image_selector' => '.header-full-width-container',
            'bg_image_x' => 'left',
            'bg_image_y' => 'top',
          ),
          'field_formatter_class' => '',
          'image_style' => '',
        ),
        'type' => 'bg_image_formatter',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_glazed_header_image',
    'label' => 'Glazed Header Image',
    'required' => FALSE,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => '',
      'file_extensions' => 'png gif jpg jpeg',
      'image_field_caption' => 0,
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'media',
      'settings' => array(
        'allowed_schemes' => array(
          0 => 'public',
          1 => 'private',
        ),
        'allowed_types' => array(
          0 => 'image',
        ),
        'browser_plugins' => array(),
      ),
      'type' => 'media_generic',
      'weight' => 9,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Glazed Header Image');

  return $field_instances;
}
