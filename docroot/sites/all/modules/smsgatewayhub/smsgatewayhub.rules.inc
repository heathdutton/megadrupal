<?php

/**
 * @file
 * Rules integration for SMS Gateway Hub. Provides rules action to send SMS.
 */

/**
 * Implements hook_rules_action_info().
 */
function smsgatewayhub_rules_action_info() {
  return array(
    'smsgatewayhub_action_send_sms' => array(
      'label' => t('Send SMS'),
      'group' => t('SMS Gateway Hub'),
      'parameter' => array(
        'number' => array(
          'type' => 'text',
          'label' => t('Numbers'),
          'description' => t('Numbers where you want to send SMS. Multiple numbers should be comma separated.'),
        ),
        'message' => array(
          'type' => 'text',
          'label' => t('Message'),
          'description' => t('Message you want to send in SMS. If this is a transactional SMS, please check whether you are authorized to send transactional SMS and also check available templates.'),
        ),
        'flash' => array(
          'type' => 'boolean',
          'label' => t('Flash message'),
          'description' => t('If checked the sms will appear as a flash message.'),
          'optional' => TRUE,
        ),
        'transactional' => array(
          'type' => 'boolean',
          'label' => t('Transactional SMS'),
          'description' => t('If checked SMS will be sent as transactional SMS, otherwise sent as promotional SMS.'),
          'optional' => TRUE,
        ),
        'scheduled_time' => array(
          'type' => 'date',
          'label' => t('Scheduled time'),
          'description' => t('Enter date time for scheduled SMS. Do not enter anything if you do not need this option.'),
          'optional' => TRUE,
          'default value' => '',
        ),
      ),
    ),
  );
}

/**
 * Rules callback to send SMS.
 *
 * @param string $number
 *   Single number or comma separated multiple numbers.
 * @param string $message
 *   Message to send in SMS.
 * @param bool $flash
 *   Whether SMS to be sent as flash message.
 * @param bool $transactional
 *   Indicates whether this is a transactional SMS.
 */
function smsgatewayhub_action_send_sms($number, $message, $flash, $transactional, $scheduled_time) {
  smsgatewayhub_send_sms($number, $message, $flash, $transactional, $scheduled_time);
}

/**
 * Implements hook_rules_event_info().
 */
function smsgatewayhub_rules_event_info() {
  return array(
    'smsgatewayhub_before_send_sms' => array(
      'label' => t('Before sending SMS'),
      'group' => t('SMS Gateway Hub'),
    ),
    'smsgatewayhub_after_send_sms' => array(
      'label' => t('After sending SMS'),
      'group' => t('SMS Gateway Hub'),
    ),
  );
}
