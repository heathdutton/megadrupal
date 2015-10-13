<?php
/**
 * @file
 * voipnumber_feature.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function voipnumber_feature_field_default_fields() {
  $fields = array();

  // Exported field: 'node-phone_number-field_phone_number'
  $fields['node-phone_number-field_phone_number'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_phone_number',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'voipnumberfield',
      'settings' => array(
        'voipnumberfield_allow_sip' => 1,
        'voipnumberfield_allowed_countries' => array(),
        'voipnumberfield_choice' => '1',
        'voipnumberfield_default' => '',
        'voipnumberfield_max_length' => '15',
        'voipnumberfield_unique' => 1,
      ),
      'translatable' => '0',
      'type' => 'voipnumberfield',
    ),
    'field_instance' => array(
      'bundle' => 'phone_number',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'voipnumberfield',
          'settings' => array(),
          'type' => 'default',
          'weight' => 0,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_phone_number',
      'label' => 'Phone Number',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'voipnumberfield',
        'settings' => array(),
        'type' => 'voipphonenumber_widget',
        'weight' => '-4',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Phone Number');

  return $fields;
}
