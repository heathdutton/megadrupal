<?php
/**
 * @file
 * Theme and preprocess functions for menus
 */

/**
 * Implements theme_menu_tree__navbar_right().
 * @see radix_stanley_preprocess_page
 */
function radix_stanley_menu_tree__navbar_right(&$variables) {
  $variables['tree'] = '<ul class="menu nav navbar-nav navbar-right">' . $variables['tree'] . '</ul>';
  return $variables['tree'];
}
