<?php
/**
 * @file
 * The code processing mail in the mail_headers module.
 */

/**
 * Modify the drupal mail system to allow HTML.
 */
class MailHeadersSystem extends DefaultMailSystem {

  /**
   * Concatenate and wrap the e-mail body for either
   * plain-text or HTML emails.
   *
   * @param (array) $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return (array)
   *   The formatted $message.
   *
   * @see hook_mail_alter()
   */
  public function format(array $message) {
    $message['body'] = implode("\n\n", $message['body']);

    if (!variable_get('mail_headers_allow_html', FALSE)) {
      // Convert any HTML to plain-text.
      $message['body'] = drupal_html_to_text($message['body']);
      // Wrap the mail body for sending.
      $message['body'] = drupal_wrap_mail($message['body']);
    }

    return $message;
  }
}
