<?php
/**
 * @file
 * current-search-link-active.theme.inc
 */


/**
 * Theme the 'current search' active link for search blocks.
 *
 * @param array $variables
 * @return string
 */
function wetkit_omega_current_search_link_active($variables) {
  // Builds accessible markup.
  // @see http://drupal.org/node/1316580
  $accessible_vars = array(
    'text' => $variables['text'],
    'active' => TRUE,
  );
  $accessible_markup = theme('facetapi_accessible_markup', $accessible_vars);

  // Sanitizes the link text if necessary.
  $sanitize = empty($variables['options']['html']);
  $variables['text'] = ($sanitize) ? check_plain($variables['text']) : $variables['text'];

  // Adds the deactivation widget.
  $temp = theme('current_search_deactivate_widget') . $variables['text'];
  $variables['text'] = '';
  $variables['text'] = $temp;

  // Resets link text, sets to options to HTML since we already sanitized the
  // link text and are providing additional markup for accessibility.
  $variables['text'] .= ' ' . $accessible_markup;
  $variables['options']['html'] = TRUE;
  return theme('link', $variables);
}
