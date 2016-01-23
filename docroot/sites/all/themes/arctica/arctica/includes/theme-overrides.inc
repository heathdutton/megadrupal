<?php

/**
 * Prepare system messages for advanced theming
 * 'messages' class is included for backward compatibility with core css
 */
function arctica_status_messages($variables) {
  $display = $variables['display'];
  $output = '';
  foreach (drupal_get_messages($display) as $type => $messages) {
    foreach ($messages as $message) {
      $output .= "<div class=\"grid-100 message messages $type\">\n";
      $output .= '  <p>' . $message . "</p>\n";
      $output .= "</div>\n";
    }
  }
  return $output;
}

/**
 * Breadcrumb with title
 */
function arctica_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    $output = '';
    $breadcrumb[] = truncate_utf8(drupal_get_title(), 60, $wordsafe = TRUE, $dots = TRUE);
    $output .= '<div class="breadcrumb">' . implode(' &raquo; ', $breadcrumb) . '</div>';
    return $output;
  }
}
/**
 * Make drupal core generated images responsive i.e. flexible in width
 */
function arctica_image($variables) {
  $attributes = $variables['attributes'];
  $attributes['src'] = file_create_url($variables['path']);

  // we remove width and height attributes
  foreach (array('alt', 'title') as $key) {

    if (isset($variables[$key])) {
      $attributes[$key] = $variables[$key];
    }
  }

  return '<img' . drupal_attributes($attributes) . ' />';
}