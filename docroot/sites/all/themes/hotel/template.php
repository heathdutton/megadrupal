<?php
/**
 * @file
 * controls load theme.
 */

require_once drupal_get_path('theme', 'hotel') . '/inc/preprocess_functions.inc';

function hotel_preprocess_html(&$vars) {
  $node_id = drupal_lookup_path('source','page-404');
  if(!empty($node_id)) {
    $parts = explode("/", $node_id);
    $n_id = false;
    if(count($parts) > 1) {
      $n_id = $parts[1];
    }
    if(in_array("html__node__$n_id", $vars['theme_hook_suggestions'])) {
      $vars['classes_array'][] = 'page-404-body';
    }
  }
}
