<?php

/**
 * @file ./drush/go.mail.inc
 *
 * Drush command to send email.
 *
 * @author Cham Pham
 * @author Quyen Bui
 */

/**
 * Command to send mail.
 */
function drush_go_mail() {
  // Get option  from user
  $to = drush_get_option('to', '');
  $from = drush_get_option('from', variable_get('site_mail', ini_get('sendmail_from')));
  $subject = drush_get_option('subject', '');
  $body = drush_get_option('body', '');

  if (empty($subject) || empty($body) || empty($to) || empty($from)) {
    return drush_set_error("--to --subject --body --from option is required.");
  }

  $message = array('subject' => $subject, 'body' => array($body));

  if (!drush_go_mail_send($message, $to, $from)) {
    return drush_set_error('An error occurred while sending the test e-mail.');
  }

  drush_log(dt("e-mail has been sent to @to success.", array('@to' => $to)), "success");
}

/**
 * Perform send mail
 *
 * @param array $message
 * @param string $to E-mail address receive mail
 * @param string $from
 * @return boolean
 */
function drush_go_mail_send($message, $to, $from) {
  $module = 'go';
  $mail_token = 'key';
  $language = language_default();

  $messages = drupal_mail($module, $mail_token, $to, $language, array(), $from, FALSE);

  // Define headers
  $messages['headers'] = array_merge($messages['headers'], array(
    'Content-Type' => 'text/html; charset=UTF-8',
    'From' => $from,
    'Sender' => $from,
    'Return-Path' => $from,
  ));

  $messages['body'] = $message['body'];
  $messages['subject'] = $message['subject'];

  // Retrieve the responsible implementation for this message.
  $system = drupal_mail_system($module, $mail_token);

  // Format the message body.
  $messages = $system->format($messages);

  return (boolean)($system->mail($messages));
}
