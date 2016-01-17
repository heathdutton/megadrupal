<?php
/**
 * @file
 * Variable module integration.
 */

/**
 * Implements hook_element_info()
 */
function config_builder_element_info() {
  $types = array();

  // A variable form element field
  $types['variable_form_element'] = array(
    '#input' => TRUE,
    '#variable' => NULL,
    '#process' => array('config_builder_element_process_variable_form_element', 'ajax_process_form'),
    '#theme_wrappers' => array('form_element'),
  );

  return $types;
}

/**
 * Process variable form element
 */
function config_builder_element_process_variable_form_element($element) {
  $element += variable_edit_subform($element['#variable']);
  return $element;
}

/**
 * Implements hook_config_builder_module_implements_alter() on behalf of
 * variable.module.
 */
function variable_config_builder_module_implements_alter(&$implementations, $hook) {
  if ('variable_info_alter' == $hook) {
    unset($implementations['config_builder']);
    $implementations['config_builder'] = FALSE;
  }
}

/**
 * Implements hook_variable_info_alter().
 */
function config_builder_variable_info_alter(&$variables, $options) {
  foreach (config_builder_crud_index_load_all() as $index) {
    if (!isset($variables[$index->id])) {
      $variables[$index->id] = array(
        'name' => $index->id,
        'title' => $index->title,
        'type' => _config_builder_variable_type_map($index->type),
        'default' => isset($index->default_value) ? $index->default_value : NULL,
        'group' => $index->config,
        'module' => 'config_builder',
      );
      switch ($index->type) {
        case 'checkboxes':
        case 'radios':
          $variables[$index->id]['options'] = $index->element['#options'];
          break;
      }
    }
  }
}

/**
 * Helper function; returns variable type from form buider type.
 */
function _config_builder_variable_type_map($type) {
  $map = array(
    'checkboxes' => 'options',
    // @TODO - Make a custom type for Fieldset, that renders the elements as
    // defined.
    'fieldset' => 'array',
    'radios' => 'select',
    // Select actually renders as Radio buttons, possibly an issue with Variable
    // module type declarations.
    'select' => 'unknown',
    'textarea' => 'text',
    'textfield' => 'string',
  );
  return isset($map[$type]) ? $map[$type] : 'unknown';
}

/**
 * Implements hook_variable_group_info().
 */
function config_builder_variable_group_info() {
  $groups = array();

  foreach (config_builder_crud_load_all() as $config) {
    $groups[$config->name] = array(
      'title' => $config->label,
      'description' => $config->description,
      'path' => $config->path,
    );
    if ('user_access' == $config->access['type']) {
      $groups[$config->name]['access'] = $config->access['user_access'];
    }
  }

  return $groups;
}
