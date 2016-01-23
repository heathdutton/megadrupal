<?php
/**
 * @file
 * taxonomy_override queries used in module
 */

function _taxonomy_override_get_association_by_machine_name($machine_name) {
  $callback = &drupal_static(__FUNCTION__);
  if (!isset($callback)) {
    $query = db_select('taxonomy_override', 'tao');
    $query
      ->condition('tao.machine_name', $machine_name, '=')
      ->fields('tao', array('taoid', 'machine_name', 'callback_function'))
      ->range(0, 1);

    $callback = $query->execute()->fetchAssoc();
  }
  return $callback;
}

/**
 *
 * Adds an association machine_name <=> callback in the database.
 *
 * @param $machine_name
 * @param $callback
 */
function _taxonomy_override_add_association($machine_name, $callback) {
  $toid = db_insert('taxonomy_override')
    ->fields(array(
    'machine_name' => $machine_name,
    'callback_function' => $callback
    ))->execute();

  return $toid;
}

/**
 *
 * Updates an association by machine_name.
 *
 * @param $machine_name
 * @param $callback
 */
function _taxonomy_override_update_association($machine_name, $callback) {
  $updated = db_update('taxonomy_override')
    ->fields(
      array('callback_function' => $callback)
    )
    ->condition('machine_name', $machine_name, '=')
    ->execute();

  return $updated;
}

function _taxonomy_overide_delete_association_by_machine_name($machine_name) {
  $deleted = db_delete('taxonomy_override')
    ->condition('machine_name', $machine_name, '=')
    ->execute();
}
