<?php

/**
 * Returns HTML for a hidden form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #name, #value, #attributes.
 *
 * @ingroup themeable
 */
function twbs_hidden($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'hidden';
  element_set_attributes($element, array('name', 'value'));
  return '<input' . drupal_attributes($element['#attributes']) . " />\n";
}
