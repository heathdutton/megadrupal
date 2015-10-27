<?php
/**
 * @file
 * Fields for the `Tumblr` content type.
 *
 * An array of fields and field instances used by the content type.
 */

/**
 * Implements hook_field_default_field_bases().
 */
function social_content_tumblr_field_default_field_bases() {
  $field_bases = array();
  return $field_bases;
}


/**
 * Implements hook_field_default_field_instances().
 */
function social_content_tumblr_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-tumblr-body'
  $field_instances['node-tumblr-body'] = array(
    'bundle' => 'tumblr',
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
      'entity_translation_sync' => FALSE,
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
      'weight' => 31,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');

  return $field_instances;
}
