<?php
/**
 * @file
 * Functions to allow exporting Felix configuration.
 */

/**
 * Export block set to object.
 *
 * @param string $name
 */
function felix_export_block_set($name) {
  $set = db_select('felix_block_set', 'bs')
    ->fields('bs')
    ->condition('bs.name', $name)
    ->execute()
    ->fetchObject();
  if (!$set) {
    return FALSE;
  }
  $set->blocks = db_select('felix_block_set_block', 'bsb')
    ->fields('bsb', array('module', 'delta'))
    ->condition('bsb.block_set', $name)
    ->execute()
    ->fetchAll();
  $set->nodetypes = db_select('felix_block_set_nodetype', 'bsn')
    ->fields('bsn', array('nodetype'))
    ->condition('bsn.block_set', $name)
    ->execute()
    ->fetchAll();
  $set->viewmodes = db_select('felix_block_set_viewmode', 'bsv')
    ->fields('bsv', array('viewmode'))
    ->condition('bsv.block_set', $name)
    ->execute()
    ->fetchAll();

  return $set;
}

/**
 * Export region to object.
 *
 * @param string $name
 */
function felix_export_region($name) {
  $region = db_select('felix_region', 'r')
    ->fields('r')
    ->condition('r.name', $name)
    ->execute()
    ->fetchObject();
  if (!$region) {
    return FALSE;
  }
  $region->data = unserialize($region->data);
  return $region;
}

/**
 * Delete block set.
 *
 * @param string $name
 */
function felix_delete_block_set($name) {
  db_delete('felix_block_set')
    ->condition('name', $name)
    ->execute();
  db_delete('felix_block_set_block')
    ->condition('block_set', $name)
    ->execute();
  db_delete('felix_block_set_nodetype')
    ->condition('block_set', $name)
    ->execute();
  db_delete('felix_block_set_viewmode')
    ->condition('block_set', $name)
    ->execute();
}

/**
 * Import block set from object.
 *
 * This will overwrite the block set with the same name, if any.
 *
 * @param object $set
 */
function felix_import_block_set($set) {
  if (is_array($set)) {
    // The input can be an array because we use features_var_export().
    $set = (object) $set;
  }
  if (!is_object($set) || empty($set->name)) {
    throw new Exception('Invalid block set');
  }

  // Delete old set, if any.
  felix_delete_block_set($set->name);

  // Import new set.
  drupal_write_record('felix_block_set', $set);
  foreach ($set->blocks as $block) {
    $block = (object) $block;
    $block->block_set = $set->name;
    drupal_write_record('felix_block_set_block', $block);
  }
  foreach ($set->nodetypes as $nodetype) {
    $nodetype = (object) $nodetype;
    $nodetype->block_set = $set->name;
    drupal_write_record('felix_block_set_nodetype', $nodetype);
  }
  foreach ($set->viewmodes as $viewmode) {
    $viewmode = (object) $viewmode;
    $viewmode->block_set = $set->name;
    drupal_write_record('felix_block_set_viewmode', $viewmode);
  }
}

/**
 * Delete region.
 *
 * @param string $name
 */
function felix_delete_region($name) {
  db_delete('felix_region')
    ->condition('name', $name)
    ->execute();
}

/**
 * Import region from object.
 *
 * This will overwrite the region with the same name, if any.
 *
 * @param object $region
 */
function felix_import_region($region) {
  if (is_array($region)) {
    // The input can be an array because we use features_var_export().
    $region = (object) $region;
  }
  if (!is_object($region) || empty($region->name)) {
    throw new Exception('Invalid region');
  }

  // Check if we need to rehash blocks.
  $data = db_select('felix_region', 'r')
    ->fields('r', array('data'))
    ->condition('r.name', $region->name)
    ->execute()
    ->fetchField();
  if ($data) {
    $data = unserialize($data);
    $rehash = FALSE;
    $removed = array_values(array_diff($data['hash'], $region->data['hash']));
    $added = array_values(array_diff($region->data['hash'], $data['hash']));
    $rehash = $removed || $added;
    if ($rehash) {
      module_load_include('inc', 'felix', 'felix.rehash');
      felix_batch_rehash_start($region, $added, $removed, array());
    }
  }

  // Delete old region, if any.
  felix_delete_region($region->name);

  // Import new region.
  $region->data = serialize($region->data);
  drupal_write_record('felix_region', $region);
}
