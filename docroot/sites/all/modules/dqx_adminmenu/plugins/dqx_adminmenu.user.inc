<?php


/**
 * Implements dqx_adminmenu_items()
 */
function user_dqx_adminmenu_items($api) {

  if ($uid = @$GLOBALS['user']->uid) {
    $api->addTree("user/$uid");
    $api->addTree('user');
    $api->forceShowItem('user/logout');
  }
}


/**
 * Implements dqx_adminmenu_items_alter()
 */
function user_dqx_adminmenu_items_alter($api) {

  $api->parentMap('user', 'dqx-adminmenu-user');

  if ($uid = @$GLOBALS['user']->uid) {
    $api->itemSetWeight("user/$uid", -99);
    $api->itemSetWeight('user/logout', -100);
  }
}


/**
 * Implements dqx_adminmenu_structure_alter()
 */
function user_dqx_adminmenu_structure_alter($api) {

  // $api->submenuSetInlineChild('admin/config/people', 'admin/config/people/accounts');
  $api->submenuSetInlineChild('admin/people', 'admin/people/permissions');
}
