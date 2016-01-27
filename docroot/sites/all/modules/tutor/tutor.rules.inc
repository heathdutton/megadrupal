<?php

/**
 * @file
 * Rules events for the Scripted questions module.
 */

/**
 * Implements hook_rules_event_info().
 */
function tutor_rules_event_info() {
  // Some repeated data.
  $event_variables = array(
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
  );

  // Declare and return the events.
  $events = array(
    'tutor_correct' => array(
      'label' => t('A question is correctly answered'),
      'group' => t('Tutor'),
      'variables' => $event_variables,
    ),
    'tutor_incorrect' => array(
      'label' => t('A question is incorrectly answered'),
      'group' => t('Tutor'),
      'variables' => $event_variables,
    ),
    'tutor_abort' => array(
      'label' => t('A question is abandoned'),
      'group' => t('Tutor'),
      'variables' => $event_variables,
    ),
  );
  return $events;
}
