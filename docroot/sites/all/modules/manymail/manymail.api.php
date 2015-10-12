<?php
/**
 * @file
 * Hooks provided by ManyMail module.
 */

/**
 * Add extra metadata to the mail (not the ManyMailMailer object).
 *
 * Use this function if you need to pass some data from the $form_state to your
 * hook_manymail_mail_alter() implementation.
 *
 * @param array $form_state
 *   The form state for the submitted send mail form.
 *
 * @see manymail_send_mail()
 * @see manymail_send_form_submit()
 * @see hook_manymail_mail_alter()
 */
function hook_manymail_mail_meta($form_state) {
  // If a checkbox named "Force utf-8" is checked, store this info
  // to later adjust the ManyMailMailer object in hook_manymail_mail_alter().
  $meta['force_utf8'] = !empty($form_state['values']['force_utf8']);

  return $meta;
}

/**
 * Alter the ManyMailMailer instance.
 *
 * Modules that implement this hook can alter the settings on the
 * ManyMailMailerinstance created by ManyMail right before it is used to
 * send e-mails with.
 *
 * WARNING: You should not use any send functionality in this hook as it may
 * cause other module implementations to break. Additionally, altering
 * recipients should not be done in this hook either.
 *
 * @param ManyMailMailer $mail
 *   The ManyMailMailer instance created by manymail_send_mail().
 * @param array $meta
 *   The metadata gathered by hook_manymail_mail_meta().
 *
 * @see manymail_send_mail()
 * @see hook_manymail_mail_meta()
 * @see http://code.google.com/a/apache-extras.org/p/phpmailer/
 */
function hook_manymail_mail_alter(&$mail, $meta) {
  $mail->ClearReplyTos();
  $mail->AddReplyTo('helpdesk@example.com', 'Our company helpdesk');
}

/**
 * Registers a recipient retrieval function.
 *
 * If your module implements a mail sending form that builds on the
 * BASE_FORM_ID 'manymail_send_mail', you need to implement this hook
 * to tell ManyMail how to build a recipient list for that form.
 *
 * The callback you register for your form should always return a
 * ManyMailRecipientList instance.
 *
 * @return array
 *   Associative array with the FORM_ID of your form as the key and
 *   an associative array as its value with the following keys:
 *     - callback: the function to call for this form.
 *     - values: (optional) an array of form values that your callback needs.
 *
 * @see manymail_recipients()
 */
function hook_manymail_recipients() {
  // Calls the function mail_friends_get_addresses to retrieve all recipients
  // for a manymail_send_form implementation with FORM_ID mail_friends_form.
  //
  // The form values of the fields friend_categories and obnoxious_people_too
  // are passed to the callback (in that order).
  $recipients['mail_friends_form'] = array(
    'callback' => 'mail_friends_get_addresses',
    'values' => array('friend_categories', 'obnoxious_people_too'),
  );

  return $recipients;
}

/**
 * Alter the ManyMail recipients data.
 *
 * After ManyMail has collected all recipient callback implementations, that
 * data is passed to this function for a final alteration step before it is
 * used to actually retrieve recipients.
 *
 * Note: A lot of functionality can be achieved through ManyMail Views. Only
 * use this function when you still want an existing form to be available, but
 * not return the recipients it usually does.
 *
 * @param array $recipients
 *   The recipient callbacks as registered with hook_manymail_recipients and
 *   as gathered by manymail_recipients().
 *
 * @see manymail_recipients()
 * @see hook_manymail_recipients()
 */
function hook_manymail_recipients_alter(&$recipients) {
  // Add a different callback to the 'Send all users' form.
  $recipients['manymail_send_all_form']['callback'] = 'all_but_admins';
}
