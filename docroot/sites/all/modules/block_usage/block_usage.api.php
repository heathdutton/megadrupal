<?php

/**
 * Implements hook_block_usage().
 *
 * If you're Context, you'll check all your context objects.
 * If you're Block Reference, you'll find all host entities by fields.
 *
 * What other modules use blocks opaquely?
 */
function hook_block_usage($module, $delta) {
  $matches = array();

  foreach (do_some_magic_here() as $entity_or_something) {
    if ($entity_or_something->status) {
      $matches[] = $entity->admin_label;
    }
  }

  return $matches;
}
