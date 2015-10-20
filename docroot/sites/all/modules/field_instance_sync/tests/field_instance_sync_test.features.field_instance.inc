<?php
/**
 * @file
 * field_instance_sync_test.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function field_instance_sync_test_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-field_instance_sync_test_type_1-body'
  $field_instances['node-field_instance_sync_test_type_1-body'] = array(
    'bundle' => 'field_instance_sync_test_type_1',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'This field is used for testing.',
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
    'required' => 0,
    'settings' => array(
      'display_summary' => 1,
      'field_instance_sync' => 1,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => -4,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('This field is used for testing.');

  return $field_instances;
}
