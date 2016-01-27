<?php

/**
 * @file
 * Link general menu functionalities to services module.
 */
include_once 'services_menu.vo.inc';

/**
 * Callback for retrieving menu resources.
 *
 * @param $menu_name
 *   Machine name of menu that is to be retrieved
 *
 * @return object
 *   ServicesMenu object
 */
function _services_menu_retrieve($menu_name) {

  $result = new ServicesMenu($menu_name);

  $menu = (object) menu_build_tree($menu_name);
  $result->tree = _services_menu_serialize_data($menu);

  return $result;
}

/**
 * Serializes the menu tree data.
 *
 * @param $data
 *   A fully built menu tree
 *
 * @return
 *   Serialized menu array
 */
function _services_menu_serialize_data($data) {

  $result = array();

  foreach ($data as $item_key => $item_value) {
    $item = new ServicesMenuItem();

    foreach ($item->link as $link_key => $link_value) {
      if (isset($item_value['link'][$link_key]))
        $item->link->$link_key = $item_value['link'][$link_key];
    }

    $item->link->path = $item_value['link']['link_path'];
    $item->link->path_alias = drupal_lookup_path('alias', $item->link->path);

    if (isset($item_value['below']))
      $item->children = _services_menu_serialize_data($item_value['below']);

    $result[$item->link->title . ' | ' . $item->link->mlid] = $item;
  }

  return $result;
}