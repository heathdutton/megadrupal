<?php

/**
 * Returns HTML for a link.
 *
 * All Drupal code that outputs a link should call the l() function. That
 * function performs some initial preprocessing, and then, if necessary, calls
 * theme('link') for rendering the anchor tag.
 *
 * To optimize performance for sites that don't need custom theming of links,
 * the l() function includes an inline copy of this function, and uses that
 * copy if none of the enabled modules or the active theme implement any
 * preprocess or process functions or override this theme implementation.
 *
 * @param $variables
 *   An associative array containing the keys 'text', 'path', and 'options'.
 *   See the l() function for information about these variables.
 *
 * @see l()
 */
function twbs_link($variables) {
  return '<a href="' . check_plain(url($variables['path'], $variables['options'])) . '"' . drupal_attributes($variables['options']['attributes']) . '>' . ($variables['options']['html'] ? $variables['text'] : check_plain($variables['text'])) . '</a>';
}
