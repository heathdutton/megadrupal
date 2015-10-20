<?php
/**
 * @file
 * Field Incomplete - Provides a block displaying of what's currently incomplete on an entity - registry.
 */

/**
 * Implements hook_block_info().
 */
function _fc_incomplete_block_info() {
  $blocks = array();
  foreach (entity_get_info() as $entity_type => $entity_info) {
    if (empty($entity_info['uri callback']) || empty($entity_info['fieldable'])) {
      // There's no display URI for this entity so it'll never appear
      // rendered, or it's not fieldable so Field Complete doesn't apply
      continue;
    }

    $blocks[$entity_type] = array(
      'info' => t('Field Incomplete: @type', array('@type' => $entity_info['label'])),
      'cache' => DRUPAL_NO_CACHE,
    );
  }
  return $blocks;
}

/**
 * Implements hook_theme().
 */
function _fc_incomplete_theme() {
  return array(
    'fc_incomplete_header' => array(
      'variables' => array('complete' => FALSE, 'name' => '', 'field_name' => '', 'entity_type' => '', 'entity' => NULL, 'bundle' => '', 'depth' => 0),
      'template' => 'fc-incomplete-header'
    ),
    'fc_incomplete_complete' => array(
      'variables' => array('complete' => FALSE),
      'file' => 'fc_incomplete.themes.inc',
    ),
    'fc_incomplete_entity_name' => array(
      'variables' => array('entity_type' => '', 'depth' => 0),
      'file' => 'fc_incomplete.themes.inc',
    ),
  );
}
