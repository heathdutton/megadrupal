<?php

/**
 * Load the data for a single menu.
 */
function muchomenu_load($id) {
  if (!is_numeric($id) || !muchomenu_exists($id)) {
    return FALSE;
  }

  $result = db_query("SELECT * FROM {muchomenu_menus} WHERE id = :id", array(':id' => $id));
  return $result->fetchAssoc();
}

/**
 * Load the data for a single menu item.
 */
function muchomenu_link_load($id) {
  $result = db_query("SELECT * FROM {muchomenu_items} WHERE id = :id", array(':id' => $id));
  return $result->fetchAssoc();
}

/**
 * Return an array of mucho menus defined in our data table
 */
function muchomenu_get_menus() {
  $output = array();
  $result = db_query('SELECT id, title FROM {muchomenu_menus} ORDER BY title');
  foreach ($result as $menu) {
    $output[$menu->id] = $menu->title;
  }
  return $output;
}

/**
 * Get all the menu items that belong to the specified menu.
 */
function muchomenu_get_items_by_menu($id) {
  $items = array();
  $result = db_query("SELECT * FROM {muchomenu_items} WHERE menu_id = :menu_id ORDER BY weight, title", array(':menu_id' => (int) $id));
  foreach ($result as $item) {
    $items[] = (array) $item;
  }
  return $items;
}

/**
 * Returns TRUE if menu with given menu id exists.
 */
function muchomenu_exists($id) {
  if (!$id) {
    return FALSE;
  }
  $count = db_query('SELECT COUNT(*) FROM {muchomenu_menus} WHERE id = :id', array(':id' => (int) $id))->fetchField();
  return ($count > 0);
}

/**
 * Finds the menu ID corresponding to a menu item
 */
function muchomenu_item_get_menu_id($item) {
  $menu_id = db_query('SELECT menu_id FROM {muchomenu_items} WHERE id = :id', array(':id' => $item['id']))->fetchField();
  return $menu_id;
}

/*
 * Invoked by _menu_tree_data
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function muchomenu_fetch_rows($id = 0, $full = FALSE, $depth = 0, $output = array()) {
  $entries = muchomenu_return_row($id, $depth);
  $depth++;
  foreach ($entries as $id => $entry) {
    if ($entry['enabled'] || $full) {
      $output[$id] = $entry;
    }
  }
  return $output;
}

/*
 * Helper function. Invoked exclusively by muchomenu_fetch_rows.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function muchomenu_return_row($id, $depth = 0) {
  $output = array();
  $result = db_query('SELECT * FROM {muchomenu_items} WHERE menu_id = :menu_id ORDER BY weight, title', array(':menu_id' => $id));
  foreach ($result as $item) {
    $output[$item->id] = array(
        'id' => $item->id,
        'menu_id' => $item->menu_id,
        'title' => $item->title,
        'alt' => $item->alt,
        'panel_name' => $item->panel_name,
        'description' => $item->description,
        'path' => $item->path,
        'enabled' => $item->enabled,
        'weight' => $item->weight,
        'depth' => $depth,
        'target' => $item->target,
        'disable_click' => $item->disable_click,
    );
  }
  return $output;
}

/**
 * Build the data representing a menu tree.
 *
 * Invoked by muchomenu_display and muchomenu_overview_form
 *
 * Params:
 * - $id - the id of the menu whose itemswe want to pull
 */
function muchomenu_tree_data($id, $full = TRUE) {
  $depth = 0;
  list(, $tree) = _muchomenu_tree_data($id, $depth, $full);
  _muchomenu_prune_tree($tree);
  return $tree;
}

/**
 * Recursive helper function to build the data representing a menu tree.
 */
function _muchomenu_tree_data($id, $depth, $full = TRUE, $previous_element = '', $parsed = 0) {
  $remnant = NULL;
  $tree = array();
  $menus = muchomenu_fetch_rows($id, $full);
  array_splice($menus, 0, $parsed);

  foreach ($menus as $item) {
    // The current item is the first in a new submenu.
    if ($item['depth'] > $depth) {
      // _menu_tree returns an item and the menu tree structure.
      list($item, $below) = _muchomenu_tree_data($id, $item['depth'], $full, $item, $parsed);
      if ($previous_element) {
        $tree[$previous_element['id']] = array(
            'link' => $previous_element,
            'below' => $below,
        );
      }
      else {
        $tree = $below;
      }
      // We need to fall back one level.
      if (!isset($item) || $item['depth'] < $depth) {
        return array($item, $tree);
      }
      // This will be the link to be output in the next iteration.
      $previous_element = $item;
    }
    // We are at the same depth, so we use the previous element.
    elseif ($item['depth'] == $depth) {
      if ($previous_element) {
        // Only the first time.
        $tree[$previous_element['id']] = array(
            'link' => $previous_element,
            'below' => FALSE,
        );
      }
      // This will be the link to be output in the next iteration.
      $previous_element = $item;
    }
    // The submenu ended with the previous item, so pass back the current item.
    else {
      $remnant = $item;
      break;
    }

    $parsed++;
  }
  if ($previous_element) {
    // We have one more link dangling.
    $tree[$previous_element['id']] = array(
        'link' => $previous_element,
        'below' => FALSE,
    );
  }
  return array($remnant, $tree);
}

/**
 * Recursive function to prune branches from the menu tree that the current user does not have access to
 * Also we add classes here.
 */
function _muchomenu_prune_tree(&$tree) {
  global $user;
  $n = 1;
  foreach ($tree as $key => &$branch) {
    $branch['link']['class'] = "muchomenu-item muchomenu-item-" . $branch['link']['id'];
    if ($n == 1) {
      $branch['link']['class'] .= " first";
    }
    $branch['link']['class'] .= $n % 2 ? ' odd' : ' even';
    if (!muchomenu_has_access_to_path($branch['link']['path'])) {
      unset($tree[$key]);
    }
    else {
      if ($branch['below']) {
        _muchomenu_prune_tree($branch['below']);
      }
    }
    if ($n == count($tree)) {
      $branch['link']['class'] .= " last";
    }
    $n++;
  }
}
