<?php

/**
 * @file
 * Admin settings for comparisons.
 */

/**
 * Admin settings form for comparisons.
 */
function comparison_settings_form() {
  $form = array();

  $form['prefix'] = array(
    '#markup' => t('These settings specify where to add the comparison links.'),
  );

  $options = array();
  foreach (array_keys(node_type_get_types()) as $type) {
    $options[$type] = $type;
  }
  $node_types = variable_get('comparison_node_types', array());
  $form['comparison_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#options' => $options,
    '#default_value' => $node_types,
  );
  $entity_info = entity_get_info('node');
  $options = array();
  foreach (array_keys(($entity_info['view modes'])) as $mode) {
    $options[$mode] = $mode;
  }
  $view_modes = variable_get('comparison_view_modes', array('teaser'));
  $form['comparison_view_modes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('View modes'),
    '#options' => $options,
    '#default_value' => $view_modes,
  );
  $fields = field_info_field_map();
  $options = array();
  foreach ($fields as $key => $field) {
    if ($field['type'] == 'image') {
      $options[$key] = $key;
    }
  }
  $fields = variable_get('comparison_fields', '');
  $form['comparison_fields'] = array(
    '#type' => 'radios',
    '#title' => t('Image fields'),
    '#description' => t('Field to use in header of comparison table.'),
    '#options' => $options,
    '#default_value' => $fields,
  );
  $image_style = variable_get('image_style', '');
  $form['image_style'] = array(
    '#type' => 'textfield',
    '#title' => t('Image style'),
    '#description' => t('Image style to use to display image in table header.'),
    '#default_value' => $image_style,
  );
  $html = variable_get('corner_html', '');
  $form['corner_html'] = array(
    '#type' => 'textarea',
    '#title' => t('Corner html'),
    '#description' => t('HTML to display in the upper left corner of table'),
    '#default_value' => $html,
  );
  return system_settings_form($form);
}
