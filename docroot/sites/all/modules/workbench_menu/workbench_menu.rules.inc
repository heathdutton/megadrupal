<?php

/**
 * @file
 * Rules integration file for node accessibility.
 */

/**
 * Implements hook_rules_file_info().
 */
function workbench_menu_rules_file_info() {
  $items = array();
  $items[] = 'workbench_menu.rules';

  return $items;
}

/**
 * Implements hook_rules_condition_info() on behalf of the node module.
 */
function workbench_menu_rules_condition_info() {
  $items = array();

  $items['workbench_menu_has_path'] = array(
    'group' => t("Workbench"),
    'label' => t("Workbench menu has path"),
    'base' => 'workbench_menu_has_path_rules_condition',
    'parameter' => array(
      'taxonomy_term' => array('type' => 'taxonomy_term', 'label' => t("Taxonomy Term"), 'optional' => FALSE),
      'path' => array('type' => 'text', 'label' => t("Path"), 'optional' => TRUE),
    ),
  );

  $items['workbench_menu_has_item_type'] = array(
    'group' => t("Workbench"),
    'label' => t("Workbench menu has item type"),
    'base' => 'workbench_menu_has_item_type_rules_condition',
    'parameter' => array(
      'taxonomy_term' => array('type' => 'taxonomy_term', 'label' => t("Taxonomy Term"), 'optional' => FALSE),
      'item_type' => array(
        'type' => 'text',
        'label' => t("Item Type"),
        'options list' => 'workbench_menu_rules_options_list_item_type',
        'restriction' => 'input',
        'optional' => FALSE,
      ),
    ),
  );

  $items['workbench_menu_has_node'] = array(
    'group' => t("Workbench"),
    'label' => t("Workbench menu has node"),
    'base' => 'workbench_menu_has_node_rules_condition',
    'parameter' => array(
      'taxonomy_term' => array('type' => 'taxonomy_term', 'label' => t("Taxonomy Term"), 'optional' => FALSE),
      'node' => array('type' => 'node', 'label' => t("Node")),
    ),
    'access callback' => 'rules_node_integration_access',
  );

  return $items;
}

/**
 * Implements hook_rules_action_info() on behalf of the node module.
 */
function workbench_menu_rules_action_info() {
  $items = array();

  $items['workbench_menu_clear_cache_by_access_id'] = array(
    'label' => t("Clear workbench menu cache by taxonomy"),
    'group' => t("Workbench"),
    'base' => 'workbench_menu_clear_cache_by_access_id_rules_action',
    'parameter' => array(
      'taxonomy_term' => array(
        'type' => 'taxonomy_term',
        'label' => t("Taxonomy Term"),
      ),
    ),
  );

  $items['workbench_menu_clear_cache_by_menu'] = array(
    'label' => t("Clear workbench menu cache by menu"),
    'group' => t("Workbench"),
    'base' => 'workbench_menu_clear_cache_by_menu_rules_action',
    'parameter' => array(
      'menu' => array(
        'type' => 'workbench_menu',
        'label' => t("Menu"),
      ),
    ),
  );

  $items['workbench_menu_clear_cache_at_path'] = array(
    'label' => t("Clear workbench menu cache at path"),
    'group' => t("Workbench"),
    'base' => 'workbench_menu_clear_cache_at_path_rules_action',
    'parameter' => array(
      'path' => array(
        'type' => 'text',
        'label' => t("Url Path"),
      ),
    ),
  );

  $items['workbench_menu_load_menu'] = array(
    'label' => t("Load workbench menu"),
    'group' => t("Workbench"),
    'base' => 'workbench_menu_load_menu_rules_action',
    'parameter' => array(
      'access_id' => array('type' => 'taxonomy_term', 'label' => t("Access ID")),
    ),
    'provides' => array(
      'menu' => array('type' => 'workbench_menu', 'label' => t("Menu")),
    ),
  );

  $items['workbench_menu_load_menu_item'] = array(
    'label' => t("Load workbench menu item"),
    'group' => t("Workbench"),
    'base' => 'workbench_menu_load_menu_item_rules_action',
    'parameter' => array(
      'item_id' => array('type' => 'integer', 'label' => t("Item ID")),
    ),
    'provides' => array(
      'menu_item' => array('type' => 'workbench_menu_item', 'label' => t("Menu Item")),
    ),
  );

  $items['workbench_menu_load_menus_by_path'] = array(
    'label' => t("Load workbench menus at path"),
    'group' => t("Workbench"),
    'base' => 'workbench_menu_load_menus_by_path_rules_action',
    'parameter' => array(
      'path' => array('type' => 'text', 'label' => t("Path"), 'optional' => TRUE),
    ),
    'provides' => array(
      'menus' => array('type' => 'list<workbench_menu>', 'label' => t("Menus")),
    ),
  );

  $items['workbench_menu_add_menu_item_node'] = array(
    'label' => t("Add workbench menu item - node"),
    'group' => t("Workbench"),
    'base' => 'workbench_menu_add_menu_item_node_rules_action',
    'parameter' => array(
      'menu' => array('type' => 'workbench_menu', 'label' => t("Menu")),
      'node' => array('type' => 'node', 'label' => t("Node")),
    ),
    'access callback' => 'rules_node_integration_access',
  );

  return $items;
}

/**
 * Condition: Check if workbench menu has a given path.
 *
 * @param object $taxonomy_term
 *   Taxonomy term associated with the menu.
 * @param $path
 *   (optional) A path to compare the menu path against.
 *
 * @return bool
 *   TRUE/FALSE depending on if the menu has a given path.
 */
function workbench_menu_has_path_rules_condition($taxonomy_term, $path = '') {
  if (!is_object($taxonomy_term)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('taxonomy_term');
    }
    return FALSE;
  }

  $access_id = $taxonomy_term->tid;

  if (!is_string($path)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_string('path');
    }
    return FALSE;
  }

  $menus = (array) workbench_menu_get_menus(array('access_id' => $access_id, 'fast' => NULL), 'id');

  if (empty($menus)) return FALSE;

  reset($menus);
  $menu = current($menus);

  if (!is_object($menu)) return FALSE;

  return $menu->path == $path;
}

/**
 * Condition: Check if workbench menu has a given item of the specified type.
 *
 * @param object $taxonomy_term
 *   Taxonomy term associated with the menu.
 * @param $item_type
 *   Item type to test for.
 *
 * @return bool
 *   TRUE/FALSE depending on if the menu has a given item of the specified
 *   type.
 */
function workbench_menu_has_item_type_rules_condition($taxonomy_term, $item_type) {
  if (!is_object($taxonomy_term)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('taxonomy_term');
    }
    return FALSE;
  }

  $access_id = $taxonomy_term->tid;
  $type = cf_db_options_get_options('workbench_menu', 'item_type', $item_type);

  if (empty($type)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_variable('item_type', "%item_type is not a valid item type.", array('%item_type' => $item_type));
    }
    return FALSE;
  }

  $menus = (array) workbench_menu_get_menus(array('access_id' => $access_id, 'fast' => NULL), 'id');

  if (empty($menus)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_variable('menu', 'The menu with an access id of %access_id was not found.', array('%access_id' => $access_id));
    }
    return FALSE;
  }

  reset($menus);
  $menu = current($menus);
  $items = (array) workbench_menu_get_menu_items(array('menu_id' => $menu->id, 'item_type' => $type->id, 'fast' => NULL), 'id');

  return !empty($items);
}

/**
 * Condition: Check if workbench menu has a given node.
 *
 * @param object $taxonomy_term
 *   The taxonomy term associated with the menu.
 * @param $node
 *   A node object to look for.
 *
 * @return bool
 *   TRUE/FALSE depending on if the menu has a given item of the specified
 *   type.
 */
function workbench_menu_has_node_rules_condition($taxonomy_term, $node) {
  if (!is_object($taxonomy_term)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('taxonomy_term');
    }
    return FALSE;
  }

  $access_id = $taxonomy_term->tid;

  if (!is_object($node)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('node');
    }

    return FALSE;
  }

  $menus = (array) workbench_menu_get_menus(array('access_id' => $access_id, 'fast' => NULL), 'id');

  if (empty($menus)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_variable('menu', 'The menu with an access id of %access_id was not found.', array('%access_id' => $access_id));
    }
    return FALSE;
  }

  reset($menus);
  $menu = current($menus);

  $items = (array) workbench_menu_get_menu_items(array('menu_id' => $menu->id, 'item_type' => 'item_node', 'fast' => NULL), 'id');

  foreach ($items as $key => &$item) {
    $properties = (array) unserialize($item->properties);

    if (empty($properties['node_id'])) continue;
    $matched = preg_match('@^' . $node->nid . '($|\?.*|#.*)@', $properties['node_id']);

    if (!$matched) {
      $matched = preg_match('@\[nid: ' . $node->nid . '\]\s*($|\?.*|#.*)@', $properties['node_id']);
    }

    if ($matched) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Action: Clear cache of a workbench menu for a given menu.
 *
 * @param object $taxonomy_term
 *   Taxonomy term associated with the menu.
 *
 * @return bool
 *   TRUE/FALSE depending on if the cache was cleared.
 */
function workbench_menu_clear_cache_by_access_id_rules_action($taxonomy_term) {
  if (!is_object($taxonomy_term)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('taxonomy_term');
    }
    return FALSE;
  }

  $access_id = $taxonomy_term->tid;
  $menus = (array) workbench_menu_get_menus(array('access_id' => $access_id, 'fast' => NULL), 'id');

  $processed = array();
  foreach ($menus as $menu) {
    workbench_menu_clear_cache_by_menu_id($menu->id, $processed);
  }

  return TRUE;
}

/**
 * Action: Clear cache of a workbench menu for a given menu.
 *
 * @param array $menu
 *   An array containing the menu object.
 *
 * @return bool
 *   TRUE/FALSE depending on if the cache was cleared.
 */
function workbench_menu_clear_cache_by_menu_rules_action($menu) {
  if (!cf_has_array_key('menu', $menu)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_array('menu');
    }
    return FALSE;
  }

  if (!is_object($menu['menu'])) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('menu[menu]');
    }
    return FALSE;
  }

  $processed = array();
  workbench_menu_clear_cache_by_menu_id($menu['menu']->id, $processed);

  return TRUE;
}

/**
 * Action: Clear cache of a workbench menu at a given path.
 *
 * $param string $path
 *   A url path to clear.
 *
 * @return bool
 *   TRUE/FALSE depending on if the cache was cleared.
 */
function workbench_menu_clear_cache_at_path_rules_action($path) {
  workbench_menu_clear_cache_at_path($path);

  return TRUE;
}

/**
 * Action: Load a single workbench menu using the access id.
 *
 * @param object $taxonomy_term
 *   The taxonomy term associated with the menu.
 *
 * @return
 *   FALSE on any failure and an array containing the menu object otherwise.
 */
function workbench_menu_load_menu_rules_action($taxonomy_term) {
  if (!is_object($taxonomy_term)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('taxonomy_term');
    }
    return FALSE;
  }

  $access_id = $taxonomy_term->tid;
  $menus = (array) workbench_menu_get_menus(array('access_id' => $access_id, 'fast' => NULL), 'id');

  if (empty($menus)) return FALSE;

  reset($menus);
  $menu = current($menus);

  if (!is_object($menu)) return FALSE;

  return array('menu' => $menu);
}

/**
 * Action: Load a single workbench menu item using the menu items ID.
 *
 * @param int $item_id
 *   The numeric item id representing the menu item.
 *
 * @return
 *   FALSE on any failure and an array containing the menu item object
 *   otherwise.
 */
function workbench_menu_load_menu_item_rules_action($item_id) {
  if (!is_numeric($item_id)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_numeric('item_id');
    }
    return FALSE;
  }

  $items = (array) workbench_menu_get_menu_items(array('id' => $item_id, 'fast' => NULL), 'id');

  if (empty($items)) return FALSE;

  reset($items);
  $item = current($items);

  if (!is_object($item)) return FALSE;

  return array('menu_item' => $item);
}

/**
 * Action: Load multiple workbench menus found at a given path.
 *
 * @param $path
 *   A path where the menus are located.
 *
 * @return
 *   An array of menu objects and an empty array on any problem.
 */
function workbench_menu_load_menus_by_path_rules_action($path) {
  if (!is_string($path)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_string('path');
    }

    return array('menus' => array());
  }

  $menus = (array) workbench_menu_get_menus(array('path' => $path, 'fast' => NULL), 'id');

  return array('menus' => &$menus);
}

/**
 * Action: Add menu item: node
 *
 * @param object $menu
 *   The workbench menu to add the item to.
 * @param object $node
 *   The node to associate with the menu item.
 *
 * @return
 *   TRUE/FALSE on success/failure.
 */
function workbench_menu_add_menu_item_node_rules_action($menu, $node) {
  if (!is_object($menu)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('menu');
    }
    return FALSE;
  }

  if (!is_object($node)) {
    if (class_exists('cf_error')) {
      cf_error::invalid_object('node');
    }
    return FALSE;
  }


  // get the default settings
  $settings = workbench_menu_get_menu_item_defaults();
  $settings['human_name'] = $node->title;
  $settings['item_type'] = 'item_node';
  $settings['properties']['node_id'] = $node->nid;
  $settings['parent_id'] = NULL;

  return workbench_menu_update_menu_item($menu, $settings);
}

/**
 * Get an array list of options for the item type.
 *
 * @return array
 *   The options list.
 */
function workbench_menu_rules_options_list_item_type() {
  return cf_db_options_get_options_list('workbench_menu', 'item_type');
}
