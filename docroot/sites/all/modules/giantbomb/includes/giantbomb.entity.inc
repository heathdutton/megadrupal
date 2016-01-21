<?php
/**
 * @file
 * Provide entity and database related functions.
 */

/**
 * Gets title for a Giant Bomb type.
 *
 * hook_menu() compatible.
 *
 * @param array $endpoint
 *   A fully loaded endpoint definition.
 * @param object $type
 *   A fully loaded Giant Bomb type definition. 
 *
 * @return string|bool
 *   Title for type, or FALSE.
 */
function giantbomb_type_title($endpoint, $type) {
  $types = giantbomb_types_all($endpoint['name'], 'object');
  $object = $type->object;
  if (isset($types[$object]->title)) {
    return t(
      '@endpoint @type',
      array('@endpoint' => $endpoint['title'], '@type' => $types[$object]->title),
      array('context' => 'giantbomb')
    );
  }

  return FALSE;
}

/**
 * Get all known Giant Bomb types keyed by object name.
 *
 * @param string $endpoint
 *   A Giant Bomb endpoint name.
 * @param string $key
 *   Any column in {giantbomb_types}.
 * @param bool $reset
 *   Reset internal fast cache.
 */
function giantbomb_types_all($endpoint, $key = 'object', $reset = FALSE) {
  $fast = &drupal_static(__FUNCTION__, array());

  if ($reset) {
    $fast = array();
  }

  if (!isset($fast[$endpoint][$key])) {
    $result = db_select('giantbomb_types', 'gbt')
      ->fields('gbt', array('id', 'object', 'list', 'entity_type', 'bundle', 'title', 'update_frequency', 'settings'))
      ->condition('endpoint', $endpoint)
      ->addTag('giantbomb_types')
      ->execute()
      ->fetchAllAssoc($key);
    foreach ($result as &$row) {
      $row->settings = unserialize($row->settings); 
    }
    $fast[$endpoint][$key] = $result;
  }

  return $fast[$endpoint][$key]; 
}

function giantbomb_jobs_all($endpoint, $reset = FALSE) {
  $fast = &drupal_static(__FUNCTION__, array());

  if ($reset) {
    $fast = array();
  }

  if (!isset($fast[$endpoint])) {
    $result = db_select('giantbomb_jobs', 'jobs')
      ->fields('jobs', array('id', 'resource', 'type', 'object_id', 'settings', 'updated', 'next_update', 'update_frequency', 'active'))
      ->condition('endpoint', $endpoint)
      ->addTag('giantbomb_jobs')
      ->execute()
      ->fetchAllAssoc('id');

    foreach ($result as &$row) {
      $row->settings = unserialize($row->settings); 
    }

    $fast[$endpoint] = $result;
  }

  return $fast[$endpoint]; 
}

/**
 * Loads a single type from the database.
 *
 * hook_menu() menu wildcard loader compatible.
 *
 * @param string $endpoint
 *   A Giant Bomb endpoint name.
 * @param string $type
 *   Source type.
 *
 * @return object|bool
 *   A type object, or FALSE.
 *
 * @see giantbomb_menu().
 */
function giantbomb_type_load($endpoint, $type) {
  $types = giantbomb_types_all($endpoint, 'object');
  return isset($types[$type]) ? $types[$type] : FALSE;
}

function giantbomb_job_load($id) {
  $result = db_select('giantbomb_jobs', 'jobs')
    ->fields('jobs', array('endpoint', 'id', 'resource', 'type', 'object_id', 'settings', 'updated', 'next_update', 'update_frequency', 'active'))
    ->condition('id', $id)
    ->execute()
    ->fetchAllAssoc('id');

  if ($result && isset($result[$id])) {
    $job = $result[$id];
    $job->settings = unserialize($job->settings);
    return $job;
  }

  return FALSE;
}