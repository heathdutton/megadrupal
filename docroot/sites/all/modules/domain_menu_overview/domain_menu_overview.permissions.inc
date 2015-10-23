<?php
/**
 * @file
 * Domain menu overview permissions file
 * This will delcare the advanced permissions for the menu like admin menu.
 */

function _domain_menu_overview_menu_overview_page() {
  if (user_access('administer menu')) {
    return menu_overview_page();
  }

  return FALSE;
}

function _domain_menu_overview_access() {
  if (user_access('administer menu')) {
    return TRUE;
  }
  $perms = array_keys(domain_menu_overview_permission());
  foreach ($perms as $perm) {
    if (user_access($perm)) {
      return TRUE;
    }
  }
  return FALSE;
}

function _domain_menu_overview_menu_access($menu = NULL) {
  global $_domain;

  if (user_access('administer menu')
    || user_access('administer '. $menu['menu_name'] . '_' .
      $_domain['domain_id'])) {
    return TRUE;
  }
  return FALSE;
}

function _domain_menu_overview_menu_link_access($menu_link = NULL) {
  global $_domain;

  if (user_access('administer menu')
    || user_access('administer '. $menu_link['menu_name'] . '_' . $_domain['domain_id'])) {
    return TRUE;
  }
  return FALSE;
}
