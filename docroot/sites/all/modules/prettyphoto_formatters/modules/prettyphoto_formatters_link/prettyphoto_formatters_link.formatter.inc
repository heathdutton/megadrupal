<?php

/**
 * @file
 * prettyPhoto formatter hooks and callbacks.
 */

/**
 * Theme function for displaying the prettyphoto link.
 *
 * @param array $variables
 *   An array of link attributes, e.g. class name.
 *
 * @return string
 *   HTML output for displaying the link.
 */
function theme_prettyphoto_formatters_link($variables) {
  $link_options = $variables['element'];

  unset($link_options['title']);
  unset($link_options['url']);

  if (isset($link_options['attributes']['class'])) {
    $link_options['attributes']['class'] = array($link_options['attributes']['class']);
  }

  $link_options['attributes']['rel'] = array(
    'prettyPhoto[' . $variables['field']['field_name'] . ']',
  );

  $link_options['query'] = array(
    'iframe' => 'true',
    'width' => $variables['iframe_width'],
    'height' => $variables['iframe_height'],
  );

  $output = '';
  if (isset($variables['element']['url']) && !empty($variables['element']['url'])) {
    if (!empty($variables['element']['title'])) {
      $title = $variables['element']['title'];
      $url = $variables['element']['url'];
      $output = l($title, $url, $link_options);
    }
    else {
      $title = $variables['element']['url'];
      $url = $variables['element']['url'];
      $output = l($title, $url, $link_options);
    }
  }

  return $output;
}
