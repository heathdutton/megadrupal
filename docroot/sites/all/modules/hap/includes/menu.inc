<?php

/**
 * Returns the menu tree render array of the selected link path.
 *
 * @param $menu_name
 *   The name of the menu to traverse through.
 *
 * @param $link_path
 *   The path to look up.
 *
 * @param $include_self
 *   A boolean to determine of the found menu link should render itself.
 */

function hap_menu_tree_segment($menu_name, $link_path, $include_self = FALSE) {
  $tree = menu_tree($menu_name);
  $segment = hap_menu_get_link_recursive($tree, $link_path);

  if ($include_self) {
    // Wrap the menu link in a new element so we can add properties to it.
    $segment = array($segment);

    // Copy original tree properties.
    foreach (element_properties($tree) as $property) {
      $segment[$property] = $tree[$property];
    }

    return $segment;

  }
  else {
    return $segment['#below'];
  }
}

/**
 * Recursive function to seek out desired menu link.
 *
 * @param $tree
 *   An array of menu data, as provided by menu_tree().
 *
 * @param $link_path
 *   The path to look up.
 *
 * @return
 *   The link item found, otherwise an empty array.
 */

function hap_menu_get_link_recursive($tree, $link_path) {
  $segment = array();

  foreach (element_children($tree) as $mlid) {
    if ($tree[$mlid]['#original_link']['link_path'] == $link_path) {
      $segment = $tree[$mlid];
    }
    elseif (!empty($tree[$mlid]['#below'])) {
      $segment = hap_menu_get_link_recursive($tree[$mlid]['#below'], $link_path);
    }

    // If we've already got something, break out of the loop to return the
    // segment.
    if (!empty($segment)) {
      break;
    }
  }

  return $segment;
}
