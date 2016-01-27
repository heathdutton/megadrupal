<?php

/**
 * @file
 * Provides the entity_uuid module admin pages.
 */

/**
 * Configuration settings form.
 */
function entity_uuid_settings_form($form, &$form_state) {
  $setting = variable_get('entity_uuid', array());
  $form['entity_uuid'] = array(
    '#tree' => TRUE,
  );

  $info = entity_get_info();
  foreach ($info as $entity_type => $value) {
    // Set default value and disabled for checkbox.
    $disabled = isset($value['uuid']) ? $value['uuid'] : FALSE;
    $default_value = isset($setting[$entity_type]) ? ($setting[$entity_type] || $disabled) : $disabled;

    $form['entity_uuid'][$entity_type] = array(
      '#type' => 'checkbox',
      '#disabled' => $disabled,
      '#title' => check_plain($value['label']),
      '#default_value' => $default_value,
    );
  }

  $form = system_settings_form($form);
  $form['#submit'][] = 'entity_uuid_apply_config';

  return $form;
}

/**
 * Execute the entity_uuid_settings_form.
 */
function entity_uuid_apply_config($form, &$form_state) {
  // Rebuild entity_info cache.
  entity_info_cache_clear();
  entity_get_info();
  // Makes sure that the schema for all entity types, which set in config is
  // set correct.
  entity_uuid_install_uuid_fields();
}
