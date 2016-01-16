<?php
/**
 * @file
 * Theme and preprocess functions for comments
 */

/**
 * Implements template_preprocess_comment().
 */
function radix_stanley_preprocess_comment(&$variables) {
  $comment = $variables['comment'];
  $author = user_load($comment->uid);

  // Add author to comment.
  $variables['author'] = theme('username', array('account' => $author));

  // Add default user picture if blank
  if (empty($variables['picture'])) {
    $default_picture = drupal_get_path('theme', 'stanley') . '/assets/images/user.png';
    $variables['picture'] = theme('image', array('path' => $default_picture));
  }
}