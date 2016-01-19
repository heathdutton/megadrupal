<?php
/**
 * @file
 * Tools for working with the menu system
 */

/**
 * Trim a menu tree so it does not extend beyond a set limit
 *
 * This is a copy of menu_tree_depth_trim.
 *
 * @see menu_tree_depth_trim in menu_block.module
 *
 * @param $tree
 * @param $depth_limit
 *
 * @return mixed
 */
function xu_menu_tree_depth_trim(&$tree, $depth_limit) {
  // Prevent invalid input from returning a trimmed tree.
  if ($depth_limit < 1) {
    return;
  }

  // Examine each element at this level to find any possible children.
  foreach (array_keys($tree) as $key) {
    if ($tree[$key]['below']) {
      if ($depth_limit > 1) {
        menu_tree_depth_trim($tree[$key]['below'], $depth_limit -1);
      }
      else {
        // Remove the children items.
        $tree[$key]['below'] = FALSE;
      }
    }
    if ($depth_limit == 1 && $tree[$key]['link']['has_children']) {
      // Turn off the menu styling that shows there were children.
      $tree[$key]['link']['has_children'] = FALSE;
      $tree[$key]['link']['leaf_has_children'] = TRUE;
    }
  }
}

/**
 * Set the breadcrumb based on the menu
 */
function xu_menu_set_breadcrumb($use_menu) {
  static $menu_id_cache = array();
  $menu_item = menu_get_item();
  $ckey = $menu_item['href'];
  if (!isset($menu_id_cache[$ckey])) {
    $result = db_query("SELECT mlid, menu_name FROM {menu_links} WHERE link_path = '%s'", $menu_item['href']);
    $menu_link_menus = array();
    while ($menu_link = db_fetch_array($result)) {
      $menu_link_menus[$menu_link['mlid']] = $menu_link['menu_name'];
    }
    $menu_id_cache[$ckey] = $menu_link_menus;
  }
  $menu_links = $menu_id_cache[$ckey];
  foreach ($menu_links as $mlid => $menu_name) {
    if ($menu_name == $use_menu) {
      menu_set_active_menu_name($menu_name);
      $breadcrumb = menu_get_active_breadcrumb();
      drupal_set_breadcrumb($breadcrumb);
      return TRUE;
    }
  }
}
