<?php

/**
 * @file
 * Hooks provided by the Taxonomy Tools Publisher module.
 */

/**
 * Act on scheduled term publishing.
 */
function hook_taxonomy_tools_publisher_terms_publish() {
  drupal_set_message('Scheduled term publishing completed.');
}

/**
 * Act on scheduled term unpublishing.
 */
function hook_taxonomy_tools_publisher_terms_unpublish() {
  drupal_set_message('Scheduled term unpublishing completed.');
}

/**
 * Act on term publishing.
 *
 * @param stdClass $term
 *   Taxonomy term object.
 */
function hook_taxonomy_tools_publisher_term_publish($term) {
  drupal_set_message($term->name);
}

/**
 * Act on term unpublishing.
 *
 * @param stdClass $term
 *   Taxonomy term object.
 */
function hook_taxonomy_tools_publisher_term_unpublish($term) {
  drupal_set_message($term->name);
}
