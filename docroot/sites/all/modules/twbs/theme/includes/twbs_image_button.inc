<?php

/**
 * Returns HTML for an image button form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #attributes, #button_type, #name, #value, #title, #src.
 *
 * @ingroup themeable
 */
function twbs_image_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'image';
  element_set_attributes($element, array('id', 'name', 'value'));

  $image_params = array(
    'path' => file_create_url($element['#src']),
  );
  if (!empty($element['#title'])) {
    $image_params['alt'] = $element['#title'];
    $image_params['title'] = $element['#title'];
  }

  $element['#attributes']['class'][] = 'btn';
  $element['#attributes']['class'][] = 'btn-default';
  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  return '<button' . drupal_attributes($element['#attributes']) . '>' . theme('image', $image_params) . '</button>';
}
