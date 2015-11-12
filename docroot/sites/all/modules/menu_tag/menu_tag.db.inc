<?php

/**
 * @file
 * Storage functions for menu_tag module.
 */

/**
 * Loads menu tag from storage.
 * 
 * @param $mlid
 *   Menu link id.
 */
function menu_tag_db_load_menu_item_tag($mlid) {
  return db_query('SELECT menu_tag FROM {menu_tag} WHERE mlid = :mlid', array(':mlid' => $mlid))
    ->fetchField();
}

/**
 * Saves or updates menu tag.
 * 
 * Note that empty menu tag is also a valid state: a menu tag is not defined 
 * for a specific menu item but we will still save that state.
 * 
 * @param $mlid
 *   Menu link id.
 * @param $menu_tag
 *   Menu tag value. Can be empty.
 */
function menu_tag_db_save_menu_item_tag($mlid, $menu_tag) {
  db_merge('menu_tag')
    ->key(array('mlid' => $mlid))
    ->fields(array(
      'mlid' => $mlid,
      'menu_tag' => $menu_tag,
    ))
    ->updateFields(array(
      'menu_tag' => $menu_tag,
    ))
    ->execute();
}

/**
 * Delete menu tag from storage.
 * 
 * @param $mlid
 *   Menu link id.
 */
function menu_tag_db_delete_menu_item_tag($mlid) {
  db_delete('menu_tag')
    ->condition('mlid', $mlid)
    ->execute();
}

/**
 * Returns array of all current menu tags.
 */
function menu_tag_db_load_menu_tags() {
  $menu_tags = array();
  $res = db_query('SELECT DISTINCT menu_tag FROM {menu_tag} WHERE menu_tag <> \'\'');
  foreach ($res as $record) {
    $menu_tags[$record->menu_tag] = $record->menu_tag;
  }
  return $menu_tags;
}

/**
 * Loads menu tag for a given menu or menu block.
 * 
 * @param $id
 *   Block delta value or menu id.
 */
function menu_tag_db_load_menu_tag($id) {
  return variable_get('menu_tag_' . $id, '');
}

/**
 * Saves menu tag for a given menu or menu block.
 * 
 * @param $id
 *   Block delta value or menu id.
 */
function menu_tag_db_save_menu_tag($id, $menu_tag) {
  return variable_set('menu_tag_' . $id, $menu_tag);
}

/**
 * Clear cache for all menu tags of a given menu name.
 * 
 * @param type $menu_name 
 *   Menu name which menu tags cache we want to clear.
 */
function menu_tag_db_cache_clear($menu_name) {
  cache_clear_all('menu_tag:' . $menu_name, 'cache');
}
