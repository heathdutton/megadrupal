<?php
/**
 * @file
 * This file contains API documentation for the unsubscribe module. Note that
 * all of this code is merely for example purposes, it is never executed when
 * using the unsubscribe module.
 */

/**
 * Alter the list of unsubscribe module exemptions.
 *
 * @param array $exemptions
 *   A single-dimensional array comprised of module machine-names.
 *
 * @ingroup hooks
 */
function hook_unsubscribe_exemptions_alter(&$exemptions) {
  // Add new exception for custom_module.
  $exemptions[] = 'custom_module';

  // Remove system module exemption.
  unset($exemptions['system']);
}

/**
 * Allows modules to override unsubscribe's blocking function.
 *
 * You might ask "Why would I need this? I can just implement hook_mail_alter()
 * to modify $message." Well, this hook is always called AFTER exemptions have
 * been checked, and unsubscribe has done its work. This means that you don't
 * need to worry about module weights. Maybe it should be ditched?
 *
 * @param array $message
 *   An associative array containing the message to be sent.
 *
 * @ingroup hooks
 */
function hook_unsubscribe_override(&$message) {
  // Exempt any mail sent by user 0.
  if (user_load_by_mail($message['from'])->uid == 0) {
    $message['send'] = TRUE;
  }
}

/**
 * React to a user being added or removed to the unsubscribe list.
 *
 * @param $account
 *  The account object for the user that was added or removed.
 *
 * @param $action
 *  A string that will have either the value 'add' or 'remove.'
 */
function hook_unsubscribe($account, $action) {
  if ($action == 'add') {
    drupal_set_message(t('@mail has been added to the unsubscribe list.', array('@mail' => $account->mail)));
  }
  else {
    drupal_set_message(t('@mail has been remove from the unsubscribe list.', array('@mail' => $account->mail)));
  }
}