<?php

/**
 * @file
 * Administration functions for the CiviCRM Content Type Groups module.
 */

/**
 * Form constructor for the settings form.
 */
function civicrm_content_types_settings_form() {
  $defaults = variable_get('civicrm_content_types_settings');

  $form['civicrm_content_types_settings']['#tree'] = TRUE;
  $form['civicrm_content_types_settings']['#type'] = 'vertical_tabs';

  $form['civicrm_content_types_settings']['types'] = array(
    '#title' => t('Content types'),
    '#type' => 'fieldset',
    '#description' => t('Select which content types are to be mirrored in CiviCRM.'),
  );

  $types = db_query("SELECT type, name FROM {node_type} WHERE disabled = :disabled", array(':disabled' => 0));
  foreach ($types as $type) {
    $form['civicrm_content_types_settings']['types'][$type->type] = array(
      '#title' => check_plain($type->name),
      '#type' => 'checkbox',
      '#default_value' => (isset($defaults['types'][$type->type])) ? $defaults['types'][$type->type] : 0,
    );
  }

  $form['#submit'][] = 'civicrm_content_types_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Form submission handler for civicrm_content_types_settings_form().
 */
function civicrm_content_types_settings_form_submit(&$form, &$form_state) {
  $old_values = variable_get('civicrm_content_types_settings', array());

  foreach ($form_state['values']['civicrm_content_types_settings']['types'] as $type => $new_value) {
    $old_value = $old_values['types'][$type];

    if ($old_value == 0 && $new_value == 1) {
      // The content type has been enabled.
      civicrm_content_types_enable($type);
    }
    elseif ($old_value == 1 && $new_value == 0) {
      // The content type has been disabled.
      civicrm_content_types_disable($type);
    }
  }
}

/**
 * Mirrors a content type with CiviCRM.
 *
 * @param string $type
 *   The machine-readable name of a content type.
 */
function civicrm_content_types_enable($type) {
  module_load_include('inc', 'civicrm_content_types', 'includes/core');

  $ctg = civicrm_content_types_get($type);

  // TODO: Use the readable content type name instead of the machine name.
  // $name = $type;
  $name = civicrm_content_types_get_name($type);

  $description = ($ctg['description']) ? $ctg['description'] : NULL;

  if ($ctg['gid']) {
    $gid = $ctg['gid'];
    civicrm_content_types_group_write($name, $description, $gid);
  }
  else {
    $gid = civicrm_content_types_group_write($name, $description);
    $record = array('type' => $type, 'gid' => $gid->id);
    drupal_write_record('civicrm_content_types', $record);
  }
}

/**
 * Prevents a content type from mirroring with CiviCRM.
 *
 * @param string $type
 *   The machine-readable name of a content type.
 */
function civicrm_content_types_disable($type) {
  module_load_include('inc', 'civicrm_content_types', 'includes/core');
  civicrm_content_types_link_delete($type, TRUE);
}
