<?php

/**
 * @file
 * Relation resources specific code.
 */

/**
 * Callback for the 'create' method.
 */
function _relation_services_relation_resource_create($relation_data) {
  try {
    $relation = (object) $relation_data;
    relation_save($relation);

    return $relation;
  }
  catch (Exception $exception) {
    watchdog_exception('relation', $exception);
    return services_error($exception, 406, $relation);
  }
}

/**
 * Callback for the 'update' method.
 */
function _relation_services_relation_resource_update($rid, $relation) {
  return _relation_services_relation_resource_create($relation);
}

/**
 * Return an array of relatoins.
 *
 * @param int $page
 *   Page number of results to return (in pages of 20).
 * @param int $page_size
 *   Integer number of items to be returned.
 *
 * @return array
 *   An array of node objects.
 */
function _relation_services_relation_resource_index($page, $page_size) {
  $rids = db_select('relation', 'r')
    ->orderBy('created', 'DESC')
    ->fields('r', array('rid'))
    ->range($page * $page_size, $page_size)
    ->execute()
    ->fetchCol();

  $relations = relation_load_multiple($rids);

  return services_resource_build_index_list($relations, 'relation', 'rid');
}
