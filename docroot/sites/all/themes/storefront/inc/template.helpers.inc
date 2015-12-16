<?php

/**
 * @file
 * Helper function for theming main and secondary variables
 */

/**
 * Wrapping menus in NAV element.
 */
function storefront_menu_variables($menu, $type) {
  $output = '<div id="' . $type . '-menu-bar" class="nav"><nav class="clearfix">'
    . $menu . '</nav></div>';
  return $output;
}
