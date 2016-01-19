<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function node_display_title_admin_page() {
  $output = 'Node admin title admin page.';
  return $output;
}

/**
 * Get node types
 * 
 * @return array list of machine names of node types.
 */
function _node_display_title_get_node_types_list() {
  $list_of_types = array();
  $node_types = node_type_get_types();
  foreach ($node_types as $node_type_object) {
    $list_of_types[] = $node_type_object->type;
  }
  return $list_of_types;
}

/**
 * Settings form function
 */
function node_display_title_list_form($form, &$form_state) {
  // Get list of node types and whether they use admin titles.
  $node_types = _node_display_title_get_node_types_list();
  $settings = variable_get('node_display_title_settings', array());
  $checkbox_options = array();
  $default_values = array();
  
  $form[] = array(
    '#markup' => '<p>' . t('Set which node types should use admin titles.') . '</p>',
  );

  
  foreach ($node_types as $type) {
    $checkbox_options[] = $type;
    if (in_array($type, $settings)) {
      $default_values[] = $type;
    }
  }

  $form['node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select node types'),
    '#options' => drupal_map_assoc($checkbox_options),
    '#default_value' => $default_values,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('save'),
  );

  return $form;
}

/**
 * Settings form submmit function
 */
function node_display_title_list_form_submit($form, &$form_state) {
  $node_display_title_settings = array();
  $instance_options = array();
  $field_instances_by_node_type = field_info_instances('node');
  $field_name = 'field_node_display_title';
  
  // Check each node type and add or remove display field instances accordingly.
  foreach ($field_instances_by_node_type as $type => $instance) {

    if (!empty($form_state['values']['node_types'][$type])) {
      // Get weight of title.
      $extra_fields = field_info_extra_fields('node', $type, 'form');
      $title_field_weight = $extra_fields['title']['weight'];
      
      // Move all fields below title down one position.
  
      // Define field instance options.
      $instance_options = array(
        'field_name' => $field_name,
        'entity_type' => 'node',
        'bundle' => $type,
        'label' => t('Display title'),
        'widget' => array(
          // Position new field directly below title field.
          'weight' => ++$title_field_weight,
        ),
        'display' => array(
          'default' => array(
            'type' => 'hidden',
          ),
        ),
      );
      
      $node_display_title_settings[] = $type;
      // Add field instance if not already present for given node type.
      if (!isset($instance['field_node_display_title'])) {
        field_create_instance($instance_options);
      }
    }
    else {
      // Remove field instance if currently present.
      if (isset($instance['field_node_display_title'])) {
        field_delete_instance($instance['field_node_display_title'], FALSE);
      }
    }
  }
  variable_set('node_display_title_settings', $node_display_title_settings);
  field_purge_batch(100);
}
