<?php


/**
 * Implements hook_dqx_adminmenu_items()
 */
function system_dqx_adminmenu_items($api) {

  // Add entire tree with admin/*
  $api->addTree('admin');

  // Submenus for entity bundles.
  foreach (entity_get_info() as $obj_type => $info) {
    foreach ($info['bundles'] as $bundle_name => $bundle_info) {

      $base = dqx_adminmenu_entity_bundle_admin_base_path($bundle_info, $bundle_name);

      if (!empty($base)) {
        $items = $api->addTree($base);
      }
    }
  }
}


/**
 * Implements hook_dqx_adminmenu_items_alter()
 */
function system_dqx_adminmenu_items_alter($api) {

  $api->removeItem('admin/index');
  $api->removeItem('admin/tasks');
  $api->removeItem('admin/compact');

  $api->removeItem('admin/modules/list/confirm');
  $api->removeItem('admin/modules/uninstall/confirm');

  $api->itemSetTitle('admin/appearance/update', t('Update themes'));
  $api->itemSetTitle('admin/appearance/list', t('List available themes'));
  $api->itemSetTitle('admin/appearance/settings', t('Theme settings'));
}


/**
 * Implements hook_dqx_adminmenu_structure_alter()
 */
function system_dqx_adminmenu_structure_alter($api) {

  $api->submenuSetInlineChild('admin/appearance', 'admin/appearance/settings');
  $api->submenuSetInlineChild('admin/config/content', 'admin/config/content/formats');
  $api->submenuSetInlineChild('admin/config/search', 'admin/config/search/path');
}

