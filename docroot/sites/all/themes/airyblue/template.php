<?php

/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 */

/**
 * Implements theme_preprocess_menu_local_task().
 *
 * @see zen_preprocess_menu_local_task()
 */
function airyblue_preprocess_menu_local_task(&$variables) {
  $link =& $variables['element']['#link'];

  // If the link does not contain HTML already, check_plain() it now.
  // After we set 'html'=TRUE the link will not be sanitized by l().
  if (empty($link['localized_options']['html'])) {
    $link['title'] = check_plain($link['title']);
  }
  $link['localized_options']['html'] = TRUE;
  $link['title'] = '<span class="tab">' . $link['title'] . '</span>';
}

/**
 * Implements theme_menu_local_tasks().
 *
 * @see zen_menu_local_tasks()
 */
function airyblue_menu_local_tasks(&$variables) {
  $output = '';

  if ($primary = drupal_render($variables['primary'])) {
    $output .= '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $output .= '<ul class="tabs primary clearfix">' . $primary . '</ul>';
  }
  if ($secondary = drupal_render($variables['secondary'])) {
    $output .= '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $output .= '<ul class="tabs secondary clearfix">' . $secondary . '</ul>';
  }

  return $output;
}
