<?php
/**
 * @file
 * contains the theme functions for secureshare module
 */

/**
 * Theme function returns the secureshare element output
 * @param $element
 */
function theme_secureshare_element(&$variables)
{
  $element = $variables['element'];

  // Assign an html ID.
  if (!isset($element['#attributes']['id'])) {
    $element['#attributes']['id'] = $element['#id'];
  }

  return '<div' . drupal_attributes($element['#attributes']) . '>' . $element['#children'] . '</div>';
}