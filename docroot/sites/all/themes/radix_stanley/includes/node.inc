<?php
/**
 * @file
 * Theme and preprocess functions for nodes
 */

/**
 * Implements template_preprocess_node().
 */
function radix_stanley_preprocess_node(&$variables) {
  $node = $variables['node'];
  $author = user_load($node->uid);

  // Add author to node.
  $variables['author'] = theme('username', array('account' => $author));

  // Add default user picture if blank
  if (empty($variables['user_picture'])) {
    $default_picture = drupal_get_path('theme', 'radix_stanley') . '/assets/images/user.png';
    $variables['user_picture'] = theme('image', array('path' => $default_picture));
  }
}