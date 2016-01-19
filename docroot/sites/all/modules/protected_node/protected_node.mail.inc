<?php

/**
 * @file
 * Protected node mail handling.
 */

/**
 * Protected node sends email to specified users.
 *
 * @param object $node
 *   A node object.
 *
 * @todo
 * We should properly handle the language...
 */
function protected_node_send_mail($node) {
  module_load_include('settings.inc', 'protected_node');

  $body = variable_get('protected_node_email_body', protected_node_email_body());
  if (strpos('[node:password]', $body) !== FALSE) {
    if (empty($node->protected_node_clear_passwd)) {
      drupal_set_message(t('The protected node email was not sent because the [node:password] is undefined. You must re-enter the password before saving in order to send the password.'), 'error');
      return;
    }
  }

  $from = variable_get('protected_node_email_from', '');
  if (!$from) {
    // Needs to be null to work properly inside drupal_mail().
    $from = NULL;
  }
  drupal_mail('protected_node', 'password',
    $node->protected_node_emails, 'en',
    array('node' => $node), $from, TRUE);

  drupal_set_message(t('The email about this page was sent to the specified email addresses.'));
}

/**
 * Implements hook_mail().
 *
 * This is the function that builds the actual message body.
 */
function protected_node_mail($key, &$message, $params) {
  $node = $params['node'];

  switch ($key) {
    case 'password':
      module_load_include('settings.inc', 'protected_node');

      $subject = variable_get('protected_node_email_subject', protected_node_email_subject());
      $subject = token_replace($subject, array('node' => $node));
      $message['subject'] = $subject;

      $body = variable_get('protected_node_email_body', protected_node_email_body());
      $body = token_replace($body, array('node' => $node));
      $message['body'][] = $body;
      break;
  }
}
