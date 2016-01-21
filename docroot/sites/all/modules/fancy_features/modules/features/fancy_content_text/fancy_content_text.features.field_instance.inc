<?php
/**
 * @file
 * fancy_content_text.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fancy_content_text_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'fancy_content-text-field_fancy_text'
  $field_instances['fancy_content-text-field_fancy_text'] = array(
    'bundle' => 'text',
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
      'inline_entity_form_preview' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'fancy_content',
    'field_name' => 'field_fancy_text',
    'label' => 'Text',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Text');

  return $field_instances;
}
