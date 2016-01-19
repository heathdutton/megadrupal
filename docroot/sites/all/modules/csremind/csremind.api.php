<?php
/**
 * @file
 * Hooks provided by the csremind (Contact Save Remind) module.
 */

/**
 * Remind a user that they have saved unread contact form messages.
 *
 * This hook is fired when the user is due to be reminded.
 *
 * @param array $reminder
 *   Details of the reminder from the 'csremind' table including the
 *   user ID.
 *
 * @param int $unread_message_count
 *   The current number of unread messages.
 */
function hook_csremind_remind_user($reminder, $unread_message_count) {
  // Implement how the user is to be reminded.
  //
  // For example, this hook is implemented in the'csremind_message' submodule
  // and implements a reminder as showing the user a Drupal status message on
  // the page.
}