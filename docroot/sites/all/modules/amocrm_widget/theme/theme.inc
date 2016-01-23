<?php
/**
 * @file
 * Theme function of Be sure module.
 */

/**
 * Process variables for amorcm-widget-page.tpl.php.
 */
function template_preprocess_amocrm_widget_page(&$vars) {
  $vars['title'] = drupal_get_title();
}

/**
 * Process variables for amorcm-widget-html.tpl.php.
 */
function template_preprocess_amocrm_widget_html(&$vars) {
  drupal_add_library('system', 'drupal.ajax');
  drupal_add_css(drupal_get_path('module', 'amocrm_widget') . '/includes/widget_fonts.css');
  drupal_add_css(drupal_get_path('module', 'amocrm_widget') . '/includes/widget_style.css');
  drupal_add_js(drupal_get_path('module', 'amocrm_widget') . '/includes/client.js');

  $vars['js'] = drupal_get_js();
  $vars['css'] = drupal_get_css();
}

/**
 * Process variables for amorcm-widget-list.tpl.php.
 */
function template_preprocess_amocrm_widget_preload(&$vars) {
  $preloader_path = drupal_get_path('module', 'amocrm_widget') . '/includes/amocrm_widget_preloader.gif';
  $vars['preloader'] = theme('image', array('path' => $preloader_path));
}
