<?php
/**
 * @file
 * This file contains modified copies of functions in menu.inc from Drupal core.
 * Dominion require a change in this function, but since the menu system contains
 * very few hooks, we have to copy some functions in order to make a change.
 *
 * The copies allows us to create a menu tree without checking the node access
 * records. This allows us to display links to content from other subsites.
 * The dominion outbound link alter functions will rewrite the links to the
 * correct subsite.
 */

/**
 * Copy of menu_tree().
 *
 * Use our version of menu_tree_page_data().
 */
function _dominion_menu_menu_tree($menu_name) {
  $menu_output = &drupal_static(__FUNCTION__, array());

  if (!isset($menu_output[$menu_name])) {
    $tree = _dominion_menu_menu_tree_page_data($menu_name);
    $menu_output[$menu_name] = menu_tree_output($tree);
  }
  return $menu_output[$menu_name];
}

/**
 * Copy of menu_tree_page_data().
 *
 * Use our version of menu_build_tree().
 */
function _dominion_menu_menu_tree_page_data($menu_name, $max_depth = NULL, $only_active_trail = FALSE) {
  $tree = &drupal_static(__FUNCTION__, array());

  // Check if the active trail has been overridden for this menu tree.
  $active_path = menu_tree_get_path($menu_name);
  // Load the menu item corresponding to the current page.
  if ($item = menu_get_item($active_path)) {
    if (isset($max_depth)) {
      $max_depth = min($max_depth, MENU_MAX_DEPTH);
    }
    // Generate a cache ID (cid) specific for this page.
    $cid = 'dominion_menu:links:' . $menu_name . ':page:' . $item['href'] . ':' . $GLOBALS['language']->language . ':' . (int) $item['access'] . ':' . (int) $max_depth;
    // If we are asked for the active trail only, and $menu_name has not been
    // built and cached for this page yet, then this likely means that it
    // won't be built anymore, as this function is invoked from
    // template_process_page(). So in order to not build a giant menu tree
    // that needs to be checked for access on all levels, we simply check
    // whether we have the menu already in cache, or otherwise, build a minimum
    // tree containing the breadcrumb/active trail only.
    // @see menu_set_active_trail()
    if (!isset($tree[$cid]) && $only_active_trail) {
      $cid .= ':trail';
    }

    if (!isset($tree[$cid])) {
      // If the static variable doesn't have the data, check {cache_menu}.
      $cache = FALSE; //cache_get($cid, 'cache_menu');
      if ($cache && isset($cache->data)) {
        // If the cache entry exists, it contains the parameters for
        // menu_build_tree().
        $tree_parameters = $cache->data;
      }
      // If the tree data was not in the cache, build $tree_parameters.
      if (!isset($tree_parameters)) {
        $tree_parameters = array(
          'min_depth' => 1,
          'max_depth' => $max_depth,
        );
        // Parent mlids; used both as key and value to ensure uniqueness.
        // We always want all the top-level links with plid == 0.
        $active_trail = array(0 => 0);

        // If the item for the current page is accessible, build the tree
        // parameters accordingly.
        if ($item['access']) {
          // Find a menu link corresponding to the current path. If $active_path
          // is NULL, let menu_link_get_preferred() determine the path.
          if ($active_link = menu_link_get_preferred($active_path, $menu_name)) {
            // The active link may only be taken into account to build the
            // active trail, if it resides in the requested menu. Otherwise,
            // we'd needlessly re-run _menu_build_tree() queries for every menu
            // on every page.
            if ($active_link['menu_name'] == $menu_name) {
              // Use all the coordinates, except the last one because there
              // can be no child beyond the last column.
              for ($i = 1; $i < MENU_MAX_DEPTH; $i++) {
                if ($active_link['p' . $i]) {
                  $active_trail[$active_link['p' . $i]] = $active_link['p' . $i];
                }
              }
              // If we are asked to build links for the active trail only, skip
              // the entire 'expanded' handling.
              if ($only_active_trail) {
                $tree_parameters['only_active_trail'] = TRUE;
              }
            }
          }
          $parents = $active_trail;

          $expanded = variable_get('menu_expanded', array());
          // Check whether the current menu has any links set to be expanded.
          if (!$only_active_trail && in_array($menu_name, $expanded)) {
            // Collect all the links set to be expanded, and then add all of
            // their children to the list as well.
            do {
              $result = db_select('menu_links', NULL, array('fetch' => PDO::FETCH_ASSOC))
                ->fields('menu_links', array('mlid'))
                ->condition('menu_name', $menu_name)
                ->condition('expanded', 1)
                ->condition('has_children', 1)
                ->condition('plid', $parents, 'IN')
                ->condition('mlid', $parents, 'NOT IN')
                ->execute();
              $num_rows = FALSE;
              foreach ($result as $item) {
                $parents[$item['mlid']] = $item['mlid'];
                $num_rows = TRUE;
              }
            } while ($num_rows);
          }
          $tree_parameters['expanded'] = $parents;
          $tree_parameters['active_trail'] = $active_trail;
        }
        // If access is denied, we only show top-level links in menus.
        else {
          $tree_parameters['expanded'] = $active_trail;
          $tree_parameters['active_trail'] = $active_trail;
        }
        // Cache the tree building parameters using the page-specific cid.
        cache_set($cid, $tree_parameters, 'cache_menu');
      }

      // Build the tree using the parameters; the resulting tree will be cached
      // by _menu_build_tree().
      $tree[$cid] = _dominion_menu_menu_build_tree($menu_name, $tree_parameters);
    }
    return $tree[$cid];
  }

  return array();
}

/**
 * Copy of menu_build_tree().
 *
 * Use our version of menu_tree_check_access().
 */
function _dominion_menu_menu_build_tree($menu_name, array $parameters = array()) {
  // Build the menu tree.
  $data = _menu_build_tree($menu_name, $parameters);

  // The original $data is referenced to the internal cache of _menu_build_tree.
  // Make a deep copy to prevent cluttering this cache and breaking other menu blocks.
  $data = unserialize(serialize($data));

  // Check access for the current user to each item in the tree.
  _dominion_menu_menu_tree_check_access($data['tree'], $data['node_links']);
  return $data['tree'];
}

/**
 * Copy of menu_tree_check_access().
 *
 * Do not check for node access records, only for node published status.
 */
function _dominion_menu_menu_tree_check_access(&$tree, $node_links = array()) {
  if ($node_links) {
    $nids = array_keys($node_links);
    $select = db_select('node', 'n');
    $select->addField('n', 'nid');
    $select->condition('n.status', 1);
    $select->condition('n.nid', $nids, 'IN');
    $nids = $select->execute()->fetchCol();
    foreach ($nids as $nid) {
      foreach ($node_links[$nid] as $mlid => $link) {
        $node_links[$nid][$mlid]['access'] = TRUE;
      }
    }
  }
  _menu_tree_check_access($tree);
}
