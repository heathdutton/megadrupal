<?php

/**
 * @file
 * Hooks provided by extra_tabs_menu.
 */

/**
 * Implements hook_extra_tabs_menu().
 *
 * Return a single menu parent that should be included as tabs, or multiple
 * menu parents in the form of an array.
 *
 * Menu parents should be in the form of 'menu_name:mlid' where the mlid is 0
 * if you want to include the whole menu tree.
 */
function hook_extra_tabs_menu() {

  // Display the whole management menu for user 1 on all pages.
  global $user;
  if ($user->uid == 1) {
    return array('management:0');
  }
}

/**
 * Implements hook_extra_tabs_menu_alter().
 *
 * Make changes to the array of menu parents that will be displayed as tabs.
 */
function hook_extra_tabs_menu_alter(&$menu_parents) {
  // Make sure that the management menu is never displayed as tabs.
  foreach ($menu_parents as $key => $menu_parent) {
    if (strpos($menu_parent, 'management:') === 0) {
      unset($menu_parents[$key]);
    }
  }
}
