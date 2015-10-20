<?php

/**
 * Deletes a bundle from the db.
 *
 * @param entity $bundle
 *   The entity representing the bundle.
 */
function _entityspice_bundle_delete($etype, $bundle) {
  $bundle->delete();
  menu_rebuild();
  return $bundle;
}

/**
 * Load a entity bundle by string.
 *
 * @param string $bundle
 *   The machine-readable name of an entity bundle to load.
 *
 * @return array.
 *   An entity bundle array or FALSE if $bundle does not exist.
 */
function _entityspice_bundle_load($etype, $type) {
  return _entityspice_entity_get_bundles($etype, $type);
}

/**
 * Saves a model type to the db.
 */
function _entityspice_bundle_save($bundle) {
  $bundle->save();
  menu_rebuild();
  return $bundle;
}

