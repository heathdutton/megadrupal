<?php

/**
 * Check path access (taken from menu_valid_path)
 */
function muchomenu_has_access_to_path($path) {
  global $_muchomenu_menu_admin;
  $item = array();

  // We indicate that a menu administrator is running the menu access check.
  $_muchomenu_menu_admin = TRUE;
  if ($path == '<front>' || url_is_external($path)) {
    return TRUE;
  }
  elseif (preg_match('/\/\%/', $path)) {
    // Path is dynamic (ie 'user/%'), so check directly against menu_router table.

    $result = db_query("SELECT * FROM {menu_router} where path = '%s' ", $path);
    $item = $result->fetchAssoc();
    if ($item) {
      $item['link_path'] = $form_item['link_path'];
      $item['link_title'] = $form_item['link_title'];
      $item['external'] = FALSE;
      $item['options'] = '';
      _menu_link_translate($item);
    }
  }
  else {
    $item = menu_get_item($path);
  }
  $_muchomenu_menu_admin = FALSE;
  if ($item['access'] === FALSE) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * Determine the zebra, half, and order attributes of an item.
 *
 * This is a helper function to generate count based classes
 * based on an items position in the sequence and the total count of
 * items. These classes consist of an item's zebra (even/odd), half (half1/half2),
 * and order (first/last).
 *
 * @param $count
 *    An item's numerical position within a sequence.
 * @param $leafcount
 *    The total count of items
 * @return
 *    zebra (event/odd), halves (half1/half2), and order (first/last). clases are
 *    returned as a space delimited list.
 */
function _muchomenu_count_attributes($position, $total_count) {
  $zebra = ($position % 2) ? ' even' : ' odd';
  $halves = ($position < $total_count / 2) ? ' half-1' : ' half-2';
  if ($position == 0) {
    $order = ' first';
  }
  elseif ($position == $total_count - 1) {
    $order = ' last';
  }
  else {
    $order = '';
  }
  return $zebra . $halves . $order;
}

/**
 * Render a muchomenu
 */
function muchomenu_display($id, $full = FALSE) {
  if (!$id || !is_numeric($id)) {
    return FALSE;
  }
  $menus = muchomenu_tree_data($id, FALSE);
  $output = muchomenu_build_output($menus, variable_get('muchomenu_layout', 'vertical'));
  return $output;
}

/**
 * Invoked by muchomenu_display and itself
 */
function muchomenu_build_output($menus, $layout) {
  foreach ($menus as $menu) {
    $has_children = empty($menu['link']['has_children']) ? 0 : $menu['link']['has_children'];
    if ($menu['below']) {
      $output .= theme('muchomenu_item', array('0' => $menu['link'], '1' => $has_children, 'prefix' => $id, 'layout' => $layout, 'menu' => muchomenu_build_output($menu['below'], $layout)));
    }
    else {
      $output .= theme('muchomenu_item', array('0' => $menu['link'], '1' => $has_children, 'prefix' => $id, 'layout' => $layout, 'menu' => ''));
    }
  }
  return $output ? theme('muchomenu_tree', array('tree' => $output)) : '';
}
