<?php
/**
 * @file
 * Election hooks for the Election Candidate module.
 */

/**
 * Implements hook_election_vote_before_grant().
 */
function election_candidate_election_vote_before_grant($post, $account) {
  // Prevent users from voting when there aren't enough candidates.
  if (!election_candidate_post_has_enough($post)) {
    return FALSE;
  }
}

/**
 * Implements hook_election_vote_access_explain_alter().
 */
function election_candidate_election_vote_access_explain_alter(&$limitations, $post, $account) {

  // Remove the explanation if it isn't relevant.
  if (!election_candidate_check_support($post->election)) {
    unset($limitations['module_election_candidate']);
    return;
  }

  // Add an explanation for this module's vote access limitation.
  $limitations['module_election_candidate']['explanation'] = t('There must be enough candidates.');

}
