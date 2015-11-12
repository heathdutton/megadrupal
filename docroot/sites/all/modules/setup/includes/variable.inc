<?php
/**
 * @file
 * Variable module integration.
 */

/**
 * Implements hook_setup_variable_set_form_alter().
 */
function variable_setup_variable_set_form_alter(&$element, $key) {
  module_load_include('inc', 'variable', 'variable.form');
  $variable = variable_form_element($key);
  if (isset($variable['#type']) && $variable['#type'] !== 'item') {
    $element = $variable;
  }
}
