<?php

/**
 * @file
 * Actions, triggers and rules -- callbacks, definitions and hooks.
 */

/**
 * Implements hook_action_info().
 */
function node_registration_action_info() {
  return array(
    // Formless actions.
    'node_registration_verify_action' => array(
      'type' => 'node_registration',
      'label' => t('Verify node registration'),
      'configurable' => FALSE,
      'behavior' => array('changes_property'),
      'triggers' => array('node_registration_insert', 'node_registration_update'),
    ),
    'node_registration_unverify_action' => array(
      'type' => 'node_registration',
      'label' => t('Unverify node registration'),
      'configurable' => FALSE,
      'behavior' => array('changes_property'),
      'triggers' => array('node_registration_insert', 'node_registration_update'),
    ),
    'node_registration_attend_action' => array(
      'type' => 'node_registration',
      'label' => t('Attend node registration'),
      'configurable' => FALSE,
      'behavior' => array('changes_property'),
      'triggers' => array('node_registration_insert', 'node_registration_update'),
    ),
    'node_registration_unattend_action' => array(
      'type' => 'node_registration',
      'label' => t('Unattend node registration'),
      'configurable' => FALSE,
      'behavior' => array('changes_property'),
      'triggers' => array('node_registration_insert', 'node_registration_update'),
    ),
    'node_registration_cancel_action' => array(
      'type' => 'node_registration',
      'label' => t('Cancel node registration'),
      'configurable' => FALSE,
      'behavior' => array('changes_property'),
      'triggers' => array(),
    ),
    'node_registration_uncancel_action' => array(
      'type' => 'node_registration',
      'label' => t('Uncancel node registration'),
      'configurable' => FALSE,
      'behavior' => array('changes_property'),
      'triggers' => array(),
    ),
    'node_registration_wait_action' => array(
      'type' => 'node_registration',
      'label' => t('Waitinglist node registration'),
      'configurable' => FALSE,
      'behavior' => array('changes_property'),
      'triggers' => array(),
    ),
    'node_registration_unwait_action' => array(
      'type' => 'node_registration',
      'label' => t('Unwaitinglist node registration'),
      'configurable' => FALSE,
      'behavior' => array('changes_property'),
      'triggers' => array(),
    ),

    // Configurable actions.
    'node_registration_send_mail_action' => array(
      'type' => 'entity',
      'label' => t('Send selected users an e-mail'),
      'configurable' => FALSE,
      'vbo_configurable' => TRUE,
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function node_registration_rules_condition_info() {
  $defaults = array(
    'parameter' => array(
      'node_registration' => array('type' => 'node_registration', 'label' => t('Node registration')),
    ),
    'group' => t('Node registration'),
    'access callback' => 'node_registration_rules_integration_access',
  );
  $items['node_registration_is_of_type'] = $defaults + array(
    'label' => t('Node registration is of type'),
    'help' => t('Evaluates to TRUE if the given registration is of one of the selected types.'),
    'base' => 'node_registration_rules_condition_is_of_type',
  );
  $items['node_registration_is_of_type']['parameter']['type'] = array(
    'type' => 'list<text>',
    'label' => t('Node registration types'),
    'options list' => 'node_type_get_names',
    'description' => t('The registration type(s) to check for.'),
    'restriction' => 'input',
  );
  $items['node_registration_is_verified'] = $defaults + array(
    'label' => t('Node registration is verified'),
    'base' => 'node_registration_rules_condition_is_verified',
  );
  $items['node_registration_has_attended'] = $defaults + array(
    'label' => t('Node registration has attended'),
    'base' => 'node_registration_rules_condition_has_attended',
  );
  $items['node_registration_is_cancelled'] = $defaults + array(
    'label' => t('Node registration is cancelled'),
    'base' => 'node_registration_rules_condition_is_cancelled',
  );
  return $items;
}

/**
 * Action handler: send e-mail.
 */
function node_registration_send_mail_action($registration, $context) {
  $token_data = array(
    'node' => node_load($registration->nid),
    'node-registration' => $registration,
    'from' => array(
      'from_name' => $context['from_name'],
      'from_email' => $context['from_email'],
    ),
  );

  $options = array();
  if (!empty($context['bcc'])) {
    $options['bcc'] = $context['bcc'];
  }

  $recipient = $registration->email;
  if (!empty($context['recipientfield'])) {
    $recipient = token_replace($context['recipientfield'], $token_data, array('clear' => TRUE, 'sanitize' => FALSE));
    $recipient = implode(', ', array_map('trim', array_filter(explode(',', $recipient))));
  }
  $options['alter'] = 'node_registration_send_mail_action';

  return _node_registration_send_email($recipient, $context['subject'], $context['message'], $token_data, $options);
}

/**
 * Action handler: verified ON.
 */
function node_registration_verify_action($registration) {
  $registration->verified = 1;
}

/**
 * Action handler: verified OFF.
 */
function node_registration_unverify_action($registration) {
  $registration->verified = 0;
}

/**
 * Action handler: attended ON.
 */
function node_registration_attend_action($registration) {
  $registration->attended = 1;
}

/**
 * Action handler: attended OFF.
 */
function node_registration_unattend_action($registration) {
  $registration->attended = 0;
}

/**
 * Action handler: cancel registration.
 */
function node_registration_cancel_action($registration, $force = FALSE) {
  $node = node_load($registration->nid);
  $settings = $node->registration;

  // Cancel.
  $registration->cancelled = REQUEST_TIME;
  if (TRUE === $force) {
    $registration->save();
  }

  $token_data = array(
    'node' => $node,
    'node-registration' => $registration,
  );

  // Send e-mail to registree.
  if ($settings->send_cancel_mail_to_registree) {
    $subject = $settings->cancel_mail_to_registree_subject;
    $message = $settings->cancel_mail_to_registree_body;
    $options = array(
      'bcc' => token_replace($settings->cancel_mail_to_registree_bcc, $token_data, array('clear' => TRUE)),
    );
    node_registration_send_broadcast($node, $subject, $message, array($registration), $options);
  }

  // Send e-mail to admin.
  if ($settings->send_cancel_mail_to_admin) {
    $subject = token_replace($settings->cancel_mail_to_admin_subject, $token_data);
    $message = token_replace($settings->cancel_mail_to_admin_body, $token_data);
    $recipients = token_replace($settings->cancel_mail_to_admin_recipients, $token_data);
    _node_registration_send_email($recipients, $subject, $message, $token_data);
  }

  // Log cancellation.
  watchdog('node_registration', 'Registration %id cancelled.', array('%id' => $registration->registration_id));
}

/**
 * Action handler: cancelled OFF.
 */
function node_registration_uncancel_action($registration) {
  $node = node_load($registration->nid);

  // Uncancel.
  $registration->cancelled = 0;
}

/**
 * Action handler: waitinglist ON.
 */
function node_registration_wait_action($registration) {
  $registration->waitinglist = 1;
}

/**
 * Action handler: waitinglist OFF.
 */
function node_registration_unwait_action($registration) {
  $registration->waitinglist = 0;
}

/**
 * Rules condition: registration node type.
 */
function node_registration_rules_condition_is_of_type($registration, $types) {
  return in_array($registration->node_type, $types);
}

/**
 * Rules condition: verified.
 */
function node_registration_rules_condition_is_verified($registration) {
  return (bool) $registration->verified;
}

/**
 * Rules condition: attended.
 */
function node_registration_rules_condition_has_attended($registration) {
  return (bool) $registration->attended;
}

/**
 * Rules condition: cancelled.
 */
function node_registration_rules_condition_is_cancelled($registration) {
  return (bool) $registration->cancelled;
}

/**
 * Node registration integration access callback.
 */
function node_registration_rules_integration_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    return entity_access('view', 'node_registration');
  }
}
