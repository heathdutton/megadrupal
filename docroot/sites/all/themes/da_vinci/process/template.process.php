<?php
/**
 * @file
 * Process functions.
 */

/**
 * Implements template_process_html().
 */
function da_vinci_process_html(&$vars) {
  if (theme_get_setting('debug') && user_access('administer content')) {
    $vars['html_classes'] = implode(' ', $vars['html_classes']);
  }
  else {
    $vars['html_classes'] = '';
  }
}

/**
 * Implements template_process_span_container().
 */
function da_vinci_process_span_container(&$vars) {
  $element = $vars['element'];
  if (isset($element['#attributes']['class'])) {
    $classes = $element['#attributes']['class'];
  }
  $classes[] = 'span-container';

  $vars['classes'] = implode(' ', $classes);
}
