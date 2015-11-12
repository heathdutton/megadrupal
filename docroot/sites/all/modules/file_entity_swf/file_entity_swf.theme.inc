<?php

/**
 * @file
 * Theme functions for File Entity SWF
 */

/**
 * Returns an <object> element with child <param> values.
 *
 * Add additional params via hook_preprocess_file_entity_file_swf. An example
 * would be to add "allowfullscreen", simple add the "allowfullscreen" key as
 * $variables['params']['allowfullscreen'] with the value of ture or false.
 *
 * @param array $variables An associative array of themable variables.
 *
 * @ingroup themeable
 *
 * @return string
 */
function theme_file_entity_file_swf($variables) {
  $output = '<object' . drupal_attributes($variables['attributes']) . '>';
  foreach ($variables['params'] as $param => $param_value) {
    $param_attributes = array(
      'name' => $param,
      'value' => $param_value,
    );
    $output .= '<param' . drupal_attributes($param_attributes) . '>';
  }
  $output .= '</object>';
  return $output;
}
