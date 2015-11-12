<?php


/**
 * Implements hook_dqx_adminmenu_items()
 */
function civicrm_dqx_adminmenu_items($api) {
  $api->addTree('civicrm');
  $api->forceShowItem('civicrm');
}


/**
 * Implements hook_dqx_adminmenu_items_alter()
 */
function civicrm_dqx_adminmenu_items_alter($api) {
  $api->itemSetWeight('civicrm', 90);
  $api->moveItem('civicrm', 'admin/civicrm');
}
