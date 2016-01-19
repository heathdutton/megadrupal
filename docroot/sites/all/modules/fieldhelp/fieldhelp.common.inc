<?php
/**
 * @file
 * Exports that help Drupal developers when working with fields.
 */

/**
 * Export a given field definition.
 */
function fieldhelp_field_export_field($field_name) {
  $field = field_info_field($field_name);
  unset($field['id']);
  $field = fieldhelp_remove_field_defaults($field);
  // Bundles and details are provided by prepareField() and are *not* gathered
  // from the field_config table, although details are redundantly saved there.
  unset($field['bundles']);
  unset($field['storage']['details']);
  if ($field['storage'] == array()) {
    unset($field['storage']);
  }
  // 'columns' and 'foreign keys', hardcoded in the field type, are removed as
  // defaults, so don't need unset($field['columns'], $field['foreign keys']);
  // In theory implementations of hook_field_create_field() might make defaults
  // we'd want to try to remove, but use of this hook is very rare.
  return $field;
}

/**
 * Export a given field instance.
 */
function fieldhelp_field_export_instance($entity_type, $field_name, $bundle_name) {
  $instance = field_info_instance($entity_type, $field_name, $bundle_name);
  unset($instance['id'], $instance['field_id']);
  $instance = fieldhelp_remove_instance_defaults($instance);
  return $instance;
}

/**
 * Return a field array stripped of all defaults.
 */
function fieldhelp_remove_field_defaults($field) {
  $minimal_field = array(
    'field_name' => $field['field_name'],
    'type' => $field['type'],
  );
  $defaults = fieldhelp_field_defaults($minimal_field);
  $stripped = array_diff_assoc_recursive($field, $defaults);
  // Pass back in the minimal field definition.
  return array_merge($minimal_field, $stripped);
}

/**
 * Provides the defaults which field_create_field() provides.
 *
 * Code taken unchanged from field_create_field().
 *
 * Takes a minimalist field array with field_name and type.
 */
function fieldhelp_field_defaults($field) {
  $field += array(
    'entity_types' => array(),
    'cardinality' => 1,
    'translatable' => FALSE,
    'locked' => FALSE,
    'settings' => array(),
    'storage' => array(),
    'deleted' => 0,
  );

  // Check that the field type is known.
  $field_type = field_info_field_types($field['type']);
  if (!$field_type) {
    throw new FieldException(t('Attempt to create a field of unknown type %type.', array('%type' => $field['type'])));
  }
  // Create all per-field-type properties (needed here as long as we have
  // settings that impact column definitions).
  $field['settings'] += field_info_field_settings($field['type']);
  $field['module'] = $field_type['module'];
  $field['active'] = 1;

  // Provide default storage.
  $field['storage'] += array(
    'type' => variable_get('field_storage_default', 'field_sql_storage'),
    'settings' => array(),
  );
  // Check that the storage type is known.
  $storage_type = field_info_storage_types($field['storage']['type']);
  if (!$storage_type) {
    throw new FieldException(t('Attempt to create a field with unknown storage type %type.', array('%type' => $field['storage']['type'])));
  }
  // Provide default storage settings.
  $field['storage']['settings'] += field_info_storage_settings($field['storage']['type']);
  $field['storage']['module'] = $storage_type['module'];
  $field['storage']['active'] = 1;
  // Collect storage information.
  module_load_install($field['module']);
  $schema = (array) module_invoke($field['module'], 'field_schema', $field);
  $schema += array('columns' => array(), 'indexes' => array(), 'foreign keys' => array());
  // 'columns' are hardcoded in the field type.
  $field['columns'] = $schema['columns'];
  // 'foreign keys' are hardcoded in the field type.
  $field['foreign keys'] = $schema['foreign keys'];
  // 'indexes' can be both hardcoded in the field type, and specified in the
  // incoming $field definition.
  $field += array(
    'indexes' => array(),
  );
  $field['indexes'] += $schema['indexes'];

  return $field;
}

/**
 * Return a field instance array stripped of all defaults.
 */
function fieldhelp_remove_instance_defaults($instance) {
  $minimal_instance = array(
    'field_name' => $instance['field_name'],
    'entity_type' => $instance['entity_type'],
    'bundle' => $instance['bundle'],
  );
  $defaults = fieldhelp_instance_defaults($minimal_instance);
  $stripped = array_diff_assoc_recursive($instance, $defaults);
  // Pass back in the minimal instance definition.
  return array_merge($minimal_instance, $stripped);
}

/**
 * Provides the defaults which field_create_instance() provides.
 *
 * This code is verbatim from _field_write_instance().
 *
 * Takes a minimalist instance array with field_name, entity_type, and bundle.
 *
 * Returns a complete instance array.
 */
function fieldhelp_instance_defaults($instance) {
  $field = field_read_field($instance['field_name']);
  $field_type = field_info_field_types($field['type']);

  // Set defaults.
  $instance += array(
    'settings' => array(),
    'display' => array(),
    'widget' => array(),
    'required' => FALSE,
    'label' => $instance['field_name'],
    'description' => '',
    'deleted' => 0,
  );

  // Set default instance settings.
  $instance['settings'] += field_info_instance_settings($field['type']);

  // Set default widget and settings.
  $instance['widget'] += array(
    // TODO: what if no 'default_widget' specified ?
    'type' => $field_type['default_widget'],
    'settings' => array(),
  );
  // If no weight specified, make sure the field sinks at the bottom.
  if (!isset($instance['widget']['weight'])) {
    $max_weight = field_info_max_weight($instance['entity_type'], $instance['bundle'], 'form');
    $instance['widget']['weight'] = isset($max_weight) ? $max_weight + 1 : 0;
  }
  // Check widget module.
  $widget_type = field_info_widget_types($instance['widget']['type']);
  $instance['widget']['module'] = $widget_type['module'];
  $instance['widget']['settings'] += field_info_widget_settings($instance['widget']['type']);

  // Make sure there are at least display settings for the 'default' view mode,
  // and fill in defaults for each view mode specified in the definition.
  $instance['display'] += array(
    'default' => array(),
  );
  foreach ($instance['display'] as $view_mode => $display) {
    $display += array(
      'label' => 'above',
      'type' => isset($field_type['default_formatter']) ? $field_type['default_formatter'] : 'hidden',
      'settings' => array(),
    );
    if ($display['type'] != 'hidden') {
      $formatter_type = field_info_formatter_types($display['type']);
      $display['module'] = $formatter_type['module'];
      $display['settings'] += field_info_formatter_settings($display['type']);
    }
    // If no weight specified, make sure the field sinks at the bottom.
    if (!isset($display['weight'])) {
      $max_weight = field_info_max_weight($instance['entity_type'], $instance['bundle'], $view_mode);
      $display['weight'] = isset($max_weight) ? $max_weight + 1 : 0;
    }
    $instance['display'][$view_mode] = $display;
  }

  return $instance;
}

/**
 * Computes the difference of arrays with index check, recursively.
 */
function array_diff_assoc_recursive($array1, $array2) {
  $difference = array();
  foreach ($array1 as $key => $value) {
    if (is_array($value)) {
      if (!isset($array2[$key]) || !is_array($array2[$key])) {
        $difference[$key] = $value;
      }
      else {
        $new_diff = array_diff_assoc_recursive($value, $array2[$key]);
        if ($new_diff != FALSE) {
          $difference[$key] = $new_diff;
        }
      }
    }
    elseif (!isset($array2[$key]) || $array2[$key] != $value) {
      $difference[$key] = $value;
    }
  }
  return $difference;
}
