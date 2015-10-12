<?php
/**
 * @file
 * bassets_client_demo.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function bassets_client_demo_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-bassets_demo-body'
  $field_instances['node-bassets_demo-body'] = array(
    'bundle' => 'bassets_demo',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => -4,
    ),
  );

  // Exported field_instance: 'node-bassets_demo-field_bassets_image'
  $field_instances['node-bassets_demo-field_bassets_image'] = array(
    'bundle' => 'bassets_demo',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file_entity',
        'settings' => array(
          'file_view_mode' => 'large_copyright',
        ),
        'type' => 'file_rendered',
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
    'field_name' => 'field_bassets_image',
    'label' => 'Image (Large with Copyright)',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => 'bassets_demo',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'media',
      'settings' => array(
        'allowed_schemes' => array(
          'bassets' => 'bassets',
          'private' => 'private',
          'public' => 'public',
        ),
        'allowed_types' => array(
          'audio' => 0,
          'document' => 0,
          'image' => 'image',
          'video' => 0,
        ),
        'browser_plugins' => array(
          'bassets' => 'bassets',
          'media_browser_bassets--media_browser_1' => 'media_browser_bassets--media_browser_1',
          'media_browser_bassets--media_browser_my_files' => 'media_browser_bassets--media_browser_my_files',
          'upload' => 0,
        ),
      ),
      'type' => 'media_generic',
      'weight' => -2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('Image (Large with Copyright)');

  return $field_instances;
}
