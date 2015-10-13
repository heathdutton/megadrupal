<?php
/**
 * @file
 * Field creation functions.
 */

define('PM_FIELD_BASE_CREATE_FAILURE', 0);
define('PM_FIELD_BASE_CREATE_SUCCESS', 1);
define('PM_FIELD_BASE_UPDATE_SUCCESS', 2);

define('PM_FIELD_INSTANCE_CREATE_FAILURE', 0);
define('PM_FIELD_INSTANCE_CREATE_SUCCESS', 1);
define('PM_FIELD_INSTANCE_UPDATE_SUCCESS', 2);

/**
 * Iterate through an array of field bases, creating if not present.
 *
 * @param array $field_bases
 *   Array of field bases to check.
 *
 * @return array $result
 *   Array of success / failure flags.
 */
function pm_field_bases_create_if_required(array $field_bases) {
  $result = array();
  if (!empty($field_bases) AND is_array($field_bases)) {
    foreach ($field_bases as $key => $field_base) {
      $field_name = $field_base['field_name'];
      $result[$key] = pm_field_base_create_if_required($field_name, $field_base);
    }
  }
  return $result;
}

/**
 * Create specific field base if not present.
 */
function pm_field_base_create_if_required($field_name, $field_base) {
  try {
    $field = field_info_field($field_name);
    if (empty($field)) {
      $field = field_create_field($field_base);
      return PM_FIELD_BASE_CREATE_SUCCESS;
    }
    else {
      field_update_field($field_base);
      return PM_FIELD_BASE_UPDATE_SUCCESS;
    }
  }
  catch (Exception $e) {
    watchdog(
      'pm',
      'See ' . __FUNCTION__ . '() ' . $e->getTraceAsString(),
      NULL, WATCHDOG_ERROR
    );
  }

  return PM_FIELD_BASE_CREATE_FAILURE;
}

/**
 * Iterate through an array of field instances, creating if not present.
 *
 * @param array $field_instances
 *   Array of field bases to check.
 *
 * @return array $result
 *   Array of success / failure flags.
 */
function pm_field_instances_create_if_required(array $field_instances) {
  $result = array();
  if (!empty($field_instances) AND is_array($field_instances)) {
    foreach ($field_instances as $key => $field_instance) {
      $field_name = $field_instance['field_name'];
      $result[$key] = pm_field_instance_create_if_required($field_name, $field_instance);
    }
  }
  return $result;
}

/**
 * Create specific field instance if not present.
 */
function pm_field_instance_create_if_required($field_name, $field_instance) {

  $field_name = $field_instance['field_name'];
  $entity_type = $field_instance['entity_type'];
  $bundle_name = $field_instance['bundle'];
  $field = field_info_field($field_name);

  $entity_type = $field_instance['entity_type'];

  // Create only if field is defined.
  try {
    if ($field) {
      // Try to load field_instance if present.
      $instance_info = field_info_instance($entity_type, $field_name, $bundle_name);

      // Create ony if instance is not present.
      if (empty($instance_info)) {
        field_create_instance($field_instance);
        return PM_FIELD_INSTANCE_CREATE_SUCCESS;
      }
      else {
        // Update field_instance if already present.
        $merged_settings  = $field_instance + $instance_info;
        field_update_instance($merged_settings);
        return PM_FIELD_INSTANCE_UPDATE_SUCCESS;
      }
    }
  }
  catch (Exception $e) {
    watchdog(
      'pm',
      'See ' . __FUNCTION__ . '() ' . $e->getTraceAsString(),
      NULL, WATCHDOG_ERROR
    );
  }
  return PM_FIELD_INSTANCE_CREATE_FAILURE;
}
