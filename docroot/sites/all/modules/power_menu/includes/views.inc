<?php

/**
 * @file
 * Views specific functions for Power Menu
 */

/*
* Implements hook_views_pre_render().
 *
 * Specific implementation of taxonomy entity handling when the views taxonomy_term built in view is used.
*/
function power_menu_views_pre_render(&$view) {

  if($view->name == 'taxonomy_term' && is_array($view->args)) {
    // Use firs term to set the menu path
    $term_id = array_pop($view->args);
    $term = taxonomy_term_load($term_id);
    if($term) {
      power_menu_set_path($term, 'taxonomy_term', NULL);
    }
  }
}
