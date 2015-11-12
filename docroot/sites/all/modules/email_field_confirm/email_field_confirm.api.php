<?php

/**
 * @file
 * Hooks provided by Email Field Confirm.
 */

/**
 * Executes when an email address has been confirmed.
 *
 * @param  array  $emails
 *   An array of email addresses that have been confirmed. This is typically a
 *   single value in the array, however it may provide multiple email addresses
 *   if there are any bulk confirmations (yet to be built).
 * @param  int    $uid
 *   The uid of the user that was originally responsible for confirming the
 *   email address.
 * @param  int    $confirmed_by_uid
 *   The uid of the user that actually confirmed the email address. In most
 *   cases this will be the same as $uid, however if a user has permission to
 *   manually confirm any email address, then their uid will be here.
 * @param  int    $status
 *   The confirmed status applied.
 *   - EMAIL_FIELD_CONFIRM_CONFIRMED
 *   - EMAIL_FIELD_CONFIRM_CONFIRMED_BYPASS
 *   - EMAIL_FIELD_CONFIRM_CONFIRMED_ADMIN
 */
function hook_email_field_confirm_emails_confirmed($emails, $uid, $confirmed_by_uid, $status) {
  // See email_field_confirm_email_field_confirm_emails_confirmed() for working
  // example.
}

/**
 * Executes when a pending email address has expired and is being removed.
 *
 * @param  array  $efc_data_array
 *   An array (key: email) of email_field_confirm objects as queried from the
 *   email_field_confirm table.
 *
 * @see email_field_confirm.install for definition of properties.
 */
function hook_email_field_confirm_delete_expired($efc_data_array) {
  // See email_field_confirm_email_field_confirm_delete_expired() for working
  // example.
}
