<?php


/**
 * Implements hook_dqx_adminmenu_items_alter()
 */
function menu_dqx_adminmenu_items_alter($api) {

  // Items that would have admin/structure/menu/manage as parent,
  // should rather have admin/structure/menu/list as parent.
  $api->parentMap('admin/structure/menu/manage', 'admin/structure/menu/list');

  // Children of admin/structure/menu/list should be shown inline.
  $api->submenuSetInlineChild('admin/structure/menu', 'admin/structure/menu/list');
}
