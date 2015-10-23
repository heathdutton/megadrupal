<?php
/**
 * @file
 * CMS theme template file
 */

/**
 * Implements theme_menu_tree().
 */
function cms_theme_menu_tree($variables) {
  return '<ul class="menu clearfix">' . $variables['tree'] . '</ul>';
}
