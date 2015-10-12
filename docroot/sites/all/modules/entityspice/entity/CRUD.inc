<?php

/**
 * Create a new entity object.
 *
 * @param array $values
 *   Associative array of values. At least include ['type' => $type].
 */
function _entityspice_entity_create($etype, array $values = array()) {
  return entity_get_controller($etype)->create($values);
}

/**
 * Loads an entity by ID.
 */
function _entityspice_entity_load($etype, $id) {
  if (empty($id)) {
    return FALSE;
  }

  $entities = _entityspice_entity_load_multiple($etype, array($id));
  return $entities ? reset($entities) : FALSE;
}

/**
 * Loads multiple entities by ID or based on a set of matching conditions.
 *
 * @see entity_load()
 *
 * @param array $entity_ids
 *   An array of entity IDs.
 * @param array $conditions
 *   An array of conditions on the {entity_type} table in the form
 *   'field' => $value.
 * @param bool $reset
 *   Whether to reset the internal entity loading cache.
 *
 * @return array
 *   An array of entity objects indexed by entity_id.
 */
function _entityspice_entity_load_multiple($etype, array $ids = array(), array $conditions = array(), $reset = FALSE) {
  if (empty($ids) && empty($conditions)) {
    return array();
  }

  return entity_load($etype, $ids, $conditions, $reset);
}

function _entityspice_entity_delete($etype, $id) {
  return _entityspice_entity_delete_multiple($etype, array($id));
}

/**
 * Deletes multiple entities by ID.
 *
 * @param array $ids
 *   An array of entity IDs to delete.
 *
 * @return array
 *   TRUE on success, FALSE otherwise.
 */
function _entityspice_entity_delete_multiple($etype, array $ids) {
  return entity_get_controller($etype)->delete($ids);
}

/**
 * Saves an entity.
 *
 * @param $entity
 *   The full entity object to save.
 *
 * @return entity
 *   The saved entity object.
 */
function _entityspice_entity_save($etype, $entity) {
  $entity->save();
  return $entity;
}

/**
 * Display an entity.
 *
 * @param $sec
 *   A Fully loaded sec object.
 * @param boolean $not_found
 *   Return drupal_not_found() if $entity is not valid.
 *
 * @return array
 *   Render array.
 */
function _entityspice_entity_view($etype, $entity, $view_mode = 'full', $not_found = TRUE) {
  if ($entity = _entityspice_get_entity($etype, $entity)) {
    $view = $entity->view($view_mode);
  }
  elseif($not_found) {
    return drupal_not_found();
  }

  return $view;
}

