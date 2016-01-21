<?php
/**
 * @file
 * Election hooks for the Election Condition module.
 */

/**
 * Implements hook_election_vote_before_grant().
 */
function election_condition_election_vote_before_grant($post, $account) {

  // Test all the conditions. All of them must pass for voting access to be
  // granted.
  foreach ($post->conditions as $condition) {
    if (!election_condition_test($condition, $post, $account)) {
      return FALSE;
    }
  }

}

/**
 * Implements hook_election_vote_access_explain_alter().
 */
function election_condition_election_vote_access_explain_alter(&$limitations, $post, $account) {

  // Remove the default explanation.
  unset($limitations['module_election_condition']);

  // Test and explain all of the defined conditions.
  foreach ($post->conditions as $machine_name => $condition) {
    $limitations['condition_' . $machine_name] = array(
      'explanation' => isset($condition['user explanation']) ? $condition['user explanation'] : $condition['name'],
    );
    $limitations['condition_' . $machine_name]['result'] = election_condition_test($condition, $post, $account);
  }

}
