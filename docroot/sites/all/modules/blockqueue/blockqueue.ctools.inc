<?php

/**
 * @file
 * CTools file.
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function blockqueue_ctools_plugin_api($owner, $api) {
  if ($owner == 'blockqueue' && $api == 'default_blockqueues') {
    return array('version' => 1);
  }
}

/**
 * Implements hook_default_blockqueue().
 */
function blockqueue_default_blockqueue() {
  $export = array();

  $blockqueue              = new stdClass();
  $blockqueue->disabled    = TRUE;
  $blockqueue->api_version = 1;
  $blockqueue->name        = 'default';
  $blockqueue->admin_title = 'Default';
  $blockqueue->size        = 0;
  $blockqueue->blocks      = array();
  $blockqueue->settings    = array();
  $export['default']       = $blockqueue;

  return $export;
}

/**
 * Implements hook_export_crud_new().
 */
function blockqueue_export_crud_new($set_defaults = TRUE) {
  $blockqueue = ctools_export_new_object('blockqueue', $set_defaults);

  if ($set_defaults) {
    $blockqueue->blocks   = array();
    $blockqueue->settings = array();
  }

  return $blockqueue;
}

/**
 * Implements hook_export_crud_load().
 */
function blockqueue_export_crud_load($name) {
  $result = ctools_export_load_object('blockqueue', 'names', array($name));

  if (isset($result[$name])) {
    $blockqueue = $result[$name];

    if ($blockqueue->export_type != EXPORT_IN_CODE) {
      if (is_string($blockqueue->blocks)) {
        $blockqueue->blocks = unserialize($blockqueue->blocks);
      }
      if (is_string($blockqueue->settings)) {
        $blockqueue->settings = unserialize($blockqueue->settings);
      }
    }

    return $blockqueue;
  }
}

/**
 * Implements hook_export_crud_load_all().
 */
function blockqueue_export_crud_load_all($name) {
  $blockqueues = ctools_export_load_object('blockqueue', 'all');

  foreach ($blockqueues as &$blockqueue) {
    if ($blockqueue->export_type != EXPORT_IN_CODE) {
      if (is_string($blockqueue->blocks)) {
        $blockqueue->blocks = unserialize($blockqueue->blocks);
      }
      if (is_string($blockqueue->settings)) {
        $blockqueue->settings = unserialize($blockqueue->settings);
      }
    }
  }

  return $blockqueues;
}

/**
 * Implements hook_export_crud_save().
 */
function blockqueue_export_crud_save(&$object) {
  if ($object->export_type & EXPORT_IN_DATABASE) {
    // Existing record.
    $update = array('bqid');
  }
  else {
    // New record.
    $update              = array();
    $object->export_type = EXPORT_IN_DATABASE;
  }

  if (!is_array($object->blocks)) {
    $object->blocks = array();
  }

  if (!is_array($object->settings)) {
    $object->settings = array();
  }

  $blocks   = $object->blocks;
  $settings = $object->settings;

  // Serialize for database storing.
  $object->blocks   = serialize($blocks);
  $object->settings = serialize($settings);

  $record = drupal_write_record('blockqueue', $object, $update);

  // Revert previous serialize operation.
  $object->blocks   = $blocks;
  $object->settings = $settings;

  // Flush block info.
  cache_clear_all(NULL, 'cache_block');

  return $record;
}

/**
 * Implements hook_export_crud_delete().
 */
function blockqueue_export_crud_delete($object) {
  if (!is_object($object)) {
    $object = ctools_export_crud_load('blockqueue', $object);
  }

  // Removes blocks.
  $blocks = db_select('block', 'b')
    ->fields('b', array('bid'))
    ->condition('module', 'blockqueue')
    ->condition('delta', $object->name)
    ->execute()
    ->fetchAllAssoc('bid');

  foreach ($blocks as $bid => $block) {
    db_delete('block')->condition('bid', $bid)->execute();
  }

  // Removes block role restrictions.
  db_delete('block_role')
    ->condition('module', 'blockqueue')
    ->condition('delta', $object->name)
    ->execute();

  // Removes blockqueue item.
  db_delete('blockqueue')
    ->condition('name', $object->name)
    ->execute();

  cache_clear_all(NULL, 'cache_block');
}

/**
 * Load Blockqueue from machine name.
 *
 * @param string $name
 *   Machine name.
 *
 * @return bool|object
 *   Blockqueue object is found, FALSE otherwise.
 */
function blockqueue_load($name) {
  ctools_include('export');

  if ($blockqueue = ctools_export_crud_load('blockqueue', $name)) {
    return $blockqueue;
  }

  return FALSE;
}

/**
 * Load all Blockqueue.
 *
 * @param bool $reset
 *   Reset.
 *
 * @return array
 *   Objects.
 */
function blockqueue_load_all($reset = FALSE) {
  ctools_include('export');

  return ctools_export_crud_load_all('blockqueue', $reset);
}

/**
 * Save a block queue.
 *
 * @param object $blockqueue
 *   Block queue to save.
 *
 * @return bool|object
 *   Block queue updated or FALSE in case of error.
 */
function blockqueue_save($blockqueue) {
  ctools_include('export');

  if (ctools_export_crud_save('blockqueue', $blockqueue)) {
    return $blockqueue;
  }

  return FALSE;
}
