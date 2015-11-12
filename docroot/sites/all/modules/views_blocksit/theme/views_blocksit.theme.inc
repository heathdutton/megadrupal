<?php

/**
 * @file
 * The theme file for the template.
 */

/**
 * Implements hook_process_views_blocksit_grid().
 */
function views_blocksit_process_views_blocksit_grid(&$vars) {
  module_load_include('inc', 'views', 'theme/theme');
  template_preprocess_views_view_unformatted($vars);

  $vars['blocksit_options'] = $vars['options']['blocksit_options'];
}
