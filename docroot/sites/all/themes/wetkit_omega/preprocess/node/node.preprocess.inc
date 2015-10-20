<?php
/**
 * @file
 * node.preprocess.inc
 */


/**
 * Implements hook_preprocess_node().
 */
function wetkit_omega_preprocess_node(&$variables, $hook) {
  // Optionally, run node-type-specific preprocess functions, like
  // wetkit_omega_preprocess_node_page() or
  // wetkit_omega_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $variables['node']->type;
  if (function_exists($function)) {
    $function($variables, $hook);
  }
  // Date Support for different themes.
  if (variable_get('wetkit_wetboew_theme', 0) == 1) {
    $variables['date_mod'] = 'gcwu-date-mod';
  }
  elseif (variable_get('wetkit_wetboew_theme', 0) == 2) {
    $variables['date_mod'] = 'gcwu-date-mod';
  }
  elseif (variable_get('wetkit_wetboew_theme', 0) == 3) {
    $variables['date_mod'] = 'base-date-mod';
  }
  elseif (variable_get('wetkit_wetboew_theme', 0) == 4) {
    $variables['date_mod'] = 'ogpl-date-mod';
  }
  else {
    $variables['date_mod'] = 'wet-date-mod';
  }
  $node = $variables['node'];
  global $language;
  $variables['date'] = isset($node->translations->data[$language->language]['changed']) ?
    format_date($node->translations->data[$language->language]['changed'], 'custom', 'Y-m-d') :
    format_date($node->changed, 'custom', 'Y-m-d');

  // Header, submitted and links wrappers have their own attributes.
  $variables['header_attributes_array']['class'][] = 'node-header';
  $variables['footer_attributes_array']['class'][] = 'submitted';
  if ($variables['user_picture']) {
    $variables['footer_attributes_array']['class'][] = 'with-user-picture';
  }
  $variables['links_attributes_array']['class'][] = 'clearfix';
}
