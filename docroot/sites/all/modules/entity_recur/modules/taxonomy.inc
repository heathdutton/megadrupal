<?php
/**
 * @file
 * Entity recur hooks for the node module.
 */

/**
 * Implements ENTITY_TYPE_entity_recur_recur_menu_path()
 */
function taxonomy_term_entity_recur_recur_menu_path() {
  return 'taxonomy/term/%taxonomy_term/recur';
}