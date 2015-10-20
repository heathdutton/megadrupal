<?php

/**
 * @file
 * Hooks provided by the Taxonomy Role Access module.
 */

/**
 * Act on taxonomy term access changes.
 *
 * @param int $tid
 *   Taxonomy term ID.
 */
function hook_taxonomy_tools_role_access_term_access_change($tid) {
  $term = taxonomy_term_load($tid);
  drupal_set_message($term->name);
}
