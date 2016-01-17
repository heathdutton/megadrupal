<?php

/**
 * @file
 * 
 * Field weight admin settings form.
 * 
 */
function field_weight_settings_form() {
  $form = array();

  $node_types = array_map('check_plain', node_type_get_names());

  $form['field_weight']['node_types']['field_weight_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled content types'),
    '#description' => t('Content (node) types that field display weights can be used on. This will add an additional tab to nodes.'),
    '#options' => $node_types,
    '#default_value' => variable_get('field_weight_node_types', array()),
    '#multiple' => TRUE,
  );

  return system_settings_form($form); 
}
