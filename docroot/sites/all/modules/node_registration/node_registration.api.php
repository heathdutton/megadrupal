<?php

/**
 * Implements hook_node_registration_access().
 *
 * Override standard NR access for any operation.
 *
 * @param NodeRegistrationEntityClass $registration
 *   The relevant $registration, which might be new (so no registration_id) in case
 *   of operation 'add'/'create'.
 * @param string $op
 *   The operation this access overrides. One of: delete, view, cancel, edit/update,
 *   add/create. The last two have aliases you must check for.
 * @param stdClass $account
 *   The relevant user object that tries to do this $op to this $registration. Never
 *   empty;
 * @param string $reason
 *   The reason the standard NR access was FALSE. Relevant for all operations. If the
 *   reason is unknown, this will be '?', so it's only empty IF access was granted
 *   by standard NR logic. This argument is useful if your code has expensive logic
 *   to disallow a certain operation: if $reason is not empty AND you're the only
 *   module overriding this access, your expensive code doesn't have to run.
 */
function hook_node_registration_access($registration, $op, $account, $reason) {
  $event_node = $registration->node;
  $settings = $event_node->registration;

  switch ($op) {
    case 'cancel':
      // Normally, a user can't cancel others' registrations.
      if ($reason && $registration->author_uid == $account->uid) {
        // But we did something with author_uid, so now it's okay.
        return TRUE;
      }
      break;

    case 'add':
    case 'create':
      // We don't like registrations on Wednesdays, even for admins.
      if (date('w', REQUEST_TIME) == 3) {
        return FALSE;
      }

      // Normally NR only allows 1 registration per user.
      if ($reason == 'registered') {
        // But this user can register guests.
        if (_some_magic_function($account)) {
          return TRUE;
        }
      }
      break;
  }
}

/**
 * Implements hook_node_registration_node_access().
 *
 * Override standard NR node access for any operation.
 *
 * @param stdClass $node
 *   
 * @param string $op
 *   
 * @param stdClass $account
 *   
 * @param string $reason
 *   
 */
function hook_node_registration_node_access($event_node, $op, $account, $reason) {
  $settings = $event_node->registration;
}

/**
 * Implements hook_node_registration_default_values_alter().
 *
 * A weird form alter while still in node_registration_form().
 *
 * You should instead use the normal form alter: hook_form_FORM_ID_alter()!
 *
 * @param array &$form
 *   
 * @param array &$values
 *   
 * @param array $context
 *   - node: 
 *   - registration: 
 *   - user: 
 */
function hook_node_registration_default_values_alter($form, $values, $context) {
  $node = $context['node'];
  $registration = $context['registration'];
  $user = $context['user'];

  // This will be automatically assigned to $form['field_fullname'][LANGUAGE][0]['value'].
  $values['field_fullname'] = format_username($user);
}

/**
 * Implements hook_node_registration_insert().
 */
function hook_node_registration_insert($registration) {

}

/**
 * Implements hook_node_registration_update().
 */
function hook_node_registration_update($registration) {

}

/**
 * Implements hook_node_registration_presave().
 */
function hook_node_registration_presave($registration) {

}

/**
 * Implements hook_node_registration_delete().
 */
function hook_node_registration_delete($registration) {

}

/**
 * Implements hook_node_registration_email_alter().
 */
function hook_node_registration_email_alter($type, $options, $context) {
  $node = $context['node'];
  $registration = $context['registration'];
}

/**
 * Implements hook_node_registration_email_TYPE_alter().
 *
 * You should instead use the normal mail alter: hook_mail_alter()!
 *
 * TYPE is the alter type passed into node_registration_send_broadcast() via
 * $options['alter']. This (and the previous) alter will only trigger if
 * an alter type was given.
 */
function hook_node_registration_email_TYPE_alter($type, &$options, $context) {
  $node = $context['node'];
  $registration = $context['registration'];
}

/**
 * Implements hook_registration_block_title_alter().
 */
function hook_registration_block_title_alter(&$title, $reason) {

}

/**
 * Implements hook_registration_block_unsubscribe_title_alter().
 */
function hook_registration_block_unsubscribe_title_alter(&$title, $reason) {

}

/**
 * Implements hook_node_registration_table_registrations_alter().
 *
 * DEPRECATED. DON'T USE.
 *
 * This probably won't even trigger, because node_registration_registrations_page()
 * isn't used anymore. This table is now created with an extensible View.
 */
function hook_node_registration_table_registrations_alter(&$table, $context) {

}
