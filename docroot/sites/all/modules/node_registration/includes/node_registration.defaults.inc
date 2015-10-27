<?php

/**
 * @file
 * Registration defaults.
 *
 * Variables that are allowed in node type settings and node settings.
 */

/**
 * Returns all default default values. Most are for both registration
 * type AND node settings.
 */
function _node_registration_defaults() {
  $defaults = array(
    'status' => NULL,

    // Separate bundle for this node.
    'private_fields' => 0,

    'date_field' => '',
    'max_registration_date_field' => '',
    'min_registration_date_field' => '',
    'capacity_field' => '',

    'start_date' => NULL,
    'allow_registration_until' => '1 day',
    'allow_cancellation_until' => '1 day',

    // E-mail: register -> registree.
    'send_mail_to_registree' => 1,
    'mail_to_registree_bcc' => '',
    'mail_to_registree_subject' => t("You've registered for: [node:title]"),
    'mail_to_registree_body' => t("This is what you've registered with:\n\nE-mail address: [node-registration:email]\n\nTo edit, go to [node-registration:edit-url]. To cancel, go to [node-registration:cancel-url]."),

    // E-mail: register -> registree (waiting list).
    'change_mail_for_waiting_list_registree' => 0,
    'mail_for_waiting_list_registree_subject' => t("You've registered for: [node:title] (BUT YOU'RE ON THE WAITING LIST)"),
    'mail_for_waiting_list_registree_body' => t("This is what you've registered with:\n\nE-mail address: [node-registration:email]\n\nBUT YOU'RE ON THE WAITING LIST.\n\nTo edit, go to [node-registration:edit-url]."),

    // E-mail: register -> admin.
    'send_mail_to_admin' => 0,
    'mail_to_admin_recipients' => variable_get('site_mail', ''),
    'mail_to_admin_subject' => t("Somebody's registered for: [node:title]"),
    'mail_to_admin_body' => t("This is what they entered:\n\nE-mail address: [node-registration:email]\n\nTo view the registration, go to [node-registration:url]."),

    // E-mail: reminder -> registree.
    'send_reminder_mail_to_registrees' => 0,
    'reminder_mail_to_registrees_subject' => t("[node:title] begins in [node:time-left]"),
    'reminder_mail_to_registrees_body' => t("Hey [node-registration:email],\n\nthis [node:content-type:name] starts in [node:time-left].\n\nDon't forget to go! It begins at [node:event-start]."),

    // E-mail: cancel -> registree.
    'send_cancel_mail_to_registree' => 0,
    'cancel_mail_to_registree_bcc' => '',
    'cancel_mail_to_registree_subject' => t("You've cancelled for: [node:title]"),
    'cancel_mail_to_registree_body' => t("This is what you were registered with:\n\nE-mail address: [node-registration:email]\n\nBut now it's gone..."),

    // E-mail: cancel -> admin.
    'send_cancel_mail_to_admin' => 0,
    'cancel_mail_to_admin_recipients' => variable_get('site_mail', ''),
    'cancel_mail_to_admin_subject' => t("Somebody's cancelled for: [node:title]"),
    'cancel_mail_to_admin_body' => t("They had ID [node-registration:registration-id] but it's now gone..."),

    // E-mail: unwaitinglist -> registree.
    'send_unwaitinglist_mail_to_registree' => 0,
    'unwaitinglist_mail_to_registree_subject' => t("You've been moved from the waiting list to the regular list"),
    'unwaitinglist_mail_to_registree_body' => t("Hi [node-registration:email],\n\ndue to a cancellation you've been moved from the waiting list to the regular list.\n\nEvent name: [node:title].\nEvent start: [node:event-start]."),

    // E-mail: sender.
    'sender_name' => '',
    'sender_mail' => '',
    'mail_system' => 0,

    // Show which error messages?
    'show_message_access' => 1,
    'show_message_date' => 1,
    'show_message_capacity' => 1,
    'show_message_registered' => 1,

    'registration_success_message' => t('Thank you, your registration has been received!'),

    'allow_exceeding_capacity' => 0,
    'allow_email_change' => 0,
    'require_email_verification' => 1,

    'capacity' => 0,
    'max_slots_per_registration_authenticated' => 1,
    'max_slots_per_registration_anonymous' => 1,

    'send_reminders_before' => '1 day',

    'toggle_enabled_in_node_form' => 1,
    'no_register_page' => 0,
  );

  return $defaults;
}
