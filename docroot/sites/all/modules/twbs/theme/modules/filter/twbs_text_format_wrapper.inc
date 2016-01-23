<?php

/**
 * Returns HTML for a text format-enabled form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element containing #children and #description.
 *
 * @ingroup themeable
 */
function twbs_text_format_wrapper($variables) {
  $element = $variables['element'];
  $output = '<div class="text-format-wrapper">';
  $output .= $element['#children'];
  if (!empty($element['#description'])) {
    $output .= '<div class="description">' . $element['#description'] . '</div>';
  }
  $output .= "</div>\n";

  return $output;
}
