<?php


/**
 * Implements hook_dqx_adminmenu_items()
 */
function entity_dqx_adminmenu_items($api) {

  // We need this to make the title callback work correctly.
  module_load_include('ui.inc', 'entity', 'includes/entity');
}


/**
 * Implements hook_dqx_adminmenu_items_alter()
 */
function entity_dqx_adminmenu_items_alter($api) {

  foreach (entity_ui_controller() as $type => $controller) {
    $info = entity_get_info($type);
    if ($path = @$info['admin ui']['path']) {
      $api->parentMap("$path/manage", "$path/list");
      $api->submenuSetInlineChild($path, "$path/list");
    }
  }
}

