<?php

/**
 * @file
 * This is the automatically imported rules file for configuring Userpoints Node Action rules
 */

/**
 * Implementation of hook_rules_event_info() to create a custom event when the user is unable to afford the cost
 */
function userpoints_node_action_rules_event_info() {
  return array(
    'userpoints_node_action_payment_success' => array(
      'label' => t('User paid !points and saved content', userpoints_translation()),
      'module' => 'Userpoints Node Action',
      'arguments' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User that was charged !points for the content', userpoints_translation()),
        ),
        'node' => array(
          'type' => 'node',
          'label' => t('Content that required !points for node actions', userpoints_translation()),
        ),
      ),
    ),
    'userpoints_node_action_payment_failure' => array(
      'label' => t('User didn\'t have enough !points to save/validate content', userpoints_translation()),
      'module' => 'Userpoints Node Action',
      'arguments' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User that tried to validate or save the content'),
        ),
        'node' => array(
          'type' => 'node',
          'label' => t('Content that the user tried to validate or save'),
        ),
      ),
    ),
  );
}
