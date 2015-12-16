<?php

/**
 * Toggleable, contextual menu for displaying lists of links.
 *
 * @param array $variables
 *   An associative array containing values for theme_links().
 *
 * @see theme_links()
 */
function theme_twbs_bootstrap_nav($variables) {
  $output = '';

  $links = &$variables['links'];
  $attributes = &$variables['attributes'];
  if (!isset($attributes['class'])) {
    $attributes['class'] = array();
  }
  if (!preg_grep('/^nav$/', $attributes['class'])) {
    $attributes['class'][] = 'nav';
  }
  if (!preg_grep('/^(nav-tabs|nav-pills)$/', $attributes['class'])) {
    $attributes['class'][] = 'nav-tabs';
  }
  $output = theme('links', $variables);

  return $output;
}
