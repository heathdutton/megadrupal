<?php
/**
 * @file
 * Provides settings form.
 *
 * @author Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Form builder for the settings form.
 *
 * @todo
 * Add help text explaining these settings take effect when saving an
 * entity to convert paths to portable paths.
 * Consider offering granularity down to entity type and bundle.
 * Use ctools_dependent form element to get bundles with selected field types.
 */
function portable_path_settings_form($form, &$form_state) {
  // Get information.
  $settings = portable_path_config();
  $bundles = field_info_bundles($entity_type = NULL);
  $fields = field_info_fields();
  $instances = field_info_instances($entity_type = NULL, $bundle_name = NULL);
  $text_field_types = portable_path_text_field_types();
  $text_field_names = portable_path_text_field_names(array_keys($text_field_types));
  $help = '';
  if (!module_exists('help')) {
    $help = t('In the text fields specified below, URLs of images or files that match the indicated stream wrappers will be converted
      to a portable format on entity save. The reverse will happen on entity edit and view. The filters applied are defined on
      the !format-settings page.',
      array('!format-settings' => l('text formats', 'admin/config/content/formats')));
  }

  // Build form elements.
  $form['portable_path_help'] = array(
    '#markup' => $help,
    '#visible' => !module_exists('help'),
  );
  $form['portable_path_stream_wrappers'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Stream wrappers to convert'),
    '#options' => array('public' => t('public'), 'private' => t('private')),
    '#default_value' => $settings['stream_wrappers'],
  );
  $form['portable_path_field_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Field types to convert'),
    '#options' => $text_field_types,
    '#default_value' => $settings['field_types'],
  );
  $form['portable_path_field_names'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Field names to convert'),
    '#options' => $text_field_names,
    '#default_value' => $settings['field_names'],
  );/*
  $form['portable_path_entity_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Entity types to convert'),
    '#options' => drupal_map_assoc(array(t('node'))),
    '#default_value' => $settings['entity_types'],
  );
  $form['portable_path_bundles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Bundles to convert'),
    '#options' => drupal_map_assoc(array(t('node'))),
    '#default_value' => $settings['bundles'],
  );*/
  return system_settings_form($form);
}

/**
 * Returns a list of known text field types.
 */
function portable_path_text_field_types() {
  // Core text field types.
  $known_types = array('text', 'text_long', 'text_with_summary');
  // @todo Add text fields from popular contrib modules.

  // Filter the field types.
  $text_field_types = array();
  $types = field_info_field_types();
  foreach ($types as $type => $info) {
    if (!in_array($type, $known_types)) {
      continue;
    }
    $text_field_types[$type] = $info['label'];
  }
  return $text_field_types;
}

/**
 * Returns a list of known text field names.
 */
function portable_path_text_field_names($types) {
  // Filter the field names.
  $text_field_names = array();
  $fields = field_info_fields();
  foreach ($fields as $field => $info) {
    if (isset($info['type']) && in_array($info['type'], $types)) {
      $text_field_names[$field] = $info['field_name'];
    }
  }
  return $text_field_names;
}
