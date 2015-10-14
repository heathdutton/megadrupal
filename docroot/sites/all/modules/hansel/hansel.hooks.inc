<?php

/**
 * @file
 * Hansel hook documentation
 */

/**
 * Define switch types.
 *
 * This hook is invoked by Hansel to get the available switch types. Modules
 * may use this to add switch types to Hansel.
 *
 * @see _hansel_get_switch_types()
 * @ingroup hooks
 */
function hook_hansel_switch_types() {
  return array(
    'url argument' => array(
      'compare' => 'hansel_switch_url_argument_compare',
      'info' => 'hansel_switch_url_argument_info',
      'config form' => 'hansel_switch_url_argument_config_form',
      'file' => 'hansel.switches.inc',
    ),
    'node id' => array(
      'compare' => 'hansel_switch_node_id_compare',
      'file' => 'hansel.switches.inc',
    ),
    'node type' => array(
      'compare' => 'hansel_switch_node_type_compare',
      'file' => 'hansel.switches.inc',
    ),
  );
}

/**
 * Define action types.
 *
 * This hook is onvoked by Hansel to get the available action types. Modules
 * may use this to add action types to Hansel.
 *
 * @see _hansel_get_action_types()
 * @ingroup hooks
 */
function hook_hansel_action_types() {
  return array(
    'add single link' => array(
      'get crumbs' => 'hansel_action_add_single_link_get_crumbs',
      'info' => 'hansel_action_add_single_link_info',
      'config form' => 'hansel_action_add_single_link_config_form',
      'file' => 'hansel.actions.inc',
    ),
    'add link to node' => array(
      'get crumbs' => 'hansel_action_add_link_to_node_get_crumbs',
      'file' => 'hansel.actions.inc',
    ),
    'add link to user' => array(
      'get crumbs' => 'hansel_action_add_link_to_user_get_crumbs',
      'config form' => 'hansel_action_add_link_to_user_config_form',
      'file' => 'hansel.actions.inc',
    ),
  );
}

/**
 * Get parent item for this path.
 * 
 * This function is used for the "add parents" breadcrumbs action. Any module
 * can extend this action to define new relations between pages, for example
 * a panels node can be defined as a parent of the node in the pane.
 * 
 * @param string $path 
 * @return array
 */
function hook_hansel_get_parent($path) {
  if ($has_parent) {
    return array(
      'path' => $parent_path,
      'title' => $parent_title,
    );
  }
  return FALSE;
}

/**
 * Alter the menu list used for activating the menu item based on the
 * breadcrumbs.
 * 
 * @param array $menus 
 */
function hook_hansel_menus_alter(&$menus) {
  $menus = array(
    'secondary-links',
    'primary-links',
  );
}
