<?php
/**
 * @file
 * Election hooks for the Election Condition module.
 */

/**
 * Implements hook_election_vote_before_grant().
 */
function election_condition_election_vote_before_grant($post, $account) {

  // Call all the callbacks for all the conditions. All the callbacks must exist
  // and return TRUE; otherwise, voting access will be denied.
  foreach ($post->conditions as $condition) {
    foreach ($condition['callbacks'] as $callback) {
      if (!function_exists($callback) || $callback($post, $account) !== TRUE) {
        return FALSE;
      }
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
    foreach ($condition['callbacks'] as $callback) {
      if (!function_exists($callback) || $callback($post, $account) !== TRUE) {
        $limitations['condition_' . $machine_name]['result'] = FALSE;
        break;
      }
      $limitations['condition_' . $machine_name]['result'] = TRUE;
    }
  }

}
