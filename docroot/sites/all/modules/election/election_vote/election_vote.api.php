<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Act before voting access is granted for a user.
 *
 * @see election_condition.election.inc
 *
 * @param object $post
 *   An election post object.
 * @param object $account
 *   A Drupal user account object.
 *
 * @return mixed
 *   Return FALSE to deny voting access. Other return values have no effect.
 */
function hook_election_vote_before_grant($post, $account) {
  // Deny access to the user with the UID 37491.
  if ($account->uid == 37491) {
    return FALSE;
  }
}

/**
 * Alter the voting access explanation given to the user.
 *
 * The explanation consists of a list of access limitations, and the results of
 * testing those limitations (pass / fail), presented to the user at the menu
 * item 'election-post/%election_post/explain' if they have the permission
 * 'view voting access explanation'.
 *
 * @param array &$limitations
 *   An array of limitations on voting access, provided by default mechanisms
 *   (such as role limitations), by other modules, or by Rules.
 * @param object $post
 *   An election post object.
 * @param object $account
 *   A Drupal user account object.
 */
function hook_election_vote_access_explain_alter(&$limitations, $post, $account) {
  $limitations['module_my_module']['explanation'] = t("You can't be user 37491.");
}

/**
 * Save votes on submission of the voting form, for this election type.
 *
 * This hook runs inside the same database transaction that saves 'ballots' to
 * the {election_ballot} table. To roll back the transaction, return FALSE or
 * throw an exception.
 *
 * @param int $ballot_id
 *   The {ballot}.ballot_id of the ballot being saved.
 * @param object $post
 *   The election post object.
 * @param array $vote_form
 *   The Form API array for the voting form.
 * @param array $vote_form_state
 *   The Form API 'form state' array for the voting form.
 *
 * @return bool
 *   Return TRUE on success, or FALSE on failure. Votes and ballots are not
 *   saved unless this implementation returns TRUE.
 */
function hook_election_vote_ELECTION_TYPE_save($ballot_id, $post, $vote_form, $vote_form_state) {
  // Get the answer out of the form.
  $answer = $vote_form_state['values']['answer'];

  // Save a vote corresponding to this ballot ID.
  db_insert('election_vote')
    ->fields(array(
      'election_id' => $post->election_id,
      'ballot_id' => $ballot_id,
      'post_id' => $post->post_id,
      'answer' => $answer,
    ))
    ->execute();

  return TRUE;
}

/**
 * Alter the value of a ballot before it is saved.
 *
 * @param int &$value
 *   The value of the ballot, normally 1.
 * @param object $election
 *   The election entity.
 * @param object $post
 *   The post that will be voted for.
 * @param object $account
 *   The user who is voting.
 */
function hook_election_vote_ballot_value_alter(&$value, $election, $post, $account) {
  // The administrator's votes should be counted twice.
  if (in_array('administrator', $account->roles)) {
    $value = 2;
  }
}
