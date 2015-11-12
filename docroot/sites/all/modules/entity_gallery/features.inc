<?php

/**
 * @file
 * Helper functions for features.module integration.
 */

/**
 * Implements hook_features_export_options().
 */
function entity_gallery_features_export_options() {
  return entity_gallery_type_get_names();
}

/**
 * Implements hook_features_export.
 */
function entity_gallery_features_export($data, &$export, $module_name = '') {
  $pipe = array();
  $map = features_get_default_map('entity_gallery');

  foreach ($data as $type) {
    // Poll entity gallery module to determine who provides the entity gallery
    // type.
    if ($info = entity_gallery_type_get_type($type)) {
      // If this entity gallery type is provided by a different module, add it
      // as a dependency
      if (isset($map[$type]) && $map[$type] != $module_name) {
        $export['dependencies'][$map[$type]] = $map[$type];
      }
      // Otherwise export the entity gallery type.
      elseif (in_array($info->base, array('entity_gallery_content', 'features'))) {
        $export['features']['entity_gallery'][$type] = $type;
        $export['dependencies']['entity_gallery'] = 'entity_gallery';
        $export['dependencies']['features'] = 'features';
      }

      $fields = field_info_instances('entity_gallery', $type);
      foreach ($fields as $name => $field) {
        $pipe['field_instance'][] = "entity-gallery-{$field['bundle']}-{$field['field_name']}";
      }
    }
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function entity_gallery_features_export_render($module, $data, $export = NULL) {
  $elements = array(
    'name' => TRUE,
    'base' => FALSE,
    'description' => TRUE,
    'has_title' => FALSE,
    'title_label' => TRUE,
    'help' => TRUE,
  );
  $output = array();
  $output[] = '  $items = array(';
  foreach ($data as $type) {
    if ($info = entity_gallery_type_get_type($type)) {
      // Force module name to be 'features' if set to 'entity_gallery'. If we
      // leave as 'entity_gallery' the entity gallery type will be assumed to be
      // database-stored by the entity gallery module.
      $info->base = ($info->base === 'entity_gallery') ? 'features' : $info->base;
      $output[] = "    '{$type}' => array(";
      foreach ($elements as $key => $t) {
        if ($t) {
          $text = str_replace("'", "\'", $info->$key);
          $text = !empty($text) ? "t('{$text}')" : "''";
          $output[] = "      '{$key}' => {$text},";
        }
        else {
          $output[] = "      '{$key}' => '{$info->$key}',";
        }
      }
      $output[] = "    ),";
    }
  }
  $output[] = '  );';
  $output[] = '  drupal_alter(\'entity_gallery_info\', $items);';
  $output[] = '  return $items;';
  $output = implode("\n", $output);
  return array('entity_gallery_info' => $output);
}

/**
 * Implements hook_features_revert().
 *
 * @param $module
 * name of module to revert gallery for
 */
function entity_gallery_features_revert($module = NULL) {
  if ($default_types = features_get_default('entity_gallery', $module)) {
    foreach ($default_types as $type_name => $type_info) {
      // Delete entity gallery types
      // We don't use entity_gallery_type_delete() because we do not actually
      // want to delete the entity gallery type (and invoke
      // hook_entity_gallery_type()).
      // This can lead to bad consequences like CCK deleting field
      // storage in the DB.
      db_delete('entity_gallery_type')
        ->condition('type', $type_name)
        ->execute();
    }
    entity_gallery_types_rebuild();
    menu_rebuild();
  }
}

/**
 * Implements hook_features_disable_feature().
 *
 * When a features module is disabled, modify any entity gallery types it
 * provides so they can be deleted manually through the gallery types UI.
 *
 * @param $module
 *   Name of module that has been disabled.
 */
function entity_gallery_features_disable_feature($module) {
  if ($default_types = features_get_default('entity_gallery', $module)) {
    foreach ($default_types as $type_name => $type_info) {
      $type_info = entity_gallery_type_load($type_name);
      $type_info->module = 'entity_gallery';
      $type_info->custom = 1;
      $type_info->modified = 1;
      $type_info->locked = 0;
      $type_info->disabled = 0;
      entity_gallery_type_save($type_info);
    }
  }
}

/**
 * Implements hook_features_enable_feature().
 *
 * When a features module is enabled, modify any entity gallery types it
 * provides so they can no longer be deleted manually through the gallery types
 * UI.
 *
 * @param $module
 *   Name of module that has been enabled.
 */
function entity_gallery_features_enable_feature($module) {
  if ($default_types = features_get_default('entity_gallery', $module)) {
    foreach ($default_types as $type_name => $type_info) {
      // Ensure the type exists.
      if ($type_info = entity_gallery_type_load($type_name)) {
        $type_info->module = $module;
        $type_info->custom = 0;
        $type_info->modified = 0;
        $type_info->locked = 1;
        $type_info->disabled = 0;
        entity_gallery_type_save($type_info);
      }
    }
  }
}
