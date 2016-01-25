<?php

/**
 * Admin form.
 */
function sky_field_admin_settings_form($form, &$form_state) {
  $entities = entity_get_info();
  $entity_options = array();
  foreach ($entities as $key => $item) {
    $entity_options[$key] = $item['label'];
  }
  $form['sky_field_entities'] = array(
    '#title' => t('Entity types'),
    '#type' => 'checkboxes',
    '#options' => $entity_options,
    '#default_value' => variable_get('sky_field_entities', array()),
    '#description' => t('Choose entities to use sky fields'),
  );
  // Get node types options.
  $options = sky_field_node_types_options();
  // Node fields node types.
  $form['sky_field_node_types'] = array(
    '#title' => t('Content types'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => variable_get('sky_field_node_types', array()),
    '#description' => t('Check "node" entity type, then choose content types to use sky fields'),
  );
  $form = system_settings_form($form);
  $form['#submit'][] = 'sky_field_admin_settings_form_submit';
  return $form;
}

function sky_field_admin_settings_form_submit($form, &$form_state) {
  menu_rebuild();
}

/**
 * Get node types options.
 *
 * @return
 *   Associative array of options.
 */
function sky_field_node_types_options() {
  $options = array();
  // Populate node types options.
  $node_field_types = node_type_get_types();
  foreach ($node_field_types as $node_field_type) {
    $options[$node_field_type->type] = $node_field_type->name;
  }
  return $options;
}
