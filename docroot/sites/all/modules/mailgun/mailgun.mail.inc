<?php

/**
 * @file
 * Implements Mailgun as a Drupal MailSystemInterface
 */

/**
 * Modify the Drupal mail system to use Mailgun when sending e-mails.
 */
class MailgunMailSystem implements MailSystemInterface {

  /**
   * Concatenate and wrap the e-mail body for either plain-text or HTML e-mails.
   *
   * @param array $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return array
   *   The formatted $message.
   */
  public function format(array $message) {
    // Join the body array into one string.
    if (is_array($message['body'])) {
      $message['body'] = implode("\n\n", $message['body']);
    }

    // If a text format is specified in Mailgun settings, run the message through it.
    $format = variable_get('mailgun_format', '_none');
    if ($format != '_none') {
      $message['body'] = check_markup($message['body'], $format);
    }

    return $message;
  }

  /**
   * Send the e-mail message.
   *
   * @see drupal_mail()
   * @see https://documentation.mailgun.com/api-sending.html#sending
   *
   * @param array $message
   *   A message array, as described in hook_mail_alter(). $message['params'] may contain additional parameters. See mailgun_send().
   *
   * @return bool
   *   TRUE if the mail was successfully accepted or queued, FALSE otherwise.
   */
  public function mail(array $message) {
    // Build the Mailgun message array.
    $mailgun_message = array(
      'from' => $message['from'],
      'to' => $message['to'],
      'subject' => $message['subject'],
      'text' => check_plain($message['body']),
      'html' => $message['body'],
    );

    // Add the CC and BCC fields if not empty.
    if (!empty($message['params']['cc'])) {
      $mailgun_message['cc'] = $message['params']['cc'];
    }
    if (!empty($message['params']['bcc'])) {
      $mailgun_message['bcc'] = $message['params']['bcc'];
    }

    $params = array();

    // Populate default settings.
    if ($variable = variable_get('mailgun_tracking', 'default') != 'default') {
      $params['o:tracking'] = $variable;
    }
    if ($variable = variable_get('mailgun_tracking_clicks', 'default') != 'default') {
      $params['o:tracking-clicks'] = $variable;
    }
    if ($variable = variable_get('mailgun_tracking_opens', 'default') != 'default') {
      $params['o:tracking-opens'] = $variable;
    }

    // For a full list of allowed parameters, see: https://documentation.mailgun.com/api-sending.html#sending.
    $allowed_params = array('o:tag', 'o:campaign', 'o:deliverytime', 'o:dkim', 'o:testmode', 'o:tracking', 'o:tracking-clicks', 'o:tracking-opens');
    foreach ($message['params'] as $key => $value) {
      // Check if it's one of the known parameters.
      $allowed = (in_array($key, $allowed_params)) ? TRUE : FALSE;
      // If more options become available but are not yet supported by the module, uncomment the following line.
      //$allowed = (substr($key, 0, 2) == 'o:') ? TRUE : FALSE;
      if ($allowed) {
        $params[$key] = $value;
      }
      // Check for custom MIME headers or custom JSON data.
      if (substr($key, 0, 2) == 'h:' || substr($key, 0, 2) == 'v:') {
        $params[$key] = $value;
      }
    }

    // Make sure the files provided in the attachments array exist.
    if (!empty($message['params']['attachments'])) {
      $params['attachments'] = array();
      foreach ($message['params']['attachments'] as $attachment) {
        if (file_exists($attachment)) {
          $params['attachments'][] = $attachment;
        }
      }
    }

    $mailgun_message['params'] = $params;

    // Queue the message if the setting is enabled.
    if (variable_get('mailgun_queue', FALSE)) {
      $queue = DrupalQueue::get('mailgun_queue', TRUE);
      $queue->createItem($mailgun_message);
      return TRUE;
    }

    return mailgun_send($mailgun_message);
  }

}
