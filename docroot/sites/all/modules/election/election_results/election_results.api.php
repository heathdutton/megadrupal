<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Alter the results display page at election-post/%election_post/results.
 *
 * @param array &$output
 *   The render array for the results page.
 * @param object $post
 *   The election post object.
 */
function hook_election_results_page_alter(&$output, $post) {
  // Example: add some extra content to the results page.
  $output['extra'] = array(
    '#markup' => t('Here is some more content for the results page.'),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );
}
