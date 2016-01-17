<?php

/**
 * @file
 * This file is empty by default because the base theme chain (Alpha & Omega)
 * provides all the basic functionality. However, in case you wish to customize
 * the output that Drupal generates through Alpha & Omega this file is a good
 * place to do so.
 *
 * Alpha comes with a neat solution for keeping this file as clean as possible
 * while the code for your subtheme grows. Please read the README.txt in the
 * /preprocess and /process subfolders for more information on this topic.
 */


/**
 * Implements template_process_html().
 *
 */
function tej_process_html(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
}

/**
 * Implements template_process_page().
 */
function tej_process_page(&$vars, $hook) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($variables);
  }
}

// Add link for Ubuntu font
drupal_add_css('http://fonts.googleapis.com/css?family=Ubuntu',
array('group' => CSS_THEME, 'type' => 'external'));
