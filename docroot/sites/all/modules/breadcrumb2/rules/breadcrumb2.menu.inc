<?php

/**
 * @file
 * Contains rules integration for the breadcrumb2 module needed during menu related evaluation.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Condition: Internal path is.
 */
function breadcrumb2_rules_internal_path_is($path) {
  $menu_item = menu_get_item();
  if (isset($menu_item['path']) && $menu_item['path'] == $path) {
    return TRUE;
  }
  return FALSE;
}


/**
 * Action: Get argument from Drupal path.
 */
function breadcrumb2_rules_get_argument_from_path($index) {
  $component = arg($index);
  return array('component' => $component);
}

/**
 * Action: Menu get active trail.
 */
function breadcrumb2_rules_menu_get_active_trail() {
  $active_trail = array();
  $item = menu_get_item();
  if (!empty($item['access'])) {
    $active_trail = menu_get_active_trail();
    // Don't show a link to the current page in the breadcrumb trail.
    $end = end($active_trail);
    if ($item['href'] == $end['href']) {
      array_pop($active_trail);
    }

    if (($item['type'] & MENU_LINKS_TO_PARENT) == MENU_LINKS_TO_PARENT) {
      array_pop($active_trail);
    }
  }
  return array('active_trail' => $active_trail);
}


/**
 * Action: Menu set active menu names.
 */
function breadcrumb2_rules_menu_set_active_menu_names($menu_names = '') {
  $menu_names_array = explode(',', $menu_names);
  if (isset($menu_names_array) && is_array($menu_names_array)) {
    menu_set_active_menu_names($menu_names_array);
  }
}