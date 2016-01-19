<?php
/**
 * @file
 * Field Incomplete - Provides a block displaying of what's currently incomplete on an entity - blocks.
 */

/**
 * Implements hook_block_view().
 */
function _fc_incomplete_block_view($entity_type) {
  $entity = fc_get_page_entity($entity_type);

  // No entity available, so skip it.
  if (empty($entity)) {
    return array();
  }

  module_load_include('inc', 'fc_incomplete');
  $entity_info = entity_get_info($entity_type);

  return array(
    'subject' => t('Incomplete items'),
    'content' => fc_incomplete_build($entity_type, $entity)->render(),
  );
}
