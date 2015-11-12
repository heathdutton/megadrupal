<?php

/**
 * @file
 * Hooks provided by the Entity External Rating module.
 */

/**
 * Responds when ratings are updated from the externa sources.
 *
 * @param array $rows
 *   An array that contains a list with rows that have been updated. Each
 *   element in the array is another array with the following elements:
 *     - rating_id: the id of the rating, string.
 *     - entity_id: the id of the entity, int.
 *     - entity_type: the type of the entity, string.
 *     - params: the parameters used to get the rating, array.
 *     - rating_total: the value of the rating, float.
 */
function hook_entity_ext_rating_updates($rows) {
  // Here, you can do whatever you want with the restults.
}

/**
 * Invoked to get the plugins list with their settings.
 */
function hook_entity_ext_rating_plugins() {
  module_load_include('inc', 'entity_ext_rating', 'entity_ext_rating.plugins');
  return _entity_ext_rating_entity_ext_rating_plugins();
}