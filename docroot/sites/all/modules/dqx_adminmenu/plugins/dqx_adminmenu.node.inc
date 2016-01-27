<?php


/**
 * Implements hook_dqx_adminmenu_items()
 */
function node_dqx_adminmenu_items($api) {

  // Add the tree starting with node/add
  $api->addTree('node/add');

  foreach ($types = node_type_get_types() as $type => $info) {
    $slug = str_replace('_', '-', $type);
    $api->addTree($path = "admin/structure/types/manage/$slug");
  }
}


/**
 * Implements hook_dqx_adminmenu_items_alter()
 */
function node_dqx_adminmenu_items_alter($api) {

  // Everything that would have "node" as parent,
  // should rather have "admin/content" as parent.
  $api->parentMap('node', 'admin/content');

  // Everything that would have "admin/structure/types/manage" as parent,
  // should rather have "admin/structure/types/list" as parent.
  $api->parentMap('admin/structure/types/manage', 'admin/structure/types/list');
}


/**
 * Implements hook_dqx_adminmenu_structure_alter()
 */
function node_dqx_adminmenu_structure_alter($api) {

  $api->submenuSetInlineChild('admin/content', 'node/add');
  $api->submenuSetInlineChild('admin/structure/types', 'admin/structure/types/list');
}



