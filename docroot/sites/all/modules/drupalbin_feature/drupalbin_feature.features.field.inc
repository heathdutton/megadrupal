<?php
/**
 * @file
 * drupalbin_feature.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function drupalbin_feature_field_default_fields() {
  $fields = array();

  // Exported field: 'node-code-field_code'
  $fields['node-code-field_code'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_code',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'geshifield',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'geshifield',
    ),
    'field_instance' => array(
      'bundle' => 'code',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'geshifield',
          'settings' => array(),
          'type' => 'default',
          'weight' => '1',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'geshifield',
          'settings' => array(),
          'type' => 'trimmed',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_code',
      'label' => 'Code',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'geshifield',
        'settings' => array(),
        'type' => 'geshifield_textarea',
        'weight' => '1',
      ),
    ),
  );

  // Exported field: 'node-code-field_tags'
  $fields['node-code-field_tags'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_tags',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid',
          ),
          'table' => 'taxonomy_term_data',
        ),
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'tags',
            'parent' => 0,
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
    ),
    'field_instance' => array(
      'bundle' => 'code',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Any tags you\'d like to associate with your code, delimitered by commas (example: Views, CCK, Module, etc).',
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
      'field_name' => 'field_tags',
      'label' => 'Tags',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'taxonomy',
        'settings' => array(
          'autocomplete_path' => 'taxonomy/autocomplete',
          'size' => 60,
        ),
        'type' => 'taxonomy_autocomplete',
        'weight' => '2',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Any tags you\'d like to associate with your code, delimitered by commas (example: Views, CCK, Module, etc).');
  t('Code');
  t('Tags');

  return $fields;
}
