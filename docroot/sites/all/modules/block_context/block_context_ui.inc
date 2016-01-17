<?php
/**
 * @file
 * Handles theme related functions such as html menus, dropdowns and links
 */

// PATHS

/**
 * UI Block Context Paths Menu
 *
 * $paths           : array of main_menu and block paths
 */
function ui_paths_menu($paths) {

  // All link
  $menu = '<li>' . l('All', 'admin/structure/block') . '</li>';

  // Path links
  foreach ($paths as $path => $title) {
    $menu = $menu . '<li>' . l($title, 'admin/structure/block/path/' . $path) . '</li>';
  }

  return $menu;
}

/**
 * UI Selected Paths Dropdown Menu
 *
 * $all_paths       : array of main_menu paths/titles and all block paths
 * $block_key       : in the form 'module_delta'
 * $block_paths     : array of paths that this block is enabled for
 *
 * returns          : html dropdown menu for the block
 */
function ui_paths_dropdown($all_paths, $block_key, $block_paths) {

  // create menu
  $menu = '';

  // create appropriate dropdown menu title
  $menu_title = $block_paths[0] != '' ? 'Selected paths:' : 'All paths';

  // only create dropdown menu items for blocks with paths
  if (count($block_paths) != 0) {

    // for each path item in the $all_paths array
    foreach ($all_paths as $path => $title) {

      // match the blocks latest paths current $all_paths path
      if (block_context_block_paths_matches_path($block_paths, $path)) {
        $menu = $menu . '<option selected="selected" value="' . $path . '">' . block_context_clip($path) . '</option>';
      }
      // or create normal dropdown item
      else {
        $menu = $menu . '<option value="' . $path . '">' . block_context_clip($path) . '</option>';
      }
    }
  }
  // wrap menu items inside select menu
  $menu = '<select multiple="multiple" title="' . $menu_title . '" class="block-context" name="blocks['  .  $block_key  .  '][paths][]">'.  $menu  .  '</select>';
  return $menu;
}

// ROLES

/**
 * UI Block Context Roles Menu
 *
 * $roles   : array of roles in the form of key => role (1 => anonymous)
 */
function ui_roles_menu($roles) {

  // All link
  $menu = '<li>' . l('All', 'admin/structure/block/role') . '</li>';

  // Path links
  foreach ($roles as $rid => $role) {
    $menu = $menu . '<li>' . l($role, 'admin/structure/block/role/' . $rid) . '</li>';
  }

  return $menu;
}

/**
 * UI Selected Roles Dropdown Menu
 *
 * $block_key       : in the form 'module_delta'
 * $block_roles     : array of roles that this block is enabled for
 *
 * returns          : html dropdown menu for the block
 */
function ui_roles_dropdown($block_key, $block_roles) {

  // define system user roles
  $all_roles = user_roles();

  // create appropriate dropdown menu title
  $title = count($block_roles) > 0 ? 'title="Selected roles:"' : 'title="All roles"';

  // create menu
  $menu = '';

  // for each system role
  foreach ($all_roles as $rid => $role) {
 
    // no link if one of block roles matches all roles
    if (block_context_block_roles_matches_role($block_roles, $rid) ) {
      $menu = $menu . '<option selected="selected" value="' . $rid . '">' . $role . '</option>';
    }
    // deselect link if path is not current
    else {
      $menu = $menu . '<option value="' . $rid . '">' . $role . '</option>';
    }
  }
  // wrap menu items inside select menu
  $menu ='<select multiple="multiple" ' . $title . ' class="block-context" name="blocks['  .  $block_key  .  '][rid][]">'.  $menu  .  '</select>';
  return $menu;
}


// NODE TYPES

/**
 * UI Block Context Node types Menu
 *
 * $node_types : array of node types in the form of key => node_type (0 => page)
 */
function ui_node_types_menu($node_types) {

  // All link
  $menu = '<li>' . l('All', 'admin/structure/block/node_type') . '</li>';

  // Path links
  foreach ($node_types as $node_type => $node_name) {
    $menu = $menu . '<li>' . l($node_name, 'admin/structure/block/node_type/' . $node_type) . '</li>';
  }

  return $menu;
}

/**
 * UI Selected Node types Dropdown Menu
 *
 * $block_key       : in the form 'module_delta'
 * $block_roles     : array of node types that this block is enabled for
 *
 * returns          : html dropdown menu for the block
 */
function ui_node_types_dropdown($block_key, $block_node_types) {

  // define system node types
  $all_node_types = block_context_node_types();

  // create appropriate dropdown menu title
  $title = count($block_node_types) > 0 ? 'title="Selected node types:"' : 'title="All node types"';

  // create menu
  $menu = '';

  // for each system node type
  foreach ($all_node_types as $node_type => $node_name) {
  
    // select link if one of the block node types matches the dropdown items current node type
      if (block_context_block_node_types_matches_node_type($block_node_types, $node_type)) {
        $menu = $menu . '<option selected="selected" value="' . $node_type . '">' . block_context_clip($node_name) . '</option>';
      }
    
    // deselect link if node type is not found
    else {
      $menu = $menu . '<option value="' . $node_type . '">' . block_context_clip($node_name) . '</option>';
    }
  }
  // wrap menu items inside select menu, then add a node_types_list()
  $menu ='<select multiple="multiple" ' . $title . ' class="block-context" name="blocks['  .  $block_key  .  '][node_type][]">'.  $menu  .  '</select>';
  return $menu;
}



// HTML HELPERS

/*
 * Block Context Clip
 * clip text for HTML display
 */
function block_context_clip($string, $number = 33) {

	$clip = substr($string, 0, $number) == $string ? $string : substr($string, 0, $number) . '...';
	return htmlspecialchars($clip);
}