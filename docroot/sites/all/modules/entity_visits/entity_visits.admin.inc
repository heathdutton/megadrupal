<?php
/**
 * @file
 * Admin settings.
 */

/**
 * Reminder settings.
 */
function entity_visits_admin_settings() {
  $form = array();

  foreach (entity_get_info() as $type => $info) {
    if ($info['view modes'] && $info['bundles']) {
      $form[$type] = array(
        '#type' => 'fieldset',
        '#title' => $info['label'],
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      foreach ($info['bundles'] as $bundle_name => $bundle) {
        $form[$type]["entity_visits_{$type}_{$bundle_name}"] = array(
          '#title' => $bundle['label'],
          '#type' => 'checkboxes',
          '#options' => _entity_visits_prepare_options($info['view modes']),
          '#default_value' => variable_get("entity_visits_{$type}_{$bundle_name}", array()),
        );
      }
    }
  }

  return system_settings_form($form);
}

/**
 * Helper to prepare options array.
 *
 * @param $data
 *
 * @return array
 */
function _entity_visits_prepare_options($data) {
  $options = array();

  foreach ($data as $name => $mode) {
    $options[$name] = $mode['label'];
  }

  return $options;
}
