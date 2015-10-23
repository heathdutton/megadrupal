<?php

/**
 * @file
 * Context's shadow implementation of hook_block_usage().
 */

/**
 * Implements hook_block_usage().
 */
function context_block_usage($module, $delta) {
  $block = $module . '-' . $delta;

  $contexts = context_load(NULL);
  $matches = array();
  foreach ($contexts as $context) {
    if (isset($context->reactions['block']['blocks'][$block])) {
      $context_name = $context->name;
      $region_name = $context->reactions['block']['blocks'][$block]['region'];
      $matches[] = l($context_name, 'admin/structure/context/list/' . $context_name . '/edit', array('fragment' => 'edit-reactions-selector')) . ' (' . $region_name . ')';
    }
  }

  return $matches;
}
