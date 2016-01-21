<?php
/**
 * @file
 * Provide default settings for all bundles.
 */

/**
 * Implements hook_default_entity_recur_settings_alter().
 */
function entity_recur_default_entity_recur_settings_alter(&$settings) {
  $defaults = array(
    'enabled' => FALSE,
    'allow_past' => FALSE,
    'max_duration' => '0',
    'form' => FALSE,
    'batch_limit' => 50,
  );
  $info = entity_get_info();
  foreach ($info as $entity_type => $entity_info) {
    if (!$entity_info['fieldable']) {
      continue;
    }
    foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
      foreach ($settings as $setting) {
        if ($setting->entity_type == $entity_type && $setting->bundle == $bundle) {
          continue 2;
        }
      }
      // Determine the available date fields on this bundle
      $fields = array();
      $instances = field_info_instances($entity_type, $bundle);
      foreach ($instances as $name => $field) {
        if ($field['widget']['module'] == 'date') {
          $fields[] = $name;
        }
      }
      if (empty($fields)) {
        continue;
      }

      $settings["{$entity_type}_{$bundle}"] = (object)(array(
        'name' => "{$entity_type}_{$bundle}",
        'label' => "{$entity_info['label']} {$bundle_info['label']}",
        'entity_type' => $entity_type,
        'bundle' => $bundle,
        'date_field' => reset($fields),
      ) + $defaults);
    }
  }
  foreach ($settings as &$setting) {
    $setting->disabled = !$setting->enabled;
  }
}

