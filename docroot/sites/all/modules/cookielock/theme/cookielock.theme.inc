<?php

/**
 * @file
 * Theme functions. This all comes from the Secure Site module (thanks!)
 */

/**
 * Process variables for cookielock-page.tpl.php
 *
 * @param $variables
 *   An array of variables from the theme system.
 */
function template_preprocess_cookielock_page(&$variables) {
  // Add relevant default variables, taken from template_preprocess_page()
  $variables['messages']   = theme('status_messages');
  $variables['base_path']  = base_path();
  $variables['logo']       = theme_get_setting('logo');
  $variables['front_page'] = url();

  // Check if logo exists before output it in a template file
  // First get relative path from logo theme setting
  $logo_parts = parse_url($variables['logo']);
  $logo_path = $logo_parts['path'];
  if (strpos($logo_path, $variables['base_path']) === 0) {
    $logo_path = substr($logo_path, strlen($variables['base_path']));
  }
  // If file does not exist clear it
  if (!file_exists($logo_path)) {
    $variables['logo'] = NULL;
  }

  // Clear out all CSS
  drupal_static_reset('drupal_add_css');

  // Add our css file back, so it will be the only one
  drupal_add_css(drupal_get_path('module', 'cookielock') . '/theme/cookielock.css');

  // Clear out all JS
  drupal_static_reset('drupal_add_js');

  // Required by Core template_process_html()
  $variables['page']['#children'] = NULL;

  // Call Drupal core default html page preprocess function
  template_preprocess_html($variables);
}

/**
 * Process variables for cookielock-page.tpl.php
 *
 * @param $variables
 *   An array of variables from the theme system.
 */
function template_process_cookielock_page(&$variables) {
  // Rather than call template_process_html(&$variables) we'll
  // replicate most of what that function does. We don't want to allow
  // css or js alter, though (because themes like Bootstrap use hook_css_alter()
  // to set CSS that's seemingly impossible to unset from cookielock. (Probably the same for js.)
  // See https://www.drupal.org/node/2549023 and https://www.drupal.org/node/2547655

  // You should still be able to create a cookielock-page template override if you want to add your own styles.

  $variables['page'] = $variables ['page']['#children'];
  $variables['head'] = drupal_get_html_head();
  $variables['css'] = drupal_add_css();
  $variables['styles'] = drupal_get_css(NULL, TRUE);
  $variables['scripts'] = drupal_get_js('header', FALSE, TRUE);
}
