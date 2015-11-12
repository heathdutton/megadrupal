<?php

/**
 * Returns HTML for guidelines for a text format.
 *
 * @param $variables
 *   An associative array containing:
 *   - format: An object representing a text format.
 *
 * @ingroup themeable
 */
function twbs_filter_guidelines($variables) {
  $format = $variables['format'];
  $attributes['class'][] = 'filter-guidelines-item';
  $attributes['class'][] = 'filter-guidelines-' . $format->format;
  $output = '<div' . drupal_attributes($attributes) . '>';
  $output .= '<h3>' . check_plain($format->name) . '</h3>';
  $output .= theme('filter_tips', array('tips' => _filter_tips($format->format, FALSE)));
  $output .= '</div>';
  return $output;
}
