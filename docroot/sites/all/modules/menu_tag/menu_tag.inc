<?php

/**
 * @file
 * Helper functions for menu_tag module.
 */

/**
 * This is a main filter function. Filter menu links in menu based on menu tag.
 * 
 * Explanation about menu_tag storage:
 * 
 * Current menu_tag storage is a result of talking among Miro (miro_dietiker), 
 * Sasha (Berdir) and Ivica (pivica).
 * 
 * Following solution is choosed: menu tags will be stored in separate 
 * table (with mlid as key); mlid-menu_tag combinations will be stored in cache 
 * for improved performance.
 * 
 * More info:
 * 
 *   - First working solution was to add additional menu_tag column to 
 *     menu_links table. This solution was simple and efficient - we only needed
 *     to take care about menu_tag saving/updating and Drupal take care of the
 *     rest (loading/deleting).
 *     But Sasha pointed that this solution can create problems because Drupal 
 *     does not know about additional columns. This can create unexpected 
 *     problems, for example upgrade problems to D8 (like i18n module had when 
 *     upgrading from D6 to D7)
 *   - Solution to this was to move menu_tag column into new table. But this 
 *     added more complexity - because we now need to care about loading & 
 *     deleting operation efficently.
 *     But menu module give us no way to alter in any way building process of
 *     menu trees. The only thing we could try was to alter select queries on
 *     menu_links table and add additional join to menu_tag table. Unfortunatly
 *     menu module is not using query alter tags consistently and we needed
 *     to drop this solution.
 *   - So on the end we decided to implement most complex and scalable solution.
 *     We will leave separate menu_tag table, and we will load menu items tag 
 *     per menu from db table when needed and for better perfromance we will 
 *     cache this results for later reusing.
 */
function menu_tag_filter_menu_items($menu_name, array &$menu_tree, $menu_tag, $context_name = 'render-array') {
  if ($menu_tag == 'no-filter') {
    // Do not apply filter.
    return;
  }
  
  // Load menu tag values for all mlids in this menu tree.
  $mlids_cache = cache_get('menu_tag:' . $menu_name);
  if (FALSE && $mlids_cache) {
    $menu_item_tags = $mlids_cache->data;
  }
  else {
    // Load all mlids for this menu.
    $mlids = array();
    $menu_links = menu_load_links($menu_name);
    foreach ($menu_links as $menu_link) {
      $mlids[] = $menu_link['mlid'];
    }

    $menu_item_tags = db_query("SELECT * FROM {menu_tag} mt WHERE mlid IN (:mlids)", array(':mlids' => $mlids))->fetchAllKeyed();
    cache_set('menu_tag:' . $menu_name, $menu_item_tags);
  }
  
  _menu_tag_filter_menu_items($menu_tree, $menu_item_tags, $menu_tag, $context_name);
}

/**
 * This is a main filter function. Filter menu links in menu based on menu tag.
 * 
 * This is for now two pass process.
 * In first past we will determine which menu items we need to render and which 
 * not (based on menu tag).
 * In second pass we will search for menu item orphans - orphans are menu items 
 * which parent will not be rendered but child will be rendered for this menu 
 * tag. We will collect all this orphans and add it again to menu array in 
 * menu_tag_orphans element, so orphans can also be rendered. 
 * 
 * @param $menu_tree
 *   Menu which we want to filter.
 * @param $menu_tag
 *   Menu tag filter value.
 * @param $context_name
 *   Render menu context name. Can be 'render-array' for menu block situation,
 *   or 'tree-array' for main/secondary menu situation.
 */
function _menu_tag_filter_menu_items(array &$menu_tree, array $menu_item_tags, $menu_tag, $context_name = 'render-array') {
  if ($menu_tag == 'no-filter') {
    // Do not apply filter.
    return;
  }
  
  // First pass.
  _menu_tag_filter_access_menu_items($menu_tree, $menu_item_tags, $menu_tag, $context_name);
  
  // Second pass.
  $orphans = _menu_tag_filter_find_orphans($menu_tree, TRUE, $context_name);
  if (!empty($orphans)) {
    $menu_tree['menu_tag_orphans'] = $orphans;
  }
}

/**
 * Filter access menu links in menu based on menu tag.
 * 
 * @param $menu
 *   Menu which we want to filter.
 * @param $menu_tag
 *   Menu tag filter value.
 */
function _menu_tag_filter_access_menu_items(array &$menu, array $menu_item_tags, $menu_tag, $context_name) {
  
  $context_keys = menu_tag_context_keys($context_name);
  
  $menu_items = element_children($menu);
  foreach ($menu_items as $key) {
    $link =& $menu[$key][$context_keys['original_link']];
    $menu_item_tag = isset($menu_item_tags[$link['mlid']]) ? $menu_item_tags[$link['mlid']] : NULL;
    
    if (!isset($menu_item_tag)) {
      // Just in case lets first check do we have a menu tag for this link.
      continue;
    }
    
    // Apply filter.
    if ($menu_tag == 'without-tag') {
      if (!empty($menu_item_tag)) {
        // Filter link with a tag.
        $menu[$key][$context_keys['access']] = FALSE;
      }
    }
    elseif ($menu_item_tag != $menu_tag) {
      // Filter link if tags do not match.
      $menu[$key][$context_keys['access']] = FALSE;
    }
    
    if (!empty($menu[$key][$context_keys['below']])) {
      // If menu item has children lets filter them also.
      _menu_tag_filter_menu_items($menu[$key][$context_keys['below']], $menu_item_tags, $menu_tag, $context_name);
    }
  }
  
}

/**
 * Find all orphans in menu. Will search orphans in first submenu level and will
 * not go beyond first level of submenus - because 
 * _menu_tag_filter_access_menu_items function will recursivly go through all
 * children menu items so we should not do it twice then here also.
 * 
 * @param array $menu
 *   Menu array.
 * @param boolean $parent_access
 *   Parent access - TRUE if we are going to render parent, FALSE otherwise.
 * @param string $context_name
 * @param int $current_level
 *   Helper parameter which defines in which submenu level we are currently. 
 *   Basically there is no need to go beyond first submenu (level 1) while
 *   we are searching for orphans.
 * @return array
 *   Array of all orphans that are founded.
 */
function _menu_tag_filter_find_orphans(array &$menu, $parent_access, $context_name, $current_level = 0) {
  $orphans = array();
  
  $context_keys = menu_tag_context_keys($context_name);
  
  $menu_items = element_children($menu);
  foreach ($menu_items as $key) {
    
    $item_access = !isset($menu[$key][$context_keys['access']]) || $menu[$key][$context_keys['access']];
    
    if (!$parent_access && $item_access) {
      // If parents will not be showned but children need to be showed then this
      // is menu link that is orphan.
      $orphans[$key] = $menu[$key];
    }
    
    if (!empty($menu[$key][$context_keys['below']])) {
      // If menu item has children lets search for orphans there but only search
      // if this is first level of submenus.
      if (++$current_level < 2) {
        $orphans += _menu_tag_filter_find_orphans($menu[$key][$context_keys['below']], $item_access, $context_name, $current_level);
      }
    }
  }
  
  return $orphans;
}

/**
 * Return an, filtered by menu tag, array of links tag to be rendered as the 
 * Main menu.
 * 
 * @see menu_main_menu()
 */
function menu_tag_main_menu() {
  $menu_tag = variable_get('menu_tag_main_menu', 'main-menu');
  return menu_tag_navigation_links(variable_get('menu_main_links_source', 'main-menu'), $menu_tag);
}

/**
 * Return an, filtered by menu tag, array of links to be rendered as the 
 * Secondary links.
 * 
 * @see menu_secondary_menu()
 */
function menu_tag_secondary_menu() {
  $menu_tag = variable_get('menu_tag_secondary_menu', 'main-menu');
  
  // If the secondary menu source is set as the primary menu, we display the
  // second level of the primary menu.
  if (variable_get('menu_secondary_links_source', 'user-menu') == variable_get('menu_main_links_source', 'main-menu')) {
    return menu_tag_navigation_links(variable_get('menu_main_links_source', 'main-menu'), $menu_tag, 1);
  }
  else {
    return menu_tag_navigation_links(variable_get('menu_secondary_links_source', 'user-menu'), $menu_tag, 0);
  }
}

/**
 * Return an fitlered menu tag array of links for a navigation menu.
 *
 * This is basically a copy of menu_navigation_links() with menu tag filtering.
 * 
 * @see menu_navigation_links
 */
function menu_tag_navigation_links($menu_name, $menu_tag, $level = 0) {
  // Don't even bother querying the menu table if no menu is specified.
  if (empty($menu_name)) {
    return array();
  }

  // Get the menu hierarchy for the current page.
  $tree = menu_tree_page_data($menu_name, $level + 1);

  // Go down the active trail until the right level is reached.
  while ($level-- > 0 && $tree) {
    // Loop through the current level's items until we find one that is in trail.
    while ($item = array_shift($tree)) {
      if ($item['link']['in_active_trail']) {
        // If the item is in the active trail, we continue in the subtree.
        $tree = empty($item['below']) ? array() : $item['below'];
        break;
      }
    }
  }
  
  // Filter this menu tree.
  menu_tag_filter_menu_items($menu_name, $tree, $menu_tag, 'tree-array');
  
  // Create a single level of links.
  $router_item = menu_get_item();
  $links = array();
  foreach ($tree as $item) { 
    if (!$item['link']['hidden'] && (!isset($item['access']) || $item['access'])) {
      $class = '';
      $l = $item['link']['localized_options'];
      $l['href'] = $item['link']['href'];
      $l['title'] = $item['link']['title'];
      if ($item['link']['in_active_trail']) {
        $class = ' active-trail';
        $l['attributes']['class'][] = 'active-trail';
      }
      // Normally, l() compares the href of every link with $_GET['q'] and sets
      // the active class accordingly. But local tasks do not appear in menu
      // trees, so if the current path is a local task, and this link is its
      // tab root, then we have to set the class manually.
      if ($item['link']['href'] == $router_item['tab_root_href'] && $item['link']['href'] != $_GET['q']) {
        $l['attributes']['class'][] = 'active';
      }
      // Keyed with the unique mlid to generate classes in theme_links().
      $links['menu-' . $item['link']['mlid'] . $class] = $l;
    }
  }
  return $links;
}

/**
 * Depending on render menu context array keys in menu array are different.
 * 
 * @param $context_name
 *   Render menu context name. Can be 'render-array' for menu block situation,
 *   or 'tree-array' for main/secondary menu situation.
 */
function menu_tag_context_keys($context_name) {
  static $context = array(
    'render-array' => array(
      'original_link' => '#original_link',
      'access' => '#access',
      'below' => '#below',
    ),
    'tree-array' => array(
      'original_link' => 'link',
      'access' => 'access',
      'below' => 'below',
    ),
  );
  return $context[$context_name];
}
