<?php
/**
 * @file
 * amazon_examples.features.field.inc
 */

/**
 * Implementation of hook_field_default_fields().
 */
function amazon_examples_field_default_fields() {
  $fields = array();

  // Exported field: 'node-amazon_example-body'
  $fields['node-amazon_example-body'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(
        0 => 'node',
      ),
      'field_name' => 'body',
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
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_with_summary',
    ),
    'field_instance' => array(
      'bundle' => 'amazon_example',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '0',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'body',
      'label' => 'Body',
      'required' => 0,
      'settings' => array(
        'display_summary' => 1,
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '4',
          'summary_rows' => 5,
        ),
        'type' => 'text_textarea_with_summary',
        'weight' => '-4',
      ),
      'widget_type' => 'text_textarea_with_summary',
    ),
  );

  // Exported field: 'node-amazon_example-field_asin'
  $fields['node-amazon_example-field_asin'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_asin',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'asin',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'asin',
    ),
    'field_instance' => array(
      'bundle' => 'amazon_example',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'asin',
          'settings' => array(),
          'type' => 'default',
          'weight' => '1',
        ),
        'teaser' => array(
          'label' => 'above',
          'module' => 'asin',
          'settings' => array(),
          'type' => 'details',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_asin',
      'label' => 'ASIN',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'asin',
        'settings' => array(),
        'type' => 'asin_autocomplete',
        'weight' => '-3',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('ASIN');
  t('Body');

  return $fields;
}
