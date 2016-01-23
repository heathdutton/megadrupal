<?php

/**
 * Get the menu name associated with the given nodequeue name
 *
 * @param $nodqueue_name -- String machine name of nodequeue
 * @return string of menu name.
 */

function nodequeue_menu_mapper_get_menu_for_nodequeue($nodqueue_name) {
  // Query string to get query menu name.
  $menu_name = db_select('nodequeue_menu_mapper', 'nmm')
    ->fields('nmm', array('menu_name'))
    ->condition('nodequeue_name', $nodqueue_name, '=')
    ->execute()
    ->fetchField();

  return $menu_name;
}

/**
 * Get the Nodequeue name associated with the given menu name
 *
 * @param $menu_name -- String machine name of menu
 * @return string of nodequeue name.
 */
function nodequeue_menu_mapper_get_nodequeue_for_menu($menu_name) {
  // Query string to get query nodequeue name.
  $queue_name = db_select('nodequeue_menu_mapper', 'nmm')
    ->fields('nmm', array('nodequeue_name'))
    ->condition('menu_name', $menu_name, '=')
    ->execute()
    ->fetchField();

  return $queue_name;
}

/**
 * Return a map of currently assigned menu name to queue name values
 *
 * @return array
 *   A array of nodequeues, keyed by assigned menu names.
 */

function nodequeue_menu_mapper_assigned_nodequeue_menus() {
  static $map = array();
  if (!$map) {
    $result = db_query('SELECT menu_name, nodequeue_name FROM {nodequeue_menu_mapper}');
    while ($get = $result->fetchObject()) {
      $map[$get->menu_name] = $get->nodequeue_name;
    }
  }
  return $map;
}

/* This function gets the "pretty" name for a nodequeue associated with a QID.
*
* @param $menu_name -- Int - Qid for a queue
* @return string for nodequeue name
*/
function nodequeue_menu_mapper_get_nodequeue_name_for_qid($qid) {
  // Query string to get menu name.
  $queue_name = db_select('nodequeue_queue', 'n')
    ->fields('n', array('title'))
    ->condition('qid', $qid, '=')
    ->execute()
    ->fetchField();

  return $queue_name;
}
