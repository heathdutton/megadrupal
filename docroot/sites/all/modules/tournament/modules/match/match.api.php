<?php

/**
 * @file
 * Hooks provided by the match module.
 */

/**
 * Act on a match participant access check.
 *
 * @param $node
 *   The match node that the action is being made upon.
 * @param $ids
 *   An array of participant IDs for access checks
 * @param $account
 *   Optional, a user object representing the user for whom the operation is to
 *   be performed. Determines access for a user other than the current user.
 *
 * @return
 *   Array of participant IDs from the given $ids, if any, for
 *   which the current user has access over.
 *
 * @see match_access()
 *
 * @ingroup match_api_hooks
 */
function hook_match_participants_access($node, $ids, $account) {
  if ($node->entity_type == 'team') {
    $teams = team_load_multiple($ids);

    foreach ($ids as $key => $id) {
      if (isset($teams[$id]->members[$account->uid])) {
        if ($teams[$id]->members[$account->uid] != TEAM_MEMBER_ROLE_ADMIN) {
          // The given user is not an admin of the team
          unset($ids[$key]);
        }
      }
      else {
        // The given user is not a member of the team
        unset($ids[$key]);
      }
    }

    return $ids;
  }
}

/**
 * Act on a match challenge access check.
 *
 * @param $entity_type
 *   The participant entity type
 * @param $entity
 *   The challenge opponent entity object.
 *
 * @return
 *   MATCH_CHALLENGE_ACCESS_DENIED to deny access. Do not return anything
 *   to allow access.
 *
 * @see match_challenge_form()
 *
 * @ingroup match_api_hooks
 */
function hook_match_challenge_access($entity_type, $entity) {
  if ($entity_type == 'team') {
    global $user;

    // Team challenging is globally disabled
    if (!variable_get('team_challenging', 1)) {
      return MATCH_CHALLENGE_ACCESS_DENIED;
    }

    // Current user is a member of the opponent
    if (isset($entity->members[$user->uid])) {
      return MATCH_CHALLENGE_ACCESS_DENIED;
    }
  }
}

/**
 * Act on a match finishing.
 *
 * @param $node
 *   The match node.
 *
 * @see match_action_form_submit()
 *
 * @ingroup match_api_hooks
 */
function hook_match_finish($node) {
  if (isset($node->tournament_node) && $node->tournament_node->type == 'round_robin') {
    round_robin_match_process($node, $node->tournament_node);
  }
}

/**
 * Break down match participants into an array of its user IDs
 *
 * @param $entity_type
 *   Participant entity type
 * @param $entity
 *   An entity object
 *
 * @return
 *   An array of user IDs that are members of the given participant.
 *
 * @see tournament_stats_match_finish()
 *
 * @ingroup match_api_hooks
 */
function hook_match_participants_extract($entity_type, $entity) {
  if ($entity_type == 'team') {
    return array_keys($entity->members);
  }
}
