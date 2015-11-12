<?php
/**
 * @file
 * fancy_content_picture.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fancy_content_picture_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'fancy_content-picture-field_fancy_picture'
  $field_instances['fancy_content-picture-field_fancy_picture'] = array(
    'bundle' => 'picture',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'file_entity',
        'settings' => array(
          'file_view_mode' => 'full',
        ),
        'type' => 'file_rendered',
        'weight' => 0,
      ),
      'inline_entity_form_preview' => array(
        'label' => 'hidden',
        'module' => 'file_entity',
        'settings' => array(
          'file_view_mode' => 'full',
        ),
        'type' => 'file_rendered',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'fancy_content',
    'field_name' => 'field_fancy_picture',
    'label' => 'Picture',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => 'pictures',
      'file_extensions' => 'jpg gif png',
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'media',
      'settings' => array(
        'allowed_schemes' => array(
          'public' => 'public',
        ),
        'allowed_types' => array(
          'audio' => 0,
          'document' => 0,
          'image' => 'image',
          'video' => 0,
        ),
        'browser_plugins' => array(
          'media_default--media_browser_1' => 0,
          'media_default--media_browser_my_files' => 0,
          'upload' => 0,
        ),
      ),
      'type' => 'media_generic',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Picture');

  return $field_instances;
}
