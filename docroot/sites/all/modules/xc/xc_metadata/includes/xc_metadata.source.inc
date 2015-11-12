<?php
/**
 * @file
 * XC Metadata source functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Get source
 *
 * @param $source_id
 *    Source identifier
 * @param $update
 *    TRUE to refresh source cache
 * @return (Object)
 *    Source object (fields: source_id, name, description, type)
 */
function xc_source_get($source_id, $update = FALSE) {
  static $sources;
  if (!is_object($sources[$source_id]) || $update) {
    $sql = 'SELECT source_id, name, description, type
            FROM {xc_source} WHERE source_id = %d';
    $result = db_query('SELECT source_id, name, description, type
            FROM {xc_source} WHERE source_id = :source_id', array(':source_id' => $source_id));
    $source = $result->fetchObject();
    $sources[$source_id] = $source;
  }
  return $sources[$source_id];
}

/**
 * Get all sources
 *
 * @return (Array)
 *   List of data objects containing xc_source records (fields: source_id, name,
 *   description, type)
 */
function xc_source_get_all() {
  static $sources;
  if (empty($sources)) {
    $sql = 'SELECT source_id, name, description, type FROM {xc_source}';
    $result = db_query($sql);
    foreach ($result as $source) {
      $sources[] = $source;
    }
  }
  return $sources;
}

/**
 * Load source object
 *
 * @see xc_source_get()
 * @param $source_id
 *    Source identifier
 * @return
 *    Source object
 */
function xc_source_load($source_id) {
  return xc_source_get($source_id);
}

/**
 * Get default storage location for a source
 *
 * @param $source_id
 *    Source identifier
 * @return
 *    Storage location object
 */
function xc_source_get_default_location($source_id) {
  static $cache;
  if (empty($cache[$source_id])) {
    $sql = 'SELECT location_id
            FROM {xc_source_locations}
            WHERE source_id = :source_id
            ORDER BY weight ASC';
    $cache[$source_id] = db_query_range($sql, 0, 1,array(':source_id' => $source_id))->fetchField();
  }
  $location_id = $cache[$source_id];
  return xc_location_get($location_id);
}

/**
 * Get all storage locations for a source
 *
 * @param $source_id
 *    Source identifier
 * @return
 *    An array of location objects for a source. The array contains location_id, name,
 *    description, types and weight of each locations.
 */
function xc_source_get_locations($source_id) {
  static $cache;

  if (empty($cache[$source_id])) {
    $locations = array();
    $result = db_query('SELECT location_id, weight
            FROM {xc_source_locations}
            WHERE source_id = :source_id
            ORDER BY weight ASC',
      array(':source_id' => $source_id)
    );
    foreach ($result as $_location) {
      $locations[$_location->location_id] = xc_location_get($_location->location_id);
      if (is_object($locations[$_location->location_id])) {
        $locations[$_location->location_id]->weight = $_location->weight;
      }
    }
    $cache[$source_id] = $locations;
  }
  return $cache[$source_id];
}

/**
 * Set a source object to the database
 *
 * @param $source
 *    Source object
 * @return
 *    New source identifier
 */
function xc_source_set(&$source) {
  $update = FALSE;
  $record = FALSE;
  if (isset($source->source_id)) {
    $sql = 'SELECT * FROM {xc_source} WHERE source_id = %d';
    $record = db_query('SELECT * FROM {xc_source} WHERE source_id = :source_id', array(':source_id' => $source->source_id))->fetchObject();
  }

  if ($record) {
    $update = TRUE;
  }
  else {
    $record = new stdClass();
  }
  $record->name = $source->name;
  $record->description = !empty($source->description) ? $source->description : '';
  $record->type = $source->type;

  if ($update) {
    $result = drupal_write_record('xc_source', $record, 'source_id');
  }
  else {
    $result = drupal_write_record('xc_source', $record);
    $source->source_id = $record->source_id;
  }
  if (!$result) {
    drupal_set_message(t('Unexpected error. Unable to save metadata source'), 'error');
  }
  // Also set locations
  if (empty($source->source_id)) {
    $source->source_id = db_last_insert_id("{xc_source}", 'source_id');
  }
  xc_source_set_locations($source);
  return $source->source_id;
}

/**
 * Set a source's locations to the database
 *
 * @param $source
 *    Source object
 */
function xc_source_set_locations($source) {
  if (!isset($source->source_id) || !is_numeric($source->source_id)) {
    return;
  }
  db_delete('xc_source_locations')
  ->condition('source_id', $source->source_id)
  ->execute();

  if (!isset($source->locations) || !is_array($source->locations)) {
    return;
  }

  foreach ($source->locations as $location_id => $location) {
    if (!is_numeric($location_id)) {
      continue;
    }
    $record = new stdClass();
    $record->source_id = $source->source_id;
    $record->location_id = $location_id;
    $record->weight = $location->weight;
    $result = drupal_write_record('xc_source_locations', $record);

    if (!$result) {
      drupal_set_message(t('Unexpected error. Unable to save metadata source with its locations'), 'error');
    }
  }
}

/**
 * Unset a source from the database
 *
 * @param $source
 *    Source object
 */
function xc_source_unset($source) {
  $result = db_delete('xc_source')
              ->condition('source_id', $source->source_id)
              ->execute();
  if (!$result) {
    drupal_set_message(t('Unexpected error. Unable to remove metadata source'), 'error');
  }
}

