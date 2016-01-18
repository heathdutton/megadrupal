<?php
/**
 * @file
 * Container related theme functions.
 */


/**
 * Overrides theme_container().
 */
function strapped_container($variables) {

  // Grab a reference to the element for ease of use.
  $element = &$variables['element'];

  // Special handling for form elements.
  if (isset($element['#array_parents'])) {

    // Assign an html ID.
    if (!isset($element['#attributes']['id'])) {
      $element['#attributes']['id'] = $element['#id'];
    }

    // Add classes.
    $element['#attributes']['class'][] = 'form-wrapper';
  }

  // Copy the element grid settings
  return  theme('column_wrapper',$variables);

}


