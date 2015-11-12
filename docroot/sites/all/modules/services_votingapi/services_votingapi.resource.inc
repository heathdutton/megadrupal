<?php

/**
 * @file
 * Service's resources callbacks.
 */

/**
 * Select votes or vote results.
 *
 * @param $type
 *  Type of voting results.
 *  Allowed values: 'votes' or 'results'
 *
 * @param $single
 *  Boolean flag. Indicates whether result should be single or not.
 *
 * @param $criteria
 *   A keyed array used to build the select query. Keys can contain
 *   a single value or an array of values to be matched.
 *   $criteria['vote_cache_id']     (If this is set, all other keys are skipped)
 *   $criteria['entity_id']
 *   $criteria['entity_type']
 *   $criteria['value_type']
 *   $criteria['tag']
 *   $criteria['uid'] (Only for 'votes' select type)
 *   $criteria['function'] (Only for 'results' select type)
 *   $criteria['timestamp']   (If this is set, records with timestamps
 *      GREATER THAN the set value will be selected.)
 *
 * @param $limit
 *   An optional integer specifying the maximum number of votes to return.
 *
 * @return array
 *   An array of vote results matching the criteria.
 */
function _services_votingapi_resource_select_votes($type, $single, $criteria, $limit) {

  if ($type == 'votes') {
    // Build single or multiple results.
    $select_function = $single ? 'votingapi_select_single_vote_value' : 'votingapi_select_votes';
  }
  elseif ($type == 'results') {
    // Build single or multiple results.
    $select_function = $single ? 'votingapi_select_single_result_value' : 'votingapi_select_results';
  }
  else {
    return services_error(t('Wrong votingapi type.'));
  }

  // Return selected votes or vote results.
  return call_user_func($select_function, $criteria, $limit);
}

/**
 *
 * Vote for an entity.
 *
 * @param $votes
 *   An array of votes, each with the following structure:
 *   $vote['entity_type']  (Optional, defaults to 'node')
 *   $vote['entity_id']    (Required)
 *   $vote['value_type']    (Optional, defaults to 'percent')
 *   $vote['value']         (Required)
 *   $vote['tag']           (Optional, defaults to 'vote')
 *   $vote['uid']           (Optional, defaults to current user)
 *   $vote['vote_source']   (Optional, defaults to current IP)
 *   $vote['timestamp']     (Optional, defaults to REQUEST_TIME)
 *
 * @param $criteria
 *   A keyed array used to determine what votes will be deleted when the current
 *   vote is cast. If no value is specified, all votes for the current content
 *   by the current user will be reset. If an empty array is passed in, no votes
 *   will be reset and all incoming votes will be saved IN ADDITION to existing
 *   ones.
 *   $criteria['vote_id']
 *   $criteria['entity_type']
 *   $criteria['entity_type']
 *   $criteria['value_type']
 *   $criteria['tag']
 *   $criteria['uid']
 *   $criteria['vote_source']
 *   $criteria['timestamp']   (If this is set, records with timestamps
 *      GREATER THAN the set value will be selected.)
 *
 * @return array
 *   Result of voting.
 *
 * @see votingapi_set_votes() to learn more.
 */
function _services_votingapi_resource_set_votes($votes, $criteria) {
  foreach ($votes as $key => $vote) {
    if (empty($vote['entity_id'])) {
      return services_error(t('Missing entity ID for vote ') . $key);
    }
    if (empty($vote['value'])) {
      return services_error(t('Missing votes value for vote ') . $key);
    }
  }
  return votingapi_set_votes($votes, $criteria);
}

