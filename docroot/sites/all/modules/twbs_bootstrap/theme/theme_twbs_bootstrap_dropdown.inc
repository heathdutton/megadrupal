<?php

/**
 * Toggleable, contextual menu for displaying lists of links.
 *
 * @param array $variables
 *   An associative array containing:
 *   - html_tag: wrapper html tag, in theme_html_tag() format.
 *   - link: link for dropdown toggle, in theme_link() format.
 *   - links: links for dropdown menu, in theme_links() format.
 */
function theme_twbs_bootstrap_dropdown($variables) {
  $output = '';

  // Generate unique id.
  $id = drupal_html_id('twbs_bootstrap_dropdown');

  // Prepare toggle.
  $link = $variables['link'];
  $options = &$link['options'];
  $options['html'] = isset($options['html']) ? $options['html'] : FALSE;
  $attributes = &$link['options']['attributes'];
  $attributes['id'] = $id;
  $attributes['data-toggle'] = 'dropdown';
  if (isset($link['path']) && $link['path']) {
    $attributes['role'] = 'button';
    $attributes['data-target'] = '#';
  }
  else {
    $link['path'] = '#';
    $options['external'] = TRUE;
  }
  $output .= theme('link', $link);

  // Prepare menu.
  $links = $variables['links'];
  $links['heading'] = NULL;
  $attributes = &$links['attributes'];
  $attributes['class'][] = 'dropdown-menu';
  $attributes['role'] = 'menu';
  $attributes['aria-labelledby'] = $id;
  $output .= theme('links', $links);

  // Prepare wrapper tag.
  $html_tag = $variables['html_tag'];
  $element = &$html_tag['element'];
  $element['#tag'] = isset($element['#tag']) ? $element['#tag'] : 'div';
  $element['#value'] = $output;
  $attributes = &$element['#attributes'];
  $attributes['class'][] = 'dropdown';
  $output = theme('html_tag', $html_tag);

  return $output;
}
