<?php

/**
 * @file
 * Needs to be documented.
 */

/**
 * Implements theme_menu_tree().
 */
function nerra_menu_tree($variables) {
  return '<ul class="menu clearfix">' . $variables['tree'] . '</ul>';
}
