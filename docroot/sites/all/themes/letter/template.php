<?php
/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. You can modify or override Drupal's theme
 *   functions, intercept or make additional variables available to your theme,
 *   and create custom PHP logic. For more information, please visit the Theme
 *   Developer's Guide on Drupal.org: http://drupal.org/theme-guide
 */

/**
 * Override or insert variables into the html templates.
 */
function letter_preprocess_html(&$variables, $hook) {
  $variables['webfont_js'] = theme_get_setting('letter_webfont_js');
  $variables['webfont_css'] = theme_get_setting('letter_webfont_css');
  drupal_add_js($variables['webfont_js'], 'external');
  drupal_add_css($variables['webfont_css'], 'external');
}
