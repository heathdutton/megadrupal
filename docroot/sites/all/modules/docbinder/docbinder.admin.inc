<?php

/**
 * @file
 * DocBinder admin pages.
 */

/**
 * Settings form
 */
function docbinder_settings_form() {
  $methods = _docbinder_get_available_methods();
  foreach ($methods as $k => $v) {
    $docbinder_zip_method_options[$k] = $v['title'];
  }
  $form['docbinder_zip_method'] = array(
    '#title' => t('Zip Method'),
    '#type' => 'select',
    '#default_value' => variable_get('docbinder_zip_method', ''),
    '#options' => $docbinder_zip_method_options,
    '#description' => t('Method used to generate bundle files.'),
  );

  $form['docbinder_field_formatting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field formatting settings'),
    '#collapsible' => true,
  );

  $form['docbinder_field_formatting']['docbinder_field_display'] = array(
    '#title' => t('File Display'),
    '#type' => 'select',
    '#default_value' => variable_get('docbinder_field_display', 'link'),
    '#options' => array(
      'link' => t('Filename as link'),
      'checkbox' => t('Checkbox')
    ),
    '#description' => t('How to display the DocBinder related links. Checkbox works with enabled javascript, only.'),
  );

  $form['docbinder_field_formatting']['docbinder_remove_hotlink'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove direct links to file.'),
    '#default_value' => variable_get('docbinder_remove_hotlink', 0),
    '#description' => t('Whether or not hotlinks to files (for direct download) should stay or be removed.'),
  );

  $form['docbinder_field_formatting']['docbinder_node_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node types'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#description' => t('Only selected node types will use the DocBinder field formatters. If none is selected, all node types will be affected.'),
  );

  $node_types = node_type_get_types();
  $docbinder_node_types_options = array();
  foreach ($node_types AS $machine_name => $node_type) {
    $docbinder_node_types_options[$machine_name] = $node_type->name;
  }
  $form['docbinder_field_formatting']['docbinder_node_types']['docbinder_node_types_active'] = array(
    '#type' => 'checkboxes',
    '#options' => $docbinder_node_types_options,
    '#default_value' => variable_get('docbinder_node_types_active'),
  );

  return system_settings_form($form);
}
