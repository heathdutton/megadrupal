<?php

/**
 * Admin form.
 */
function node_field_admin_settings_form($form, &$form_state) {
  // Get node types options.
  $options = node_field_node_types_options();
  // Node fields node types.
  $form['node_field_node_types'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => variable_get('node_field_node_types', array()),
    '#description' => t('Choose content types to use node fields'),
  );
  // Return default system settings form.
  $form = system_settings_form($form);
  return $form;
}

/**
 * Get node types options.
 *
 * @return
 *   Associative array of options.
 */
function node_field_node_types_options() {
  $options = array();
  // Populate node types options.
  $node_field_types = node_type_get_types();
  foreach ($node_field_types as $node_field_type) {
    $options[$node_field_type->type] = $node_field_type->name;
  }
  return $options;
}
