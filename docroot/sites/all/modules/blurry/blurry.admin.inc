<?php

/**
 * Form builder for a settings page.
 */
function blurry_settings_form() {
  $form = array();
  $node_types = node_type_get_names();

  foreach ($node_types as $type => $name) {
    $fields = _blurry_get_node_fields($type);

    $form['node_type'][$type] = array(
      '#type' => 'fieldset',
      '#title' => $name,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['node_type'][$type]['blurry_' . $type . '_fields'] = array(
      '#type' => 'checkboxes',
      '#options' => $fields,
      '#multiple' => TRUE,
      '#tree' => TRUE,
      '#default_value' => variable_get('blurry_' . $type . '_fields', array()),
    );
  }

  return system_settings_form($form);
}

/**
 * Get textarea fields of given content type.
 */
function _blurry_get_node_fields($type) {
  $fields = field_info_instances('node', $type);
  $text_fields = array();

  foreach ($fields as $field) {
    if ($field['widget']['type'] == 'text_textarea_with_summary' || $field['widget']['type'] == 'text_textarea') {
      $text_fields[$field['field_name']] = $field['label'];
    }
    elseif ($field['widget']['type'] == 'field_collection_embed') {
      $fc_fields = field_info_instances('field_collection_item', $field['field_name']);
      foreach ($fc_fields as $fc_field) {
        if ($fc_field['widget']['type'] == 'text_textarea_with_summary' || $fc_field['widget']['type'] == 'text_textarea') {
          $text_fields[$field['field_name']][$field['field_name'] . '__' . $fc_field['field_name']] = $fc_field['label'];
        }
      }
    }
  }

  return $text_fields ? $text_fields : array();
}