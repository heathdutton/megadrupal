<?php
/**
 * Implements hook_entity_info().
 */
function oempro_lists_entity_info() {
  $return = array(
    'oempro_list' => array(
      'label' => t('Oempro List'),
      'plural label' => t('Oempro Lists'),
      'controller class' => 'EntityAPIControllerExportable',
      'base table' => 'oempro_lists',
      'uri callback' => 'oempro_list_uri',
      'fieldable' => FALSE,
      'exportable' => TRUE,
      'label callback' => 'oempro_lists_label',
      'module' => 'oempro_lists',
      'entity keys' => array(
        'id' => 'id',
        'name' => 'name',
      ),
      'bundle keys' => array(
        'bundle' => 'type',
      ),
      'bundles' => array(
        'oempro_list' => array(
          'label' => t('Oempro List'),
        ),
      ),
      'view modes' => array(
        'full' => array(
          'label' => t('Complete List'),
          'custom settings' => FALSE,
        ),
      ),
    ),
  );

  return $return;
}

/**
 * Loads a list by ID.
 */
function oempro_lists_load($list_id) {
  $lists = oempro_lists_load_multiple(array($list_id), array());
  return $lists ? reset($lists) : FALSE;
}

/**
 * Loads multiple registrations by ID or based on a set of matching conditions.
 *
 * @see entity_load()
 *
 * @param $list_ids
 * @param $conditions
 *   An array of conditions on the {oempro_list} table in the form
 *     'field' => $value.
 * @param $reset
 *   Whether to reset the internal contact loading cache.
 *
 * @return
 *   An array of contact objects indexed by registration_id.
 */
function oempro_lists_load_multiple($list_ids = array(), $conditions = array(), $reset = FALSE) {
  if (empty($list_ids)) {
    $list_ids = FALSE;
  }

  return entity_load('oempro_list', $list_ids, $conditions, $reset);
}

/**
 * Gets an array of all lists, keyed by the list name.
 *
 * @param $name
 *   If set, the list with the given name is returned.
 *
 * @return oempro_list[]
 *   Depending whether $name isset, an array of lists or a single one.
 */
function oempro_lists_load_multiple_by_name($name = NULL) {
  $lists = entity_load_multiple_by_name('oempro_list', isset($name) ? array($name) : FALSE);
  return isset($name) ? reset($lists) : $lists;
}

/**
 * Deletes multiple lists by ID.
 *
 * @param $list_ids
 *   An array of contact IDs to delete.
 *
 * @return
 *   TRUE on success, FALSE otherwise.
 */
function oempro_lists_delete_multiple($list_ids) {
  return entity_get_controller('oempro_list')->delete($list_ids);
}

/**
 * Saves a list.
 *
 * @param $list
 *   The full list object to save.
 *
 * @return
 *   The saved list object.
 */
function oempro_lists_save($list) {
  $list->updated = REQUEST_TIME;
  // Set the creation timestamp if not set.
  if (!isset($list->created) || empty($list->created)) {
    $list->created = REQUEST_TIME;
  }
  return entity_get_controller('oempro_list')->save($list);
}

function oempro_lists_label($list) {
  return $list->label;
}
