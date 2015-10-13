<?php
/**
 * @file
 * Provide default settings for all bundles.
 */

/**
 * Implements hook_default_entity_popup_settings_alter().
 */
function entity_popup_default_entity_popup_settings_alter(&$settings) {
  $info = entity_get_info();
  foreach ($info as $entity_type => $entity_info) {
    $view_mode_names = array_keys($entity_info['view modes']);
    $default_view_mode = reset($view_mode_names);
    if (isset($entity_info['view modes']['full'])) {
      $default_view_mode = 'full';
    }
    elseif (isset($entity_info['view modes']['default']) || !$default_view_mode) {
      $default_view_mode = 'default';
    }
    foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
      foreach ($settings as $setting) {
        // Setting already exists, skip it.
        if ($setting->entity_type == $entity_type && $setting->bundle == $bundle) {
          continue 2;
        }
      }

      $settings["{$entity_type}_{$bundle}"] = (object)(array(
        'name' => "{$entity_type}_{$bundle}",
        'label' => "{$entity_info['label']} {$bundle_info['label']}",
        'entity_type' => $entity_type,
        'bundle' => $bundle,
        'disabled' => FALSE,
        'view_mode' => $default_view_mode,
      ));
    }
  }
}
