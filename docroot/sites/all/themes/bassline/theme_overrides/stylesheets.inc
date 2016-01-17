<?php
/**
 * @file
 * Stylesheets theme overrides.
 */

/**
 * Implements hook_css_alter().
 *
 * Remove default drupal styles and some common contrib styles.
 */
function bassline_css_alter(&$css) {
  // Drupal core stylesheets.
  // Aggregator.
  if (module_exists('aggregator')) {
    unset($css['modules/aggregator/aggregator.css']);
  }
  // Book.
  if (module_exists('book')) {
    unset($css['modules/book/book.css']);
  }
  // Comment.
  if (module_exists('comment')) {
    unset($css['modules/comment/comment.css']);
  }
  // Forum.
  if (module_exists('forum')) {
    unset($css['modules/forum/forum.css']);
  }
  // Poll.
  if (module_exists('poll')) {
    unset($css['modules/poll/poll.css']);
  }
  // Search.
  if (module_exists('search')) {
    unset($css['modules/search/search.css']);
  }
  // Field.
  unset($css['modules/field/theme/field.css']);
  // Node.
  unset($css['modules/node/node.css']);
  // System.
  unset($css['modules/system/system.menus.css']);
  unset($css['modules/system/system.theme.css']);
  unset($css['modules/system/system.base.css']);
  unset($css['modules/system/system.messages.css']);
  // User.
  unset($css['modules/user/user.css']);

  // Contrib stylesheets.
  // Views.
  if (module_exists('views')) {
    $views = drupal_get_path('module', 'views');
    unset($css[$views . '/css/views.css']);
  }
  // Ctools.
  if (module_exists('ctools')) {
    $ctools = drupal_get_path('module', 'ctools');
    unset($css[$ctools . '/css/ctools.css']);
    unset($css[$ctools . '/css/modal.css']);
  }
}
