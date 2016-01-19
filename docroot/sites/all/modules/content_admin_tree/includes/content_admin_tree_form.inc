<?php
/**
 * @file
 * A file which contains the configuration form
 * for the module.
 */

/**
 * Content adminstration config form callback.
 */
function content_admin_tree_form($form, &$form_state) {
  // Create the fieldset for node types.
  $form['node_types_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node Types'),
    '#description' => t('Select node types to be displayed.'),
    '#collapsible' => FALSE,
  );
  // Get the node types.
  $node_types_rebuild = _node_types_build($rebuild = TRUE);
  // Get the array keyed by type with the value of node type name.
  $node_types = $node_types_rebuild->names;
  // Create the fields for the node types.
  foreach ($node_types as $type => $name) {
    // Create the initial value for the drupal variable.
    $default_value = variable_get('content_admin_tree_' . $type);
    if ($default_value === NULL) {
      $default_value = 1;
      variable_set('content_admin_tree_' . $type, $default_value);
    }
    $form['node_types_fieldset']['content_admin_tree_' . $type] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($name),
      '#default_value' => variable_get('content_admin_tree_' . $type),
    );
    // Add custom submit handler for rebuilding the menu data cache.
    $form['#submit'][] = 'content_admin_tree_submit_handler';
  }
  return system_settings_form($form);
}

/**
 * Submit handler for clearing menu data cache.
 */
function content_admin_tree_submit_handler($form, &$form_state) {
  // Rebuild Content Admin Tree Menu when form submitted.
  if (!empty($form_state['values'])) {
    ctools_include('object-cache');
    ctools_object_cache_clear('content_admin_tree', 'mla');
  }
}
