<?php
/**
 * @file
 * Implements elastic email support on behalf of Drupal core.
 */


/**
 * Modify the drupal mail system to use smtp when sending emails.
 * Include the option to choose between plain text or HTML
 */
class ElasticEmailMailSystem implements MailSystemInterface {

  protected $AllowHtml;

  /**
   * Concatenate and wrap the e-mail body for either plain-text or HTML emails.
   *
   * @param array $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return string
   *   The formatted $message.
   */
  public function format(array $message) {
    $this->AllowHtml = TRUE;
    $message['body'] = implode("\n\n", $message['body']);
    return $message;
  }

  /**
   * Send the e-mail message.
   *
   * @see drupal_mail()
   *
   * @param array $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return bool
   *   TRUE if the mail was successfully accepted, otherwise FALSE.
   */
  public function mail(array $message) {
    $is_queue_enabled = variable_get(ELASTIC_EMAIL_QUEUE_ENABLED, FALSE);

    // If queueing is available and enabled, queue the message.
    if ($is_queue_enabled) {
      $queue = DrupalQueue::get('elastic_email');
      $queue->createItem($message);
      $queue->createQueue();

      watchdog('elastic_email_queue', 'Message added to the Queue - no. of messages: ' . $queue->numberOfItems(), NULL, WATCHDOG_INFO);

      return t('Email message queued for delivery via Elastic Email at cron time.');
    }
    else {
      // Otherwise send the message directly.
      $this->send($message);
      return t('Queuing unavailable. Email sent directly via Elastic Email.');
    }
  }

  /**
   * Performs the actual sending of the message to the Elastic Email service. This
   * code was originally based on the'sendElasticEmail' code snippet from the
   * Elastic Email website.
   *
   * The $elastic_username and $api_key variables are generally not required, as
   * they they are specified in the module configuration. However, you may supply
   * these parameters if you wish to override the configuration values.
   *
   * You must provide either the $subject or $body_text parameter. That is, it is
   * not possible to send an empty email.
   *
   * If this method succeeds, an array will be returned with the following
   * elements:
   * - array['success']['tx_id']: the transaction id returned by Elastic Email
   * - array['success']['to']: semi-colon separated list of recipients
   * - array['success']['msg']: user-friendly message
   *
   * If there's an error, an array will be returned with the following element:
   * - array['error'] : The error message returned by Elastic Email, or an error
   * message from this module if a pre-condition was not met (e.g. missing
   * required parameters).
   *
   * @param string $from
   *   The 'from' (sender) email address.
   * @param string $from_name
   *   (optional) The name of the sender. Defaults to NULL.
   * @param string $to
   *   The semi-colon separated list of recipient email addresses.
   * @param string $subject
   *   The subject line.
   * @param string $body_text
   *   The plain-text body of the message.
   * @param string $body_html
   *   The html-text body of the message.
   * @param string $elastic_username
   *   (optional) The Elastic Email account username (typically the account email
   *   address). If not provided, the value from the module configuration is used.
   * @param string $api_key
   *   (optional) The Elastic Email account API key. If not provided, the value
   *   from the module configuration is used.
   *
   * @return array
   *   Returns an array with either a 'success' or 'error' elements. See main
   *   function description for details. Note that the error message text will
   *   have already been passed through t().
   *
   * @todo Provide support for HTML-based email and attachments?
   */
  public static function elasticEmailSend($from, $from_name = NULL, $to, $subject = '', $body_text = NULL, $body_html = NULL, $elastic_username = NULL, $api_key = NULL) {
    if (!$elastic_username) {
      // If no username provided, get it from the module configuration.
      $elastic_username = variable_get(ELASTIC_EMAIL_USERNAME, NULL);
    }

    if (!$api_key) {
      // If no API Key provided, get it from the module configuration.
      $api_key = variable_get(ELASTIC_EMAIL_API_KEY, NULL);
    }
    $result = array();

    if (empty($elastic_username) || empty($api_key)) {
      $result['error'] = t('Unable to send email to Elastic Email because username or API key not specified.');
    }
    elseif (empty($from) || empty($to) || (empty($subject) && empty($body_text))) {
      $result['error'] = t('Unable to send email because some required email parameters are empty.');
    }

    if (!isset($result['error'])) {
      // It's necessary to urlencode() each of the data values.
      $data = 'username=' . urlencode($elastic_username);
      $data .= '&api_key=' . urlencode($api_key);
      $data .= '&from=' . urlencode($from);
      $data .= '&reply_to=' . urlencode($from);

      if (!empty($from_name)) {
        $data .= '&from_name=' . urlencode($from_name);
        $data .= '&reply_to_name=' . urlencode($from_name);
      }

      $data .= '&to=' . urlencode($to);
      $data .= '&subject=' . urlencode($subject);

      if (!empty($body_text)) {
        $data .= '&body_text=' . urlencode($body_text);
      }
      if (!empty($body_html)) {
        $data .= '&body_html=' . urlencode($body_html);
      }

      if (variable_get('elastic_email_use_default_channel', FALSE)) {
        $data .= '&channel=' . variable_get('elastic_email_default_channel', '');
      }

      $ctx = stream_context_create(array(
        'http' => array('method' => 'post', 'content' => $data)
      ));
      $fp = @fopen(ELASTIC_EMAIL_ENDPOINT, 'rb', FALSE, $ctx);

      // The response should be safe, but call check_plain() for paranoia's sake.
      $response = check_plain(@stream_get_contents($fp));

      if (empty($response)) {
        $result['error'] = t('Error: no response (or empty response) received from Elastic Email service.');
      }
      elseif (!preg_match('/[0-9a-z]{8}-[0-9a-z]{4}-[0-9a-z]{4}-[0-9a-z]{4}-[0-9a-z]{12}/', $response)) {
        $result['error'] = $response;
      }
      else {
        // Message was successfully delivered.
        $result['success']['msg'] = t('Success [@tx_id]; message sent to: @recipients',
          array('@tx_id' => $response, '@recipients' => $to));

        $result['success']['tx_id'] = $response;
        $result['success']['recipients'] = $to;
      }
    }

    return $result;
  }

  /**
   * Sends the emails from the queue when called from the cron.
   *
   * @param array $message
   *   Standard Drupal email message object.
   *
   * @return bool
   *   TRUE if message delivered; FALSE otherwise.
   */
  public static function cronSend(array $message) {
    $mail = new self();
    return $mail->send($message);
  }

  /**
   * Email sending function, called from job queue, or directly.
   *
   * @param array $message
   *   Standard Drupal email message object.
   *
   * @return bool
   *   TRUE if message delivered; FALSE otherwise.
   */
  protected function send($message = array()) {
    // If there's no 'from', then use the default site email.
    if (empty($message['from'])) {
      $from = variable_get('site_mail', NULL);
      if (!empty($from)) {
        $message['from'] = $from;
      }
    }

    // Parse $message['from'] into $from and $from_name if in full RFC format.
    if (preg_match('~^"?([^"]+)"? <\s*(.+)\s*>$~', $message['from'], $matches)) {
      $from_name = $matches[1];
      $from = $matches[2];
    }
    else {
      $from_name = NULL;
      $from = $message['from'];
    }

    // Array to hold the set of email recipients.
    $recipients = array();

    // Parse the various fields that can contain email recipients.
    $this->parseRecipient($recipients, $message['to']);
    if (isset($message['headers']['Cc'])) {
      $this->parseRecipient($recipients, $message['headers']['Cc']);
    }
    if (isset($message['headers']['Bcc'])) {
      $this->parseRecipient($recipients, $message['headers']['Bcc']);
    }

    // Concatenate recipients to a semi-colon separated string.
    if (count($recipients)) {
      $to = implode('; ', $recipients);
    }

    // Check the header content type to see if email is plain text, if not we
    // send as HTML.
    $s_html = (strpos($message['headers']['Content-Type'], 'text/plain') !== FALSE);

    // Attempt to send the message.
    $result = self::elasticEmailSend($from, $from_name, $to, $message['subject'],
      ($s_html ? NULL : $message['body']), ($s_html ? $message['body'] : NULL));

    if (isset($result['error'])) {
      // If there's an error, log it.
      watchdog('elastic_email', 'Failed to send email.  Reason: ' . $result['error'], NULL, WATCHDOG_CRITICAL);
    }
    if (variable_get(ELASTIC_EMAIL_LOG_SUCCESS, FALSE)) {
      // If success, only log if the ELASTIC_EMAIL_LOG_SUCCESS variable is TRUE.
      if (isset($result['success'])) {
        watchdog('elastic_email', 'Email sent successfully: ' . $result['success']['msg'], NULL, WATCHDOG_INFO);
      }
    }

    return isset($result['success']) && $result['success'] ? TRUE : FALSE;
  }

  /**
   * Given a comma-delimited list of email addresses in the $to parameter, parse
   * the addresses and add to the $recipients array.
   *
   * @param array $recipients
   *   A passed-by-reference array holding recipient email addresses.
   * @param string $to
   *   A comma-delimited list of email addresses.
   */
  protected function parseRecipient(&$recipients, $to) {
    if (!$to) {
      return;
    }

    // Trim any whitespace.
    $to = trim($to);
    if (!empty($to)) {
      // Explode on comma.
      $parts = explode(',', $to);
      foreach ($parts as $part) {
        // Again, trim any whitespace.
        $part = trim($part);
        if (!empty($part)) {
          $recipients[] = $part;
        }
      }
    }
  }
}
