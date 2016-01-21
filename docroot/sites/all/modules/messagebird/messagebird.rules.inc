<?php

/**
 * @file
 * Provides Rules integration for MessageBird.
 */

/**
 * Implements hook_rules_action_info().
 */
function messagebird_rules_action_info() {

  return array(
    'messagebird_send' => array(
      'label' => t('Send a SMS message'),
      'parameter' => array(
        'body' => array(
          'type' => 'text',
          'label' => t('Body'),
        ),
        'recipient' => array(
          'type' => 'text',
          'label' => t('Recipient'),
        ),
        'originator' => array(
          'type' => 'text',
          'label' => t('Originator'),
        ),
      ),
      'group' => t('MessageBird'),
    ),
  );
}
