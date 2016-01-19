<?php

/**
 * @file
 * The theme file for the template.
 */

/**
 * Implements hook_preprocess_views_swipeview_slideshow().
 */
function views_swipeview_process_views_swipeview_slideshow(&$vars) {
  // Ensure we gain some useful classes from the default unformatted style.
  module_load_include('inc', 'views', 'theme/theme');
  template_preprocess_views_view_unformatted($vars);

  $vars['swipeview_options'] = $vars['options']['swipeview_options'];
}
