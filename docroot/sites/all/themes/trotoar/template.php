<?php

require_once dirname(__FILE__) . '/includes/trotoar.inc';

/**
 * Override or insert variables into the block template.
 */
function trotoar_preprocess_block(&$variables) {
  if ($variables['block_id'] == 1) {
    $variables['classes_array'][] = 'first';
  }
  if (isset($variables['block']->last_in_region)) {
    $variables['classes_array'][] = 'last';
  }
}

/**
 * Thanks for the Amazing Zen, this part is similar with Zen code.
 */
$variables['add_respond_js'] = '';$variables['add_html5_shim'] = '';$variables['add_metatags'] = '';

function trotoar_preprocess_html(&$variables, $hook) { 
  // Add variables and paths needed for HTML5 and responsive support.
  $variables['base_path'] = base_path();
  $variables['path_to_trotoar'] = drupal_get_path('theme', 'trotoar');
  $variables['add_respond_js'] = theme_get_setting('responsive_respond');
  $variables['add_html5_shim'] = theme_get_setting('responsive_html5');
  $variables['add_metatags']   = theme_get_setting('responsive_meta');
}