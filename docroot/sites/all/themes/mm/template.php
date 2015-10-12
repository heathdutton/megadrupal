<?php

/**
 * @file
 * Theme functions for MM Theme.
 */

/**
 * Implementation of theme_preprocess_html().
 */
function mm_preprocess_html(&$vars) {
  // Add CSS files for Internet Explorer-specific styles.
  drupal_add_css(path_to_theme() . '/css/ielt9.css', array(
    'group' => CSS_THEME,
    'every_page' => TRUE,
    'browsers' => array('IE' => 'lt IE 9', '!IE' => FALSE),
    'preprocess' => FALSE,
  ));
  drupal_add_css(path_to_theme() . '/css/ielt8.css', array(
    'group' => CSS_THEME,
    'every_page' => TRUE,
    'browsers' => array('IE' => 'lt IE 8', '!IE' => FALSE),
    'preprocess' => FALSE,
  ));
}

/**
 * Implements hook_page_alter().
 */
function mm_page_alter($page) {
  // Add meta tag for viewport, for easier responsive theme design.
  $viewport = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1',
    ),
  );
  drupal_add_html_head($viewport, 'viewport');
}
