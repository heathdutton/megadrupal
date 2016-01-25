<?php
/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_event_info().
 */
function mandrill_inbound_rules_event_info() {
  return array(
    'mandrill_webhook_inbound' => array(
      'label' => t('Incoming message is received.'),
      'group' => t('Mandrill'),
      'variables' => array(
        'type' => array(
          'type' => 'text',
          'label' => t('Type of received message.')
        ),
        'id' => array(
          'type' => 'integer',
          'label' => t('ID of received message.'),
        ),
        'from_email' => array(
          'type' => 'text',
          'label' => t('Email address who sent email.')
        ),
        'from_name' => array(
          'type' => 'text',
          'label' => t('Name who sent email.')
        ),
        'from_account' => array(
          'type' => 'user',
          'label' => t('User who sent email.')
        ),
        'to_mail' => array(
          'type' => 'text',
          'label' => t('Email address who got email.')
        ),
        'subject' => array(
          'type' => 'text',
          'label' => t('Received subject.')
        ),
        'text' => array(
          'type' => 'text_formatted',
          'label' => t('Received text.')
        ),
        'raw_msg' => array(
          'type' => 'text_formatted',
          'label' => t('Received raw of message.')
        ),
      ),
    ),
  );
}
