<?php
// $Id$

/**
 * Implementation of hook_rules_event_info().
 */
function field_rules_rules_event_info() {
  $info = array();
  $info['node_type_insert'] = array(
    'group' => t('Data'),
    'module' => 'field_rules',
    'access callback' => 'field_rules_access',
    'label' => t('After saving a new content type'),
    'variables' => array(
      'node_type' => array('type' => 'node_type', 'label' => t('Node type')),
    ),
  );
  return $info;
}

/**
 * Implementation of hook_rules_action_info().
 */
function field_rules_rules_action_info() {
  $info = array();
  $info['field_rules_create_field'] = array(
    'label' => t('Create a field'),
    'group' => t('Field'),
    'provides' => array(
      'created_field' => array(
        'label' => t('Created field'),
        'type' => 'field',
      ),
    ),
  );
  $info['field_rules_save_field'] = array(
    'label' => t('Save a field'),
    'group' => t('Field'),
    'parameter' => array(
      'field' => array(
        'type' => 'field',
        'label' => t('Field'),
      ),
    ),
  );
  $info['field_rules_load_field'] = array(
    'label' => t('Load a field'),
    'group' => t('Field'),
    'parameter' => array(
      'field_name' => array(
        'type' => 'text',
        'label' => t('Field name'),
      ),
    ),
    'provides' => array(
      'loaded_field' => array(
        'label' => t('Loaded field'),
        'type' => 'field',
      ),
    ),
  );
  $info['field_rules_delete_field'] = array(
    'label' => t('Delete a field'),
    'group' => t('Field'),
    'parameter' => array(
      'field' => array(
        'type' => 'field',
        'label' => t('Field'),
      ),
    ),
  );
  $info['field_rules_create_field_instance'] = array(
    'label' => t('Create a field instance'),
    'group' => t('Field instance'),
    'provides' => array(
      'created_field_instance' => array(
        'label' => t('Created field instance'),
        'type' => 'field_instance',
      ),
    ),
    'parameter' => array(
      'node_type' => array(
        'type' => 'node_type',
        'label' => t('Node type'),
      ),
      'field' => array(
        'type' => 'field',
        'label' => t('Field'),
      ),
    ),
  );
  $info['field_rules_save_field_instance'] = array(
    'label' => t('Save a field instance'),
    'group' => t('Field instance'),
    'parameter' => array(
      'field_instance' => array(
        'type' => 'field_instance',
        'label' => t('Field instance'),
      ),
    ),
  );
  $info['field_rules_load_field_instance'] = array(
    'label' => t('Load a field instance'),
    'group' => t('Field instance'),
    'parameter' => array(
      'node_type' => array(
        'type' => 'node_type',
        'label' => t('Node type'),
      ),
      'field_instance' => array(
        'type' => 'field',
        'label' => t('Field'),
      ),
    ),
    'provides' => array(
      'loaded_field_instance' => array(
        'label' => t('Loaded field instance'),
        'type' => 'field_instance',
      ),
    ),
  );
  $info['field_rules_delete_field_instance'] = array(
    'label' => t('Delete a field instance'),
    'group' => t('Field instance'),
    'parameter' => array(
      'field_instance' => array(
        'type' => 'field_instance',
        'label' => t('Field instance'),
      ),
    ),
  );
  return $info;
}

/**
 * Implementation of hook_rules_data_info().
 */
function field_rules_rules_data_info() {
  return array(
    'field' => array(
      'label' => t('field'),
      'wrap' => TRUE,
      'property info' => _field_rules_field_info(),
    ),
    'field_instance' => array(
      'label' => t('field_instance'),
      'wrap' => TRUE,
      'property info' => _field_rules_field_instance_info(),
      //'creation callback' => 'field_rules_field_create',
    ),
  );
}

function _field_rules_field_info() {
  $properties = array();
  $properties['id'] = array(
    'label' => t('Field Id'),
    'type' => 'integer',
    'description' => t('The unique ID of the field.'),
    'getter callback' => 'entity_property_verbatim_get',
  );
  $properties['field_name'] = array(
    'label' => t('Field name'),
    'type' => 'text',
    'description' => t('The machine name of this field.'),
    'setter callback' => 'entity_property_verbatim_set',
    'getter callback' => 'entity_property_verbatim_get',
  );
  $properties['type'] = array(
    'label' => t('Field type'),
    'type' => 'text',
    'description' => t('The type this field.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer content types',
    'getter callback' => 'entity_property_verbatim_get',
  );
  $properties['active'] = array(
    'label' => t('Active'),
    'type' => 'boolean',
    'description' => t('Whether the field is active.'),
    'setter callback' => 'entity_property_verbatim_set',
    'getter callback' => 'entity_property_verbatim_get',
  );
  $properties['locked'] = array(
    'label' => t('Locked'),
    'type' => 'boolean',
    'description' => t('Whether the field is locked.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer content types',
    'getter callback' => 'entity_property_verbatim_get',
  );
  $properties['cardinality'] = array(
    'label' => t('Cardinality'),
    'type' => 'integer',
    'description' => t('Field cardinality.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer content types',
    'getter callback' => 'entity_property_verbatim_get',
  );
  $properties['translatable'] = array(
    'label' => t('Translatable'),
    'type' => 'boolean',
    'description' => t('Whether the field is translatable.'),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer content types',
    'getter callback' => 'entity_property_verbatim_get',
  );
  return $properties;
}

function _field_rules_field_instance_info() {
  $properties = array();
  $properties['id'] = array(
    'label' => t('Field instance id'),
    'type' => 'integer',
    'description' => t('The unique ID of the field instance.'),
    'getter callback' => 'entity_property_verbatim_get',
  );
  $properties['field_id'] = array(
    'label' => t('Field Id'),
    'type' => 'integer',
    'description' => t('The id of the field this belongs to.'),
    'getter callback' => 'entity_property_verbatim_get',
    'setter permission' => 'administer content types',
  );
  $properties['field_name'] = array(
    'label' => t('Field name'),
    'type' => 'text',
    'description' => t('The machine name of this field.'),
    'getter callback' => 'entity_property_verbatim_get',
    'setter permission' => 'administer content types',
  );
  return $properties;
}

function field_rules_access() {
  return TRUE;
}

function field_rules_create_field() {
  return array('created_field' => array('type' => 'text'));
}

function field_rules_save_field($field) {
  try {
    return field_create_field($field);
  }
  catch (FieldException $e) {
    return field_update_field($field);
  }
}

function field_rules_load_field($field_name) {
  if ($field = field_read_field($field_name)) {
    return array('loaded_field' => $field);
  }
}

function field_rules_delete_field($field) {
  return field_delete_field($field['field_name']);
}

function field_rules_create_field_instance($node_type, $field = NULL) {
  $instance = array();
  $instance['field_name'] = $field['field_name'];
  $instance['entity_type'] = 'node';
  $instance['bundle'] = $node_type->type;

  return array('created_field_instance' => field_create_instance($instance));
}

function field_rules_save_field_instance($field_instance) {
  try {
    return field_update_instance($field_instance);
  }
  catch (FieldException $e) {
    return field_create_instance($field_instance);
  }
}

function field_rules_load_field_instance($node_type, $field) {
  return array('loaded_field_instance' => field_read_instance('node', $field['field_name'], $node_type->type));
}

function field_rules_delete_field_instance($field_instance) {
  return field_delete_instance($field_instance);
}
