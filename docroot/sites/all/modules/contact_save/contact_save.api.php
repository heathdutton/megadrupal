<?php
/**
 * @file
 * Hooks provided by the Contact Save module.
 */

/**
 * A new contact form message has been saved.
 *
 * This hook is fired once, when a new message has been saved to the database.
 *
 * @param int $message_id
 *   The message ID of the newly saved message.
 */
function hook_contact_save_message_insert($message_id) {
}

/**
 * A contact form message has been read for the first time.
 *
 * This hook is fired only once for a message, the first time the message is
 * read.
 *
 * @param int $message_id
 *   The message ID of the message that was read.
 */
function hook_contact_save_message_first_read($message_id) {
}

/**
 * A contact form message has been read.
 *
 * This hook is fired every time a message is read.
 *
 * @param int $message_id
 *   The message ID of the message that was read.
 */
function hook_contact_save_message_read($message_id) {
}
