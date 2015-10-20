<?php
/**
 * @file
 * Variable module integration.
 */

/**
 * Implements hook_config_builder_form_builder_types_alter() on behalf of
 * variable.module.
 */
function variable_config_builder_form_builder_types_alter(&$fields) {
  $variables = _config_builder_variables_options_list();
  $fields['variable_form_element'] = array(
    'title' => t('Variable'),
    'properties' => array(
      'variable',
    ),
    'default' => array(
      '#type' => 'variable_form_element',
      '#variable' => key($variables[key($variables)]),
    ),
  );
}

/**
 * Implements hook_config_builder_form_builder_properties_alter() on behalf of
 * variable.module.
 */
function variable_config_builder_form_builder_properties_alter(&$properties, $form_type) {
  if ('config_builder' == $form_type) {
    $properties['variable'] = array(
      'form' => 'config_builder_property_variable_form_element_form',
    );
  }
}

/**
 * Configuration form for the "variable" property.
 */
function config_builder_property_variable_form_element_form(&$form_state, $form_type, $element, $property) {
  $form = array();

  $form['variable'] = array(
    '#title' => t('Variable'),
    '#type' => 'select',
    '#options' => _config_builder_variables_options_list(),
    '#default_value' => $element['#variable'],
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Helper functon; Build variables options list.
 */
function _config_builder_variables_options_list() {
  $variables = &drupal_static(__FUNCTION__, array());
  $module_info = system_get_info('module');
  foreach (variable_get_info() as $name => $variable) {
    if (!isset($variables[$module_info[$variable['module']]['name']])) {
      $variables[$module_info[$variable['module']]['name']] = array();
    }
    $variables[$module_info[$variable['module']]['name']][$name] = $variable['title'];
    if (isset($variable['multiple'])) {
      $variable = variable_build($variable);
      foreach ($variable['children'] as $name => $child) {
        $variables[$module_info[$variable['module']]['name']][$name] = "{$variable['title']} [{$child['title']}]";
      }
    }
  }
  ksort($variables);
  foreach ($variables as &$group) {
    ksort($group);
  }
  return $variables;
}
