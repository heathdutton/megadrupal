<?php

/**
 * @file
 * Drush integration file for Entity Documentation module.
 */

/**
 * Implements hook_drush_help().
 */
function entity_documentation_drush_help($command) {
  switch ($command) {
    case 'drush:export-documentation':
      return dt('Export Entity Documentation');
  }
}

/**
 * Implements hook_drush_command().
 */
function entity_documentation_drush_command() {
  $items = array();
  $items['export-documentation'] = array(
    'description' => dt('Export Entity Documentation.'),
    'arguments' => array(
      'exporter' => dt('Exporter'),
      'entity' => dt('Entity'),
      'bundle' => dt('Bundle'),
    ),
    'examples' => array(
      'Standard example' => 'drush export-documentation',
      'Argument example' => 'drush export-documentation pdf node article',
    ),
    'aliases' => array('ed'),
  );
  return $items;
}

/**
 * Implements drush_hook_my_command().
 */
function drush_entity_documentation_export_documentation(
    $exporter = NULL, $entity = NULL, $bundle = NULL) {

  // Validate input.
  if (drush_entity_documentation_export_documentation_validate($exporter, $entity, $bundle)) {

    // Check all arguments are null.
    $all_null = $exporter == NULL && $entity == NULL && $bundle == NULL;

    if ($all_null) {
      module_load_include('inc', 'entity_documentation', 'includes/entity_documentation.functions');
      // Process auto export and force update.
      ed_auto_export(TRUE);
      drush_print('Documentation exported. Select documentation to be exported at admin/config/development/entity-documentation/settings.');
    }
    else {
      // Export specific document.
      module_load_include('inc', 'entity_documentation', 'includes/entity_documentation.functions');
      // Check and create directory.
      $dir = ed_check_directory();

      // If directory still does not exist, log error.
      if ($dir) {
        $file = $dir . DIRECTORY_SEPARATOR . $entity . '-' . $bundle . '.' . $exporter;
        entity_documentation_file_export($exporter, $entity, $bundle, $file, TRUE);
        drush_print('Documentation exported.');
      }
      else {
        drush_set_error('Directory is not writtable.');
      }
    }
  }
}

/**
 * Validate drush input.
 *
 * @param $exporter
 *   Exporter type.
 * @param $entity
 *   Entity type.
 * @param $bundle
 *   Bundle type.
 *
 * @return boolean
 *   Validation.
 */
function drush_entity_documentation_export_documentation_validate(
    $exporter = NULL, $entity = NULL, $bundle = NULL) {

  // Check that all arguments are null or none.
  $all_null = $exporter == NULL && $entity == NULL && $bundle == NULL;
  $none_null = $exporter != NULL && $entity != NULL && $bundle != NULL;

  if ($all_null) {
    return TRUE;
  }
  elseif ($none_null) {

    // Get entity types.
    module_load_include('inc', 'entity_documentation', 'includes/entity_documentation.functions');
    $types = ed_get_entity_types();

    // Get exporters.
    $exporters = ed_get_exporters();

    // Check exporter is valid.
    if (!array_key_exists($exporter, $exporters)) {
      drush_set_error('No valid exporter given.');
      return FALSE;
    }

    // Check entity and bundle is valid.
    if (!array_key_exists($entity, $types)) {
      drush_set_error('No valid entity given.');
      return FALSE;
    }
    else {
      // Check bundle is valid.
      $bundle_found = FALSE;
      foreach ($types as $type) {
        foreach ($type['bundles'] as $bundle_key => $bundle_array) {
          if ($bundle_key == $bundle) {
            $bundle_found = TRUE;
            break;

          }
        }
      }

      if (!$bundle_found) {
        drush_set_error('No valid bundle given.');
        return FALSE;
      }
    }
  }
  else {
    drush_set_error('Either pass no arguments or exporter, entity and bundle in that order.');
    return FALSE;
  }

  return TRUE;
}
