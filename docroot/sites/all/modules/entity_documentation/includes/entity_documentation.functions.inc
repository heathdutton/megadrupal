<?php

/**
 * @file
 * Functions file for Entity Documentation module.
 */

/**
 * Get entity types function.
 *
 * @return array
 *   Entity types as an array.
 */
function ed_get_entity_types() {

  // Invoke hook to get entity types.
  $types = array();
  $types = module_invoke_all('ed_type', $types);

  return $types;
}

/**
 * Get exporters.
 *
 * @return array
 *   Exporters as an array.
 */
function ed_get_exporters() {

  // Invoke hook to get exporters.
  $exporters = array();
  $exporters = module_invoke_all('ed_exporter', $exporters);

  return $exporters;
}

/**
 * Get array documentation for specific bundle.
 *
 * @param $entity
 *   Entity type.
 * @param $bundle
 *   Bundle to get array for.
 *
 * @return array
 *   Array with the documentation.
 */
function ed_bundle_array($entity, $bundle) {

  // Invoke hook to get array.
  $documentation_array = array();
  $documentation_array = module_invoke_all('ed_bundle_documentation', $documentation_array, $entity, $bundle);

  return $documentation_array;
}

/**
 * Get auto export settings.
 *
 * @return array
 *   Auto export settings.
 */
function ed_get_auto_export() {
  $auto_export_settings = variable_get('ed_auto_export');
  if (strlen($auto_export_settings) < 1) {
    return array();
  }
  return unserialize($auto_export_settings);
}

/**
 * Get if auto export is enabled for this bundle.
 *
 * @param $entity
 *   Entity type.
 * @param $bundle
 *   Bundle type.
 * @param $exporter
 *   Exporter type.
 *
 * @return bool
 *   If auto export is enabled.
 */
function ed_get_auto_export_enabled($entity, $bundle, $exporter) {

  // Get settings.
  $auto_export_settings = ed_get_auto_export();

  $key = $entity . '_' . $bundle . '_' . $exporter;
  if (array_key_exists($key, $auto_export_settings) && $auto_export_settings[$key]) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Process auto export.
 *
 * @param $force
 *   Force file creation even if it hasn't passed its time interval.
 */
function ed_auto_export($force = FALSE) {

  // Check and create directory.
  $dir = ed_check_directory();

  // If directory still does not exist, log error.
  if ($dir) {
    // Get documentation to be exported.
    $types = ed_get_entity_types();
    $exporters = ed_get_exporters();
    $auto_export_settings = ed_get_auto_export();

    foreach ($types as $entity_key => $entity) {
      foreach ($entity['bundles'] as $bundle_key => $bundle) {
        foreach ($exporters as $exporter_key => $exporter) {
          $key = $entity_key . '_' . $bundle_key . '_' . $exporter_key;
          $file = $dir . DIRECTORY_SEPARATOR . $entity_key . '-' . $bundle_key . '.' . $exporter_key;
          if (array_key_exists($key, $auto_export_settings) && $auto_export_settings[$key]) {
            entity_documentation_file_export($exporter_key, $entity_key, $bundle_key, $file, $force);
          }
        }
      }
    }
  }
  else {
    // Log error.
    watchdog('entity_documentation', 'Directory is not writtable.', WATCHDOG_ERROR);
  }
}

/**
 * Check and create export directory.
 *
 * @return string or FALSE
 *   Directory path or FALSE on failure.
 */
function ed_check_directory() {

  // Create directory.
  $dir = variable_get('ed_export_path',
    variable_get('file_public_path', 'sites/default/files') . '/entity_documentation');
  if (substr($dir, 0, 1) !== '/') {
    $dir = drupal_realpath('.') . '/' . $dir;
  }
  if (!file_prepare_directory($dir)) {
    drupal_mkdir($dir);
  }

  // Check again if directory exists.
  if (!file_prepare_directory($dir)) {
    return FALSE;
  }

  return $dir;
}

/**
 * Export documentation of specific bundle in file.
 *
 * @param $exporter
 *   Exporter to use.
 * @param $entity
 *   Entity type.
 * @param $bundle
 *   Bundle to export.
 * @param $force
 *   Force file creation even if it hasn't passed its time interval.
 */
function entity_documentation_file_export($exporter, $entity, $bundle, $file, $force = FALSE) {
  $update_interval = variable_get('ed_update_interval');

  if (!file_exists($file)
    || (file_exists($file) && (time() - filemtime($file)) > $update_interval)
    || $force) {
    // Invoke hooks for file export.
    module_invoke_all('ed_documentation_file_export', $exporter, $entity, $bundle, $file);
  }
}
