<?php
/**
 * @file
 * fancy_content_ctools_content.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fancy_content_ctools_content_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'fancy_content-ctools_content-field_fancy_ctools_content'
  $field_instances['fancy_content-ctools_content-field_fancy_ctools_content'] = array(
    'bundle' => 'ctools_content',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'ctools_field',
        'settings' => array(),
        'type' => 'ctools_field_formatter',
        'weight' => 0,
      ),
      'inline_entity_form_preview' => array(
        'label' => 'hidden',
        'module' => 'ctools_field',
        'settings' => array(),
        'type' => 'ctools_field_formatter',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'fancy_content',
    'field_name' => 'field_fancy_ctools_content',
    'label' => 'Ctools Content',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'ctools_field',
      'settings' => array(),
      'type' => 'ds_selector',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Ctools Content');

  return $field_instances;
}
