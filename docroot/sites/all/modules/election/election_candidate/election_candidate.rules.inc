<?php
/**
 * @file
 * Rules integration file for the Election Candidate module.
 */

/**
 * Implements hook_rules_event_info().
 */
function election_candidate_rules_event_info() {
  $defaults = array(
    'group' => t('Election candidate'),
    'module' => 'election_candidate',
    'access callback' => 'election_candidate_rules_config_access',
  );
  $events = array();
  $events['election_candidate_nomination_submitted'] = $defaults + array(
    'label' => t('After submitting a nomination'),
    'variables' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('The user who submitted the nomination'),
        'skip save' => TRUE,
      ),
      'candidate' => array(
        'type' => 'election_candidate',
        'label' => t('The candidate'),
      ),
      'post' => array(
        'type' => 'election_post',
        'label' => t('The post for which the nomination was submitted'),
      ),
      'election' => array(
        'type' => 'election',
        'label' => t('The election to which the post belongs'),
      ),
    ),
  );
  return $events;
}

/**
 * Access callback for configuring Rules related to candidates.
 *
 * @see election_candidate_rules_event_info()
 */
function election_candidate_rules_config_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    return election_access('update') && entity_access('view', 'user');
  }
}
