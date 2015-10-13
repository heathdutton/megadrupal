<?php


/**
 * @file
 * Contains database functions for the Menu to Taxonomy module.
 */

/**
 * Inserts/updates a Menu to Taxonomy sync record.
 *
 * @param array $record
 *   The Menu to taxonomy sync record.
 */
function _menu_to_taxonomy_record_save(array $record) {
  if (empty($record['mlid']) || empty($record['tid']) || empty($record['vid'])) {
    throw new Exception(t('Incorrect Menu to taxonomy sync record.'));
  }
  $new = _menu_to_taxonomy_record_exists($record['tid']);
  // Save if the record does not exist, otherwise update the existing record:
  if (empty($new)) {
    drupal_write_record('menu_to_taxonomy', $record);
  }
  else {
    drupal_write_record('menu_to_taxonomy', $record, 'tid');
  }
}

/**
 * Checks to see if a specific sync record exists.
 *
 * @param int $tid
 *   The term id.
 *
 * @return bool
 *   The count of matches (which should be 1 or 0).
 */
function _menu_to_taxonomy_record_exists($tid) {
  return (bool) db_query("SELECT COUNT(tid) FROM {menu_to_taxonomy} WHERE tid = :tid", array(':tid' => $tid))->fetchField();
}

/**
 * Retrieves the term / menu relations for a vocabulary.
 *
 * @param int $vid
 *   The vocabulary id.
 *
 * @return array
 *   Array with as key the term id and as value the menu link ID.
 */
function _menu_to_taxonomy_get_menu_items($vid) {
  $result = db_query('SELECT tid, mlid FROM {menu_to_taxonomy} WHERE vid = :vid', array(':vid' => $vid));
  $menu_items = $result->fetchAllKeyed();
  return $menu_items;
}

/**
 * Deletes all terms linked to menu items associated with this vocabulary.
 *
 * @param int $vid
 *   The vocabulary's id.
 */
function _menu_to_taxonomy_delete_all_terms($vid) {
  $menu_terms = _menu_to_taxonomy_get_menu_items($vid);
  if (!empty($menu_terms)) {
    foreach ($menu_terms as $tid => $mlid) {
      // This will also delete the sync record.
      taxonomy_term_delete($tid);
    }
  }
}

/**
 * Gets the term for a given menu link and vocabulary.
 *
 * @param int $mlid
 *   The menu link ID.
 * @param int $vid
 *   The vocabulary ID.
 *
 * @return array
 *   The term IDs.
 */
function _menu_to_taxonomy_get_tid($mlid, $vid) {
  $result = db_select('menu_to_taxonomy', 'm')
    ->fields('m', array('tid'))
    ->condition('mlid', $mlid)
    ->condition('vid', $vid)
    ->execute()
    ->fetchField();
  return $result;
}
