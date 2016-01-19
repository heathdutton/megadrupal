<?php

/**
 * Returns HTML for a rangefield form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #description, #min, #max, #attributes,
 *     #step.
 *
 * @ingroup themeable
 */
function twbs_rangefield($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'range';
  element_set_attributes($element, array('id', 'name', 'value', 'step', 'min', 'max'));
  _form_set_class($element, array('form-text', 'form-range'));

  $output = '<input' . drupal_attributes($element['#attributes']) . ' />';

  return $output;
}
