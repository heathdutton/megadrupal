<?php
/**
 * @file
 * Theme functions.
 */

/**
 * Generates the output of the crop area.
 *
 * @param array $variables
 *   Form element variables.
 *
 * @return markup
 *   HTNL markup of the cropbox.
 */
function theme_jcrop_element($variables) {
  $element = $variables['element'];
  $output = '';

  if (isset($element['cropbox'])) {
    $state_class = drupal_html_class('state-' . $element['state']['#value']);
    $output .= '<div class="crop-image-wrap ' . $state_class . '">';
  }

  if (isset($element['cropbox'])) {
    $output .= '<div class="jcrop-crop-cropbox">';
    $output .= drupal_render($element['cropbox']);
    $output .= '</div>';
  }

  $output .= drupal_render_children($element);

  if (isset($element['cropbox'])) {
    $output .= '</div>';
  }

  return $output;
}
