<?php

/**
 * @file
 * Hooks provided by the Pathrevisions module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allow modules to respond to a path revision being inserted.
 *
 * @param $path
 *   An associative array containing the following keys:
 *   - nid: The node nid.
 *   - vid: The node vid.
 *   - alias: The URL alias.
 *   - pid: Unique path alias identifier.
 *   - language: The language of the alias.
 *
 * @see pathrevisions_save()
 */
function hook_pathrevisions_insert($path) {
  db_insert('mytable')
    ->fields(array(
      'nid' => $path['nid'],
      'vid' => $path['nid'],
      'alias' => $path['alias'],
      'pid' => $path['pid'],
    ))
    ->execute();
}

/**
 * Allow modules to respond to a path revision being updated.
 *
 * @param $path
 *   An associative array containing the following keys:
 *   - nid: The node nid.
 *   - vid: The node vid.
 *   - alias: The URL alias.
 *   - pid: Unique path alias identifier.
 *   - language: The language of the alias.
 *
 * @see pathrevisions_save()
 */
function hook_pathrevisions_update($path) {
  db_update('mytable')
    ->fields(array('alias' => $path['alias'], 'pid' => $path['pid']))
    ->condition(array('nid' => $path['nid'], 'vid' => $path['vid']))
    ->execute();
}

/**
 * Allow modules to respond to a path being deleted.
 *
 * @param $paths
 *   An array of paths, each of which is an associative array containing the
 *   following keys:
 *   - nid: The node nid.
 *   - vid: The nod vid.
 *   - alias: The URL alias.
 *   - pid: Unique path alias identifier.
 *   - language: The language of the alias.
 *
 * @see pathrevisions_delete()
 */
function hook_pathrevisions_delete($paths) {
  db_delete('mytable')
    ->condition(array('nid' => $path['nid'], 'vid' => $path['vid']))
    ->execute();
}

/**
 * @} End of "addtogroup hooks".
 */
