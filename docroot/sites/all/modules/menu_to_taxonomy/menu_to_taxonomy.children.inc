<?php

/**
 * @file
 * Contains helper functions related to menu children.
 */

/**
 * Gets all child items of a given menu link.
 *
 * @param array $link
 *   The menu link array.
 *
 * @return array
 *   The children.
 *
 * @see http://dropbucket.org/node/485
 */
function _menu_to_taxonomy_get_all_menu_children(array $link) {
  $menu = $link['menu_name'];
  $mlid = $link['mlid'];
  $tree = menu_tree_all_data($menu);
  $children = array();
  if ($mlid) {
    foreach ($tree as $branch) {
      $check = _menu_to_taxonomy_find_mlid_in_menu($mlid, $branch);
      if ($check) {
        $branch = $check;
        foreach ($branch['below'] as $twig) {
          $children[] = $twig['link']['mlid'];
          if ($twig['below']) {
            $children[] = _menu_to_taxonomy_get_all_menu_children(array(
              'menu_name' => $menu,
              'mlid' => $twig['link']['mlid'],
            ));
          }
        }
      }
    }
  }
  return _menu_to_taxonomy_flatten($children);
}


/**
 * Recursively searches a menu tree for a given menu link.
 *
 * Helper function for get_all_menu_children().
 *
 * @param int $mlid
 *   The Menu link id.
 * @param array $link
 *   THe Menu tree item.
 *
 * @return array|NULL
 *   The link array or NULL.
 */
function _menu_to_taxonomy_find_mlid_in_menu($mlid, array $link) {
  if ($mlid == $link['link']['mlid']) {
    return $link;
  }
  else {
    foreach ($link['below'] as $item) {
      $response = _menu_to_taxonomy_find_mlid_in_menu($mlid, $item);
      if ($response) {
        return $response;
      }
    }
  }
}

/**
 * Helper function which flattens a multidimensional array.
 *
 * @param array $array
 *   The unflattened array.
 *
 * @return array
 *   The flattened array.
 *
 * @see http://stackoverflow.com/questions/1319903/how-to-flatten-a-multidimensional-array
 */
function _menu_to_taxonomy_flatten(array $array) {
  $return = array();
  array_walk_recursive($array, function($a) use (&$return) {
    $return[] = $a;
  });
  return $return;
}

/**
 * Gets all mlids of a given menu tree.
 *
 * @param array $tree
 *   The menu tree (see menu_tree_all_data()).
 *
 * @return array
 *   Menu link IDs for children.
 */
function _menu_to_taxonomy_get_menu_items_all(array $tree) {
  $children = array();
  foreach ($tree as $branch) {
    if (isset($branch['link']['mlid'])) {
      $children[] = $branch['link']['mlid'];
    }
    if (isset($branch['below'])) {
      $children[] = _menu_to_taxonomy_get_menu_items_all($branch['below']);
    }
  }
  return _menu_to_taxonomy_flatten($children);
}
