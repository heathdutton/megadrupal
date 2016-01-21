<?php

/**
 * @file
 * Functions for integrating the Rules module with Email Field Confirm.
 */

/**
 * Implements hook_rules_event_info().
 * @ingroup rules
 */
function email_field_confirm_rules_event_info() {
  return array(
    'email_field_confirm_emails_confirmation' => array(
      'label' => t('User has confirmed email field address'),
      'group' => t('Email Field Confirmation'),
      'variables' => array(
        'emails' => array(
          'type' => 'list',
          'label' => t('Array of email addresses confirmed.'),
        ),
        'user' => array(
          'type' => 'user',
          'label' => t('User object of the user responsible for confirming.'),
        ),
        'confirmed_by_user' => array(
          'type' => 'user',
          'label' => t('User object of the user who actually confirmed. May be different from the responsible user if they have bypass permissions.'),
        ),
        'status' => array(
          'type' => 'intenger',
          'label' => t('The confirm status.'),
        ),
      ),
      'module' => 'email_field_confirm',
    ),
  );
}
