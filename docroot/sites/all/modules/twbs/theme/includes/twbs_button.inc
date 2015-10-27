<?php

/**
 * Returns HTML for a button form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #attributes, #button_type, #name, #value.
 *
 * @ingroup themeable
 */
function twbs_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'btn';
  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  if (!preg_grep('/^btn-.*$/', $element['#attributes']['class'])) {
    $element['#attributes']['class'][] = 'btn-default';
  }

  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['disabled'] = 'disabled';
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  return '<button' . drupal_attributes($element['#attributes']) . '>' . $element['#attributes']['value'] . '</button> ';
}
