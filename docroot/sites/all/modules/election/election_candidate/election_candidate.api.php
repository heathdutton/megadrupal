<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Help determine whether the election post has enough candidates for voting.
 *
 * @see election_candidate_post_has_enough()
 *
 * @param bool &$enough
 *   Whether the post has enough candidates for voting to be possible.
 * @param object $post
 *   The election post object.
 * @param int $num_candidates
 *   The number of published, 'hopeful' candidates for this post.
 */
function hook_election_candidate_post_has_enough_alter(&$enough, $post, $num_candidates) {
  // Example: if the post has a custom setting (and some kind of custom voting
  // form) allowing "write-in" candidates, then there are always 'enough'
  // candidates.
  if (!empty($post->settings['allow_write_ins'])) {
    $enough = TRUE;
  }
}

/**
 * Alter the candidate's name before displaying it.
 *
 * @see election_candidate_get_name()
 *
 * @param string &$name
 *   The candidate's name.
 * @param object $candidate
 *   The candidate object.
 */
function hook_election_candidate_name_alter(&$name, $candidate) {
  // Example: for a custom candidate type, set the name to the rendered value
  // of a custom field called 'custom_label'.
  if ($candidate->type == 'custom_type' && isset($candidate->custom_label)) {
    $name = $candidate->custom_label[LANGUAGE_NONE][0];
    $name = field_view_value('election_candidate', $candidate, 'custom_label', $name);
    $name = drupal_render($name);
  }
}
