<?php

/**
 * @file
 * Contains preprocess and theme functions.
 */

/**
 * Implements hook_preprocess_page().
 */
function reset_preprocess_page(&$variables) {
  // Remove feed icon according to theme setting.
  if (!theme_get_setting('feed_icon')) {
    $variables['feed_icons'] = '';
  }

  // Remove breadcrumb according to theme setting.
  if (!theme_get_setting('breadcrumb')) {
    $variables['breadcrumb'] = '';
  }

  if (!$variables['tabs']['#primary']) {
    $variables['tabs'] = '';
  }

  $variables['main_menu'] = theme_get_setting('toggle_main_menu') ? menu_tree(variable_get('menu_main_links_source', 'main-menu')) : '';
  $variables['secondary_menu'] = theme_get_setting('toggle_secondary_menu') ? menu_tree(variable_get('menu_secondary_links_source', 'user-menu')) : '';
}

/**
 * Display a view as a grid style.
 */
function reset_preprocess_views_view_grid(&$vars) {
  drupal_add_css(drupal_get_path('theme', 'reset') . '/css/views-view-grid.css');
  foreach (element_children($vars['row_classes']) as $key => $value) {
    $vars['row_classes'][$key] .= 'view-grid-row has-' . $vars['options']['columns'] . '-cols';
  }
}
