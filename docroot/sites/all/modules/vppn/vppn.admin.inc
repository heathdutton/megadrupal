<?php

/**
 * @file
 * Administration functionality for VPPN.
 */

/**
 * Form builder / page callback for the admin config form.
 */
function vppn_config_form($form) {

  // Add a description.
  $form['description'] = array(
    '#type' => 'item',
    '#markup' => t('Select content types that should have the view permission on each node.'),
  );

  // Each content type.
  foreach (node_type_get_types() as $type => $info) {

    // Add a setting for each type.
    $form['vppn_node_' . $type] = array(
      '#type' => 'checkbox',
      '#title' => $info->name,
      '#default_value' => variable_get('vppn_node_' . $type, 0),
    );
  }

  // Return the renderable system settings form.
  return system_settings_form($form);
}
