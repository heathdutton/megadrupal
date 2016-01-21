<?php

/**
 * @file
 * Integrates Field Instance Sync with Features.
 */

/**
 * Implements hook_features_export_options().
 */
function field_instance_sync_features_export_options() {
  module_load_include('inc', 'features', 'includes/features.field');

  return field_instance_features_export_options();
}

/**
 * Implements hook_features_export().
 */
function field_instance_sync_features_export($data, &$export, $module_name) {
  // Add field instances to the pipe, these need to be exported too. The field
  // instance export will take care of adding the base fields.
  $pipe = array('field_instance' => array());

  // Add a dependency on the Field Instance Sync module.
  $export['dependencies']['field_instance_sync'] = 'field_instance_sync';

  foreach ($data as $identifier) {
    // Add the component to the export and add the field instance to the pipe.
    $export['features']['field_instance_sync'][$identifier] = $identifier;
    $pipe['field_instance'][] = $identifier;
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function field_instance_sync_features_export_render($module, $data, $export = NULL) {
  $code = array();

  $code[] = '  $field_data = array();';
  $code[] = '';

  foreach ($data as $identifier) {
    if ($instance = features_field_instance_load($identifier)) {
      $export = array();
      $export['synced'] = field_instance_sync_is_synced($instance);
      if ($export['synced']) {
        $export['slave'] = !field_instance_sync_is_master_instance($instance);
      }

      $rendered_export = features_var_export($export, '  ');
      $instance_identifier = features_var_export($identifier);
      $code[] = "  \$field_data[{$instance_identifier}] = {$rendered_export};";

      $pipe['field_instance'][] = $instance['field_name'];
    }
  }
  $code[] = '';
  $code[] = '  return $field_data;';
  $code = implode("\n", $code);

  return array('field_instance_sync_defaults' => $code);
}

/**
 * Implements hook_features_revert().
 */
function field_instance_sync_features_revert($module) {
  field_instance_sync_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function field_instance_sync_features_rebuild($module) {
  if ($defaults = features_get_default('field_instance_sync', $module)) {
    // Load the master bundle settings so we can update it if needed.
    $master_bundles = variable_get('field_instance_sync_master_bundles', array());

    // Load all the existing instances up front so that we don't have to rebuild
    // the cache all the time.
    field_info_cache_clear();
    $field_instances = field_info_instances();

    foreach ($defaults as $identifier => $default_data) {
      list($entity_type, $bundle, $field_name) = explode('-', $identifier);
      // Only continue if the base field and field instance exist.
      if (field_info_field($field_name) && isset($field_instances[$entity_type][$bundle][$field_name])) {
        // Update the field instance with the synchronisation settings.
        $field_instance = $field_instances[$entity_type][$bundle][$field_name];
        $field_instance['settings']['field_instance_sync'] = $default_data['synced'];
        if ($field_instance !== $field_instances[$entity_type][$bundle][$field_name]) {
          try {
            field_update_instance($field_instance);
          }
          catch (FieldException $e) {
            watchdog('features', 'Attempt to update field instance %label (in %entity entity type %bundle bundle) failed: %message', array('%label' => $field_instance['field_name'], '%entity' => $field_instance['entity_type'], '%bundle' => $field_instance['bundle'], '%message' => $e->getMessage()), WATCHDOG_ERROR);
          }
        }

        // Update the master bundle settings.
        if (!$default_data['slave']) {
          $master_bundles[$entity_type][$field_name] = $bundle;
        }
      }
    }
    // Store the updated master bundle settings.
    variable_set('field_instance_sync_master_bundles', $master_bundles);
  }
}
