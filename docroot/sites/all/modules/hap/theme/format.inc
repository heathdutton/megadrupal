<?php

/**
 * Theme function for rendering a definition list.
 * @todo document the variables
 */
function theme_hap_format_definition_list($variables) {
  $items = '';
  foreach ($variables['items'] as $item) {
    $items .= '<dt>' . $item['term'] . '</dt>';
    $definitions = '';

    foreach ($item['data'] as $definition) {
      $definitions .= '<dd>' . $definition . '</dd>';
    }

    $items .= $definitions;
  }

  return '<dl' . drupal_attributes($variables['attributes']) . '>' . $items . '</dl>';
}



/**
 * Theme function for formatting a date.
 */
function theme_hap_format_date($variables) {
  $timestamp = mktime($variables['hour'], $variables['minute'], $variables['second'], $variables['month'], $variables['day'], $variables['year']);
  return '<span class="hap-date">' . date($variables['format'], $timestamp) . '</span>';
}
