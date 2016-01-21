<?php

/**
 * @file
 * Rules extensions managing statistics for Tutor questions.
 *
 * Note that these actions are custom-written to work with the statistics
 * entities that comes with this module. If fields are removed or changed, these
 * actions will break. Sorry for making it so custom-written – I hope to change
 * that later on. Blame @Itangalo
 */

/**
 * Implements hook_rules_action_info().
 */
function tutor_statistics_rules_action_info() {
  $defaults = array(
    'group' => t('Tutor'),
    'parameter' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('User to log statistics for'),
      ),
      'question_node' => array(
        'type' => 'node',
        'label' => t('Node containing the question'),
      ),
      'question_name' => array(
        'type' => 'text',
        'label' => t('Question name'),
        'description' => t('The name of the question will be stored is a new statistics entity is created, but is not actually used for now.'),
      ),
    ),
    'provides' => array(
      'stats' => array(
        'type' => 'tutor_statistics',
        'bundle' => 'default',
        'label' => t('Statistics entity'),
        'save' => TRUE,
      ),
    ),
  );
  $actions = array(
    'tutor_statistics_plus_one' => $defaults + array(
      'label' => t('Add one to answer streak'),
    ),
    'tutor_statistics_reset' => $defaults + array(
      'label' => t('Reset current answer streak'),
    ),
  );

  return $actions;
}

/**
 * Implements hook_rules_event_info().
 */
function tutor_statistics_rules_event_info() {
  $events = array(
    'tutor_statistics_increase' => array(
      'label' => t('The best streak for a question is increased'),
      'group' => t('Tutor'),
      'variables' => array(
        'question' => array(
          'label' => t('The question id'),
          'type' => 'text',
        ),
        'entity' => array(
          'label' => t('The entity with the question field'),
          'type' => 'entity',
        ),
        'account' => array(
          'label' => t('The user answering the question'),
          'type' => 'user',
        ),
        'max' => array(
          'label' => t('New best streak'),
          'type' => 'integer',
        ),
      ),
    ),
  );
  return $events;
}

/**
 * Action callback for 'tutor_statistics_plus_one'.
 *
 * Loads a relation with question statistics and adds 1 to the field with
 * current streak length. Creates a new relation if necessary.
 *
 * @param $account
 *   The user account to log statistics for.
 * @param $node
 *   The node on which the question exists. Must be a node, as this feature is
 *   configured – no other entities will work with this relation type.
 * @return
 *   The relation entity.
 */
function tutor_statistics_plus_one($account, $node, $question_name) {
  // Take care of the case with anonymous users: Set uid to 0.
  if (!isset($account->uid)) {
    $uid = 0;
  }
  else {
    $uid = $account->uid;
  }

  $entity = tutor_statistics_force_get($uid, $node->nid, $question_name);

  // Create variable aliases for code readability.
  $current = &$entity->tutor_streak_current[LANGUAGE_NONE][0]['value'];
  $max = &$entity->tutor_streak_max[LANGUAGE_NONE][0]['value'];

  // Add one to the current streak. Update longest streak if need be.
  $current++;
  if ($current > $max) {
    $max = $current;
    rules_invoke_event('tutor_statistics_increase', $question_name, $node, $account, $max);
  }

  return array(
    'stats' => $entity,
  );
}

/**
 * Action callback for 'tutor_statistics_reset'.
 *
 * Loads a relation with question statistics and resets the current streak
 * length to zero. Creates a new relation if necessary.
 *
 * @param $account
 *   The user account to log statistics for.
 * @param $node
 *   The node on which the question exists. Must be a node, as this feature is
 *   configured – no other entities will work with this relation type.
 * @return
 *   The relation entity.
 */
function tutor_statistics_reset($account, $node, $question_name) {
  // Take care of the case with anonymous users: Set uid to 0.
  if (!isset($account->uid)) {
    $uid = 0;
  }
  else {
    $uid = $account->uid;
  }

  $entity = tutor_statistics_force_get($uid, $node->nid, $question_name);

  // Reset the current streak to zero.
  $entity->tutor_streak_current[LANGUAGE_NONE][0]['value'] = 0;

  return array(
    'stats' => $entity,
  );
}
