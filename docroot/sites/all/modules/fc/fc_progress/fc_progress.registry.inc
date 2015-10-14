<?php
/**
 * @file
 * Field Complete Progress - Provides blocks to display a progress bar of Field completion - registry.
 */

/**
 * Implements hook_block_info().
 */
function _fc_progress_block_info() {
  $blocks = array();
  foreach (entity_get_info() as $entity_type => $entity_info) {
    if (empty($entity_info['uri callback']) || empty($entity_info['fieldable'])) {
      // There's no display URI for this entity so it'll never appear
      // rendered, or it's not fieldable so Field Complete doesn't apply
      continue;
    }

    $blocks[$entity_type] = array(
      'info' => t('Field Complete Progress: @type', array('@type' => $entity_info['label'])),
      'cache' => DRUPAL_NO_CACHE,
    );
  }
  return $blocks;
}

/**
 * Implements hook_theme().
 */
function _fc_progress_theme() {
  return array(
    'fc_progress_bar' => array(
      'variables' => array('entity_type' => '', 'entity' => NULL),
      'template' => 'fc-progress-bar',
      'file' => 'fc_progress.blocks.inc',
    ),
    'fc_progress_next' => array(
      'variables' => array('entity_type' => '', 'entity' => NULL, 'field_name' => ''),
      'template' => 'fc-progress-next',
      'file' => 'fc_progress.blocks.inc',
    ),
  );
}
