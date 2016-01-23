<?php


/**
 * Implements hook_dqx_adminmenu_items()
 *
 * TODO:
 * We want to reuse more and more of existing admin_menu functionality.
 * This is just the beginning.
 */
function admin_menu_dqx_adminmenu_items($api) {

  // Get placeholder expansion arguments from hook_admin_menu_map()
  // implementations.
  module_load_include('inc', 'admin_menu', 'admin_menu.map');

  $expand_map = _dqx_adminmenu_admin_menu_expand_map();

  foreach ($expand_map as $route => $expand_info) {
    if (!empty($expand_info['arguments'])) {
      foreach ($expand_info['arguments'] as $i => $arguments) {
        $variations = array($route => TRUE);
        foreach ($arguments as $wildcard => $replacements) {
          $variations_refined = array();
          foreach ($replacements as $replacement) {
            foreach ($variations as $variation => $true) {
              $variation_refined = str_replace($wildcard, $replacement, $variation);
              $variations_refined[$variation_refined] = TRUE;
            }
          }
          $variations = $variations_refined;
        }
        foreach ($variations as $variation => $true) {
          $api->addTree($variation);
        }
      }
    }
  }
}


/**
 * Implements hook_dqx_adminmenu_items_alter()
 *
 * TODO:
 * We want to reuse more and more of existing admin_menu functionality.
 * This is just the beginning.
 */
function admin_menu_dqx_adminmenu_items_alter($api) {

  // $api->parentMap('admin/structure/menu/manage', 'admin/structure/menu/list');

  // $api->submenuSetInlineChild('admin/structure/menu', 'admin/structure/menu/list');
}


function _dqx_adminmenu_admin_menu_expand_map() {

  $expand_map = array();

  // We might want to kick some module implementations.
  $modules = module_implements('admin_menu_map');
  $modules = array_combine($modules, $modules);

  // We are smarter than this..
  unset($modules['field_ui']);
  unset($modules['node']);
  unset($modules['taxonomy']);

  foreach ($modules as $module) {
    $f = $module . '_' . 'admin_menu_map';
    if (function_exists($f)) {
      $result = $f();
      if (!isset($result)) {
        // Skip.
      }
      elseif (is_array($result)) {
        $expand_map = array_merge_recursive($expand_map, $result);
      }
      else {
        $expand_map[] = $result;
      }
    }
  }

  // Allow modules to alter the expansion map.
  drupal_alter('admin_menu_map', $expand_map);

  return $expand_map;
}
