<?php
/**
 * @file
 * Theme functions for the "Out of date" module.
 */

/**
 * Theme wrapper for the "Out of date" message.
 *
 * @ingroup themeable
 */
function theme_outofdate_message($variables) {
  $element = $variables['element'];
  if (empty($element['#classes'])) {
    $classes = array();
  }
  elseif (is_string($element['#classes'])) {
    $classes = explode(' ', $element['#classes']);
  }
  elseif (is_array($element['#classes'])) {
    $classes = $element['#classes'];
  }
  else {
    $classes = array();
  }

  $classes = array_unique($classes + array('outofdate'));
  $element['#attributes']['class'] = implode(' ', $classes);

  return '<div ' . drupal_attributes($element['#attributes']) . '>' . $element['#children'] . '</div>';
}
