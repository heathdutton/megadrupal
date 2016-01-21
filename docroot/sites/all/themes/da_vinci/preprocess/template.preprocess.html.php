<?php
/**
 * @file
 * Preproccess functions for HTML.
 */

/**
 * Implements da_vinci_preprocess_html().
 */
function da_vinci_preprocess_html(&$vars) {
  if (isset($vars['node'])) {
    // For full nodes.
    $vars['classes_array'][] = ($vars['node']) ? 'full-node' : '';
    // For forums.
  }
  if (module_exists('panels') && function_exists('panels_get_current_page_display')) {
    $vars['classes_array'][] = (panels_get_current_page_display()) ? 'panels' : '';
  }
  if (module_exists('libraries')) {
    $lib_dv_dir = libraries_get_path('da-vinci-plugins');
    $lib_easymodal_dir = libraries_get_path('easymodal');
    drupal_add_js($lib_dv_dir . '/css_browser.js');
    if (theme_get_setting('styleguide') && module_exists('styleguide') && module_exists('jquery_update')) {
      $theme_path = drupal_get_path('theme', 'da_vinci');
      $lib_dv_dir = libraries_get_path('da-vinci-plugins');
      drupal_add_js($lib_dv_dir . '/jquery.actual.min.js');
      drupal_add_js($lib_easymodal_dir . '/jquery.easyModal.js');
      drupal_add_js($theme_path . '/js/modales.js');
    }
  }
  if (theme_get_setting('debug')) {
    $vars['html_classes'][] = 'debug';
  }

  // Since menu is rendered in preprocess_page we need to detect it
  // here to add body classes.
  $has_main_menu = theme_get_setting('toggle_main_menu');
  $has_secondary_menu = theme_get_setting('toggle_secondary_menu');

  // Add extra classes to body for more flexible theming.
  if ($has_main_menu or $has_secondary_menu) {
    $vars['classes_array'][] = 'with-navigation';
  }

  if ($has_secondary_menu) {
    $vars['classes_array'][] = 'with-subnav';
  }

  if (!empty($vars['page']['featured'])) {
    $vars['classes_array'][] = 'featured';
  }

  if ($vars['is_admin']) {
    $vars['classes_array'][] = 'admin';
  }

  if (!empty($vars['page']['header_top'])) {
    $vars['classes_array'][] = 'header_top';
  }

  // Add unique classes for each page and website section.
  $path = drupal_get_path_alias($_GET['q']);
  $temp = explode('/', $path, 2);
  $section = array_shift($temp);
  $page_name = array_shift($temp);

  // Add template suggestions.
  $vars['theme_hook_suggestions'][] = "page__section__" . $section;
  $vars['theme_hook_suggestions'][] = "page__" . $page_name;

}
