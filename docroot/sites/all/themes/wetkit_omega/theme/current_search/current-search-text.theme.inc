<?php
/**
 * @file
 * current-search-text.theme.inc
 */


/**
 * Theme the 'current search text' for search blocks.
 *
 * @param array $variables
 * @return string
 */
function wetkit_omega_current_search_text(array $variables) {
  // Initializes output, sanitizes text if necessary.
  $sanitize = empty($variables['options']['html']);
  $output = ($sanitize) ? check_plain($variables['text']) : $variables['text'];

  // Adds wrapper markup and CSS classes.
  if ($variables['wrapper'] && $variables['element']) {
    $attributes = array('class' => $variables['class']);
    $element = check_plain($variables['element']);
    $output = '<' . $element . drupal_attributes($attributes) . '>' . $output . '</' . $element . '>';
  }

  return '<ul><li>' . $output . '</li></ul>';
}
