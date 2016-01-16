<?php

/**
 * @file E-Mail implementation for the ecard module.
 */

function ecard_mail_send_ecard($ecard, $key = 'ecard_ecard') {
  $module = 'ecard';

  // Specify 'to' and 'from' addresses, and update sent_time for ecard_ecard.
  switch ($key) {
    case 'ecard_ecard':
      $to = ecard_recipient($ecard->mail_to, $ecard->name_to);
      $ecard->sent_time = time();
      $ecard->sent = 1;
      ecard_update($ecard);
      break;
    case 'ecard_copy':
      $to = ecard_recipient($ecard->mail_from, $ecard->name_from);
      // Just to be sure the ecard url can't be used in the email copy template.
      $ecard->ecard_url = $ecard->ecard_copy_url;
      break;
    case 'ecard_notice':
      $to = ecard_recipient($ecard->mail_from, $ecard->name_from);
      break;
  }
  if ($ecard->mail_from) {
    $from = ecard_recipient($ecard->mail_from, $ecard->name_from);
  }

  $language = $GLOBALS['language'];

  $send = TRUE;

  $params['ecard'] = $ecard;

  $message = drupal_mail($module, $key, $to, $language, $params, $from, $send);

  return $message['result'];
}

/**
 * Implements hook_mail
 */
function ecard_mail($key, &$message, $params) {

  switch ($key) {
    case 'ecard_ecard':
      $message['subject'] = token_replace($params['ecard']->mail['letter']['ecard_subject'], $params);
      $message['body'][] = token_replace(theme('ecard_letter', array('message' => $params['ecard']->mail['letter']['ecard_letter'])), $params);
      break;

    case 'ecard_copy':
      $message['subject'] = token_replace($params['ecard']->mail['copy']['ecard_copy_subject'], $params);
      $message['body'][] = token_replace(theme('ecard_copy', array('message' => $params['ecard']->mail['copy']['ecard_copy'])), $params);
      break;

    case 'ecard_notice':
      $message['subject'] = token_replace($params['ecard']->mail['notice']['ecard_notice_subject'], $params);
      $message['body'][] = token_replace(theme('ecard_copy', array('message' => $params['ecard']->mail['notice']['ecard_notice'])), $params);
      break;
  }
}

/**
 * Helper function to create a RFC 2822 conform recipient.
 */
function ecard_recipient($mail, $name = NULL) {
  if ($name) {
    return $name . ' <' . $mail . '>';
  }
  else {
    return $mail;
  }
}
