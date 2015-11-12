<?php


/**
 * Implements hook_dqx_adminmenu_items()
 *
 * See also field_ui_admin_menu_map().
 * We try to be smarter than that, see
 * 
 */
function field_ui_dqx_adminmenu_items($api) {

  foreach (entity_get_info() as $obj_type => $info) {
    foreach ($info['bundles'] as $bundle_name => $bundle_info) {

      $base = dqx_adminmenu_entity_bundle_admin_base_path($bundle_info, $bundle_name);

      if (!empty($base)) {

        // Assemble fields
        switch ($obj_type) {
          case 'user':
            $fields = field_info_fields('user');
            break;
          default:
            $fields = field_info_instances($obj_type, $bundle_name);
        }

        foreach ($fields as $field) {
          if ($fieldname = @$field['field_name']) {
            $path = "$base/fields/$fieldname";
            $api->addTree($path);
          }
        }
      }
    }
  }
}


/**
 * Implements hook_dqx_adminmenu_items_alter()
 */
function field_ui_dqx_adminmenu_items_alter($api) {

  foreach (entity_get_info() as $obj_type => $info) {
    foreach ($info['bundles'] as $bundle_name => $bundle_info) {

      $base = dqx_adminmenu_entity_bundle_admin_base_path($bundle_info, $bundle_name);

      if (!empty($base)) {

        $api->submenuSetInlineChild($base, "$base/fields");
      }
    }
  }
}
