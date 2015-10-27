<?php
/**
 * @file
 * bassets_client_image_metadata.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function bassets_client_image_metadata_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'file-image-field_asset_caption'
  $field_instances['file-image-field_asset_caption'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 9,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'medium' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_asset_caption',
    'label' => 'Caption',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 2,
      ),
      'type' => 'text_textarea',
      'weight' => 15,
    ),
  );

  // Exported field_instance: 'file-image-field_asset_colordepth'
  $field_instances['file-image-field_asset_colordepth'] = array(
    'bundle' => 'image',
    'default_value' => array(
      0 => array(
        'value' => 24,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 42,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 16,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 42,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 42,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 42,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 42,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 42,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_asset_colordepth',
    'label' => 'Color depth',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 24,
    ),
  );

  // Exported field_instance: 'file-image-field_asset_copyright'
  $field_instances['file-image-field_asset_copyright'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'prepopulated by Exif/IPTC',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 13,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 31,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_asset_copyright',
    'label' => 'Copyright notice',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
        'rows' => 2,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 23,
    ),
  );

  // Exported field_instance: 'file-image-field_asset_datetimeoriginal'
  $field_instances['file-image-field_asset_datetimeoriginal'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 23,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
        ),
        'type' => 'date_default',
        'weight' => 25,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 23,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 23,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 32,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 23,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 23,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 23,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_asset_datetimeoriginal',
    'label' => 'Creation date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 59,
    ),
  );

  // Exported field_instance: 'file-image-field_asset_description'
  $field_instances['file-image-field_asset_description'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'prepopulated by Exif/IPTC',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_asset_description',
    'label' => 'Description',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
        'rows' => 2,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'file-image-field_asset_license'
  $field_instances['file-image-field_asset_license'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 14,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'medium' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_asset_license',
    'label' => 'License',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 20,
    ),
  );

  // Exported field_instance: 'file-image-field_asset_license_url'
  $field_instances['file-image-field_asset_license_url'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 15,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'medium' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_asset_license_url',
    'label' => 'License URL',
    'required' => 0,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => '',
        'configurable_title' => 0,
        'rel' => '',
        'target' => 'default',
        'title' => '',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'rel_remove' => 'default',
      'title' => 'value',
      'title_label_use_field_label' => FALSE,
      'title_maxlength' => 128,
      'title_value' => 'View license conditions.',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 21,
    ),
  );

  // Exported field_instance: 'file-image-field_asset_lock'
  $field_instances['file-image-field_asset_lock'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_asset_lock',
    'label' => 'Lock',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
      'wysiwyg_override' => 0,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => 'list_default',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'file-image-field_asset_photographer'
  $field_instances['file-image-field_asset_photographer'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'prepopulated by Exif/IPTC',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 20,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 12,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 20,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 20,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 29,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 20,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 20,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 20,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_asset_photographer',
    'label' => 'Photographer',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'exif_field' => 'naming_convention',
        'formatter' => '',
        'formatter_settings' => array(),
        'image_field' => 'file',
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 22,
    ),
  );

  // Exported field_instance: 'file-image-field_asset_source'
  $field_instances['file-image-field_asset_source'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 10,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_asset_source',
    'label' => 'Image source',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
        'rows' => 2,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 24,
    ),
  );

  // Exported field_instance: 'file-image-field_asset_tags'
  $field_instances['file-image-field_asset_tags'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_asset_tags',
    'label' => 'Asset Tags',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
        'rows' => 5,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'file-image-field_computed_height'
  $field_instances['file-image-field_computed_height'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 38,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 18,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 38,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 38,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 35,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 38,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 38,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 38,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_computed_height',
    'label' => 'Height',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'exif_field' => 'naming_convention',
        'formatter' => '',
        'formatter_settings' => array(),
        'image_field' => 'file',
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 17,
    ),
  );

  // Exported field_instance: 'file-image-field_computed_width'
  $field_instances['file-image-field_computed_width'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 37,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 17,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 37,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 37,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 34,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 37,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 37,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 37,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_computed_width',
    'label' => 'Width',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'exif_field' => 'naming_convention',
        'formatter' => '',
        'formatter_settings' => array(),
        'image_field' => 'file',
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 16,
    ),
  );

  // Exported field_instance: 'file-image-field_exif_aperturevalue'
  $field_instances['file-image-field_exif_aperturevalue'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 12,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 17,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 12,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 12,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 21,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 12,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 12,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 12,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_exif_aperturevalue',
    'label' => 'Exif aperture value',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'exif_field' => 'naming_convention',
        'formatter' => '',
        'formatter_settings' => array(),
        'image_field' => 'file',
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 61,
    ),
  );

  // Exported field_instance: 'file-image-field_exif_compression'
  $field_instances['file-image-field_exif_compression'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Image quality',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 15,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 20,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_exif_compression',
    'label' => 'Exif compression',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'exif_field' => 'naming_convention',
        'formatter' => '',
        'formatter_settings' => array(),
        'image_field' => 'file',
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 21,
    ),
  );

  // Exported field_instance: 'file-image-field_exif_exposuretime'
  $field_instances['file-image-field_exif_exposuretime'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 13,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 18,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 13,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 13,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 22,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 13,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 13,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 13,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_exif_exposuretime',
    'label' => 'Exif exposure time',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'exif_field' => 'naming_convention',
        'formatter' => '',
        'formatter_settings' => array(),
        'image_field' => 'file',
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 62,
    ),
  );

  // Exported field_instance: 'file-image-field_exif_flash'
  $field_instances['file-image-field_exif_flash'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 18,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 21,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 18,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 18,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 27,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 18,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 18,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 18,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_exif_flash',
    'label' => 'Exif flash usage',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'exif_field' => 'naming_convention',
        'formatter' => '',
        'formatter_settings' => array(),
        'image_field' => 'file',
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 67,
    ),
  );

  // Exported field_instance: 'file-image-field_exif_focallength'
  $field_instances['file-image-field_exif_focallength'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 20,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 25,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_exif_focallength',
    'label' => 'Exif focal length',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'exif_field' => 'naming_convention',
        'formatter' => '',
        'formatter_settings' => array(),
        'image_field' => 'file',
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 65,
    ),
  );

  // Exported field_instance: 'file-image-field_exif_isospeedratings'
  $field_instances['file-image-field_exif_isospeedratings'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 14,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 19,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 14,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 14,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 23,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 14,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 14,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 14,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_exif_isospeedratings',
    'label' => 'Exif ISO equivalent',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'exif_field' => 'naming_convention',
        'formatter' => '',
        'formatter_settings' => array(),
        'image_field' => 'file',
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 63,
    ),
  );

  // Exported field_instance: 'file-image-field_exif_model'
  $field_instances['file-image-field_exif_model'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 19,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 22,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 19,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 19,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 28,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 19,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 19,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 19,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_exif_model',
    'label' => 'Exif camera model',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'exif_field' => 'naming_convention',
        'formatter' => '',
        'formatter_settings' => array(),
        'image_field' => 'file',
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 68,
    ),
  );

  // Exported field_instance: 'file-image-field_exif_ownername'
  $field_instances['file-image-field_exif_ownername'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 21,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 24,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 21,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 21,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 30,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 21,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 21,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 21,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_exif_ownername',
    'label' => 'Exif camera owner',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'exif_field' => 'naming_convention',
        'formatter' => '',
        'formatter_settings' => array(),
        'image_field' => 'file',
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 69,
    ),
  );

  // Exported field_instance: 'file-image-field_file_image_alt_text'
  $field_instances['file-image-field_file_image_alt_text'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Alternative text is used by screen readers, search engines, and when the image cannot be loaded. By adding alt text you improve accessibility and search engine optimization.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 19,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_file_image_alt_text',
    'label' => 'Alt text',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'file-image-field_file_image_aspect_ratio'
  $field_instances['file-image-field_file_image_aspect_ratio'] = array(
    'bundle' => 'image',
    'default_value' => array(
      0 => array(
        'value' => 'other',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 19,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 36,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 39,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_file_image_aspect_ratio',
    'label' => 'Aspect ratio',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'file-image-field_file_image_title_text'
  $field_instances['file-image-field_file_image_title_text'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Title text is used in the tool tip when a user hovers their mouse over the image. Adding title text makes it easier to understand the context of an image and improves usability.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'full' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 10,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_file_image_title_text',
    'label' => 'Title text',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 14,
    ),
  );

  // Exported field_instance: 'file-image-field_file_mimetype'
  $field_instances['file-image-field_file_mimetype'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 35,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 21,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 35,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 35,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 33,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 35,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 35,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 35,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_file_mimetype',
    'label' => 'File type',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'exif_field' => 'naming_convention',
        'formatter' => '',
        'formatter_settings' => array(),
        'image_field' => 'file',
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 19,
    ),
  );

  // Exported field_instance: 'file-image-field_gps_gpslatitude'
  $field_instances['file-image-field_gps_gpslatitude'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 25,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_gps_gpslatitude',
    'label' => 'GPS latitude',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
      'wysiwyg_override' => 1,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 1,
        'formatter' => 'text_default',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 44,
    ),
  );

  // Exported field_instance: 'file-image-field_gps_gpslatituderef'
  $field_instances['file-image-field_gps_gpslatituderef'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 24,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 13,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_gps_gpslatituderef',
    'label' => 'GPS latitude reference',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
      'wysiwyg_override' => 1,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => 'text_default',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 43,
    ),
  );

  // Exported field_instance: 'file-image-field_gps_gpslongitude'
  $field_instances['file-image-field_gps_gpslongitude'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 23,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 12,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_gps_gpslongitude',
    'label' => 'GPS longitude',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
      'wysiwyg_override' => 1,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 1,
        'formatter' => 'text_default',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 46,
    ),
  );

  // Exported field_instance: 'file-image-field_gps_gpslongituderef'
  $field_instances['file-image-field_gps_gpslongituderef'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 22,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_gps_gpslongituderef',
    'label' => 'GPS longitude reference',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
      'wysiwyg_override' => 1,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => 'text_default',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 45,
    ),
  );

  // Exported field_instance: 'file-image-field_image_colorspace'
  $field_instances['file-image-field_image_colorspace'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 43,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 23,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 43,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 43,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 43,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 43,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 43,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_image_colorspace',
    'label' => 'Color space',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 23,
    ),
  );

  // Exported field_instance: 'file-image-field_image_iscolor'
  $field_instances['file-image-field_image_iscolor'] = array(
    'bundle' => 'image',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Choose "monochrome" in case the image is basically black and white but colored. A typical case is the sepia effect.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 41,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 22,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 41,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 41,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 38,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 41,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 41,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 41,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_image_iscolor',
    'label' => 'Color image?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 22,
    ),
  );

  // Exported field_instance: 'file-image-field_image_metering_mode'
  $field_instances['file-image-field_image_metering_mode'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 27,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 24,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_image_metering_mode',
    'label' => 'Metering mode',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 64,
    ),
  );

  // Exported field_instance: 'file-image-field_image_motive_category'
  $field_instances['file-image-field_image_motive_category'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 7,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_image_motive_category',
    'label' => 'Motive category',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'file-image-field_image_motive_type'
  $field_instances['file-image-field_image_motive_type'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 6,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_image_motive_type',
    'label' => 'Motive type',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'file-image-field_image_orientation'
  $field_instances['file-image-field_image_orientation'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 28,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_image_orientation',
    'label' => 'Orientation',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 20,
    ),
  );

  // Exported field_instance: 'file-image-field_image_print_resolution'
  $field_instances['file-image-field_image_print_resolution'] = array(
    'bundle' => 'image',
    'default_value' => array(
      0 => array(
        'value' => 300,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_integer',
        'weight' => 25,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 14,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_image_print_resolution',
    'label' => 'Print resolution',
    'required' => 1,
    'settings' => array(
      'max' => '',
      'min' => 1,
      'prefix' => '',
      'suffix' => ' dpi',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 25,
    ),
  );

  // Exported field_instance: 'file-image-field_image_subjectdistancerange'
  $field_instances['file-image-field_image_subjectdistancerange'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 26,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 26,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_image_subjectdistancerange',
    'label' => 'Distance to subject',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 66,
    ),
  );

  // Exported field_instance: 'file-image-field_printable_measure'
  $field_instances['file-image-field_printable_measure'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 20,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 37,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 40,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_printable_measure',
    'label' => 'Printable measure',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
        'size' => 60,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 27,
    ),
  );

  // Exported field_instance: 'file-image-field_remarks'
  $field_instances['file-image-field_remarks'] = array(
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 11,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 18,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_remarks',
    'label' => 'Remarks',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
        'rows' => 2,
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 26,
    ),
  );

  // Exported field_instance: 'file-image-field_unit_of_measurement'
  $field_instances['file-image-field_unit_of_measurement'] = array(
    'bundle' => 'image',
    'default_value' => array(
      0 => array(
        'value' => 2,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'full' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 24,
      ),
      'large' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'large_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'medium' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'medium_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'thumbnail' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'thumbnail_copyright' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_unit_of_measurement',
    'label' => 'Unit',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(
        'display_empty' => 0,
        'formatter' => '',
        'formatter_settings' => array(),
      ),
      'type' => 'field_extrawidgets_read_only',
      'weight' => 26,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Alt text');
  t('Alternative text is used by screen readers, search engines, and when the image cannot be loaded. By adding alt text you improve accessibility and search engine optimization.');
  t('Aspect ratio');
  t('Asset Tags');
  t('Caption');
  t('Choose "monochrome" in case the image is basically black and white but colored. A typical case is the sepia effect.');
  t('Color depth');
  t('Color image?');
  t('Color space');
  t('Copyright notice');
  t('Creation date');
  t('Description');
  t('Distance to subject');
  t('Exif ISO equivalent');
  t('Exif aperture value');
  t('Exif camera model');
  t('Exif camera owner');
  t('Exif compression');
  t('Exif exposure time');
  t('Exif flash usage');
  t('Exif focal length');
  t('File type');
  t('GPS latitude');
  t('GPS latitude reference');
  t('GPS longitude');
  t('GPS longitude reference');
  t('Height');
  t('Image quality');
  t('Image source');
  t('License');
  t('License URL');
  t('Lock');
  t('Metering mode');
  t('Motive category');
  t('Motive type');
  t('Orientation');
  t('Photographer');
  t('Print resolution');
  t('Printable measure');
  t('Remarks');
  t('Title text');
  t('Title text is used in the tool tip when a user hovers their mouse over the image. Adding title text makes it easier to understand the context of an image and improves usability.');
  t('Unit');
  t('Width');
  t('prepopulated by Exif/IPTC');

  return $field_instances;
}
