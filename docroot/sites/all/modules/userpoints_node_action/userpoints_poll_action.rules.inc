<?php

/**
 * @file
 * This is the automatically imported rules file for configuring Userpoints Poll Action rules
 */

/**
 * Implementation of hook_rules_event_info() to create a custom event when the user is unable to afford the cost
 */
function userpoints_poll_action_rules_event_info() {
  return array(
    'userpoints_poll_action_payment_success' => array(
      'label' => t('User paid !points and interacted with a poll', userpoints_translation()),
      'module' => 'Userpoints Poll Action',
      'arguments' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User that was charged !points for the interacting with the poll', userpoints_translation()),
        ),
        'node' => array(
          'type' => 'node',
          'label' => t('Poll that required !points for interactions', userpoints_translation()),
        ),
      ),
    ),
    'userpoints_poll_action_payment_failure' => array(
      'label' => t('User didn\'t have enough !points to interact with a poll', userpoints_translation()),
      'module' => 'Userpoints Poll Action',
      'arguments' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User that tried to intract with the poll'),
        ),
        'node' => array(
          'type' => 'node',
          'label' => t('Poll that the user tried to interact with'),
        ),
      ),
    ),
    'userpoints_poll_action_payment_semi-success' => array(
      'label' => t('User didn\'t have enough !points to interact with a poll, but was able to interact anyways', userpoints_translation()),
      'module' => 'Userpoints Poll Action',
      'arguments' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User that managed to intract with the poll'),
        ),
        'node' => array(
          'type' => 'node',
          'label' => t('Poll that the user managed to interact with'),
        ),
      ),
    ),
  );
}
