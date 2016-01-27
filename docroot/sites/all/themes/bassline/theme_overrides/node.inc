<?php
/**
 * @file
 * Node theme overrides.
 */

/**
 * Implements template_preprocess_node().
 */
function bassline_preprocess_node(&$vars) {
  // Add suggestions to allow templates to target specific view modes.
  $vars['theme_hook_suggestions'][] = 'node__' . $vars['type'] . '__' .
    $vars['view_mode'];

  // Add classes based on node status.
  if ($vars['promote']) {
    $vars['classes_array'][] = 'promoted';
  }
  if ($vars['sticky']) {
    $vars['classes_array'][] = 'sticky';
  }
  if ($vars['status'] == "0") {
    $vars['classes_array'][] = 'unpublished';
  }
}
