<?php

/**
 * @file
 * Contains theme override functions and preprocess functions for the Subtle Simplicity theme.
 */

function subtle_simplicity_preprocess_html(&$variables) {
  drupal_add_css('http://fonts.googleapis.com/css?family=Fredericka the Great:regular|Questrial:400&amp;subset=latin', array('type' => 'external'));
}

/**
 * Intercept template variables
 *
 * @param $vars
 *   A sequential array of variables passed to the theme function.
 */
function subtle_simplicity_preprocess(&$variables, $hook) {
  if (module_exists('libraries')) {
    $variables['cssgrid_library_path'] = module_invoke('libraries', 'get_path', '1140_cssgrid') . '/';
  }
  else {
    $variables['cssgrid_library_path'] = 'sites/all/libraries/1140_cssgrid/';
  }
  drupal_add_css($variables['cssgrid_library_path'] . 'css/1140.css');
  drupal_add_css($variables['cssgrid_library_path'] . 'css/ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 9', '!IE' => FALSE), 'preprocess' => FALSE));
}

/**
 * Implementation of template_preprocess_page().
 */
function subtle_simplicity_preprocess_page(&$variables) {

  $page = $variables['page'];

  /**
   * Here we define class for 1140 grid based on what regions are set
   */

  //If sidebar_first and sidebar_second make main_content
  // 6 columns wide and the sidebars 3.
  if (!empty($page['sidebar_first']) && !empty($page['sidebar_second'])) {
    $variables['main_content_class']  =  'sixcol';
    $variables['sidebar_first_class'] =  'threecol';
    $variables['sidebar_second_class']  =  'threecol last';
  }
  //If its just a sidebar than main is 9 and sidebar is 3.
  elseif (!empty($page['sidebar_first'])) {
    $variables['main_content_class']  =  'ninecol last';
    $variables['sidebar_first_class']  =  'threecol';
  }
  elseif (!empty($page['sidebar_second'])) {
    $variables['main_content_class']  =  'ninecol';
    $variables['sidebar_second_class']  =  'threecol last';
  }//if theres no sidebars than make main content full width.
  else {
    $variables['main_content_class']  =  'twelvecol';
  }
}

/**
 * Implementation of hook_css_alter().
 */
function subtle_simplicity_css_alter(&$css) {
  //kill borons layout css file so we can add our own 1140 without problems.
  if (isset($css[drupal_get_path('theme', 'boron') . '/css/layout.css'])) {
    unset($css[drupal_get_path('theme', 'boron') . '/css/layout.css']);
  }
}
