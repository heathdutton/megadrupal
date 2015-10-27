<?php

/**
 * @file
 * Make the YUI Grid theme settings available.
 */

/**
 * Implements hook_proceess_HOOK().
 */
function yui_grid_process_html(&$variables) {
  $variables['yui_page_width'] = theme_get_setting('yui_page_width');
}

/**
 * Implements hook_proceess_HOOK().
 */
function yui_grid_process_page(&$variables) {
  $variables['yui_sidebar_width'] = theme_get_setting('yui_sidebar_width');
  $variables['yui_sidebar_location'] = theme_get_setting('yui_sidebar_location');
  $variables['yui_breadcrumbs'] = theme_get_setting('yui_breadcrumbs');
  $variables['yui_responsive'] = theme_get_setting('yui_responsive');
  $variables['yui_main_width'] = yui_grid_calculate_main_width();
}


/**
 * Implements hook_preproceess_HOOK().
 *
 * Make it easier to use blocks in grids.
 */
function yui_grid_preprocess_block(&$variables) {
  $variables['classes_array'][] = 'yui-u';
  if ($variables['elements']['#weight'] == 1) {
    $variables['classes_array'][] = 'first';
  }
}


/**
 * Calculate the width of the main content element given the sidebar.
 */
function yui_grid_calculate_main_width() {
  $sidebar_width = theme_get_setting('yui_sidebar_width');
  $sidebar = theme_get_setting('yui_sidebar_location');

  if ($sidebar) {
    $split_fraction = explode("-", $sidebar_width);
    $difference = $split_fraction[1] - $split_fraction[0];
    $main_width = $difference . "-" . $split_fraction[1];
  }
  else {
    $main_width = "1";
  }

  return $main_width;
}
