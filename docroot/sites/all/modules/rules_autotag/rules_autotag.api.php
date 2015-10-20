<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */


/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the term array used for autotagging.
 *
 * @param array $terms
 *   An array of arrays, keyed by splitted term names.
 * @param object $vocabulary
 *   The vocabulary the terms belong to.
 */
function hook_rules_autotag_terms_alter(&$terms, $vocabulary) {
  // Always add these words.
  $terms['additional'] = array(
    'tid' => 1,
    'splitted' => TRUE,
    'original_term_name' => 'additional word',
  );
  $terms['marketing'] = array(
    'tid' => 2,
    'splitted' => FALSE,
    'original_term_name' => 'marketing',
  );
}

/**
 * Alter the term DB query.
 *
 * @param SelectQuery $query
 *   The query object ot alter.
 */
function hook_query_rules_autotag_terms_alter($query) {
  // Join the original term query from rules_autotag with our field table to
  // exclude terms that should not be tagged.
  $query->leftJoin('field_data_field_dont_autotag', 'fd', "t.tid = fd.entity_id AND fd.entity_type = 'taxonomy_term'");
  $query->condition(
    db_or()
      ->condition('fd.field_dont_autotag_value', NULL)
      ->condition('fd.field_dont_autotag_value', '1', '<>'));
}

/**
 * @} End of "addtogroup hooks".
 */
