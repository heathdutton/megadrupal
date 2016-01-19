<?php
/**
 * @file
 * Mail Tracking implementations of MailSystemInterface.
 */

/**
 * Rewrite all URL's to pass through the gateway and dynamically select the
 * MailSystemInterface to use.
 */
class MailTrackingMailSystem implements MailSystemInterface {

  /**
   * @var Cache while rewriting URL's.
   */
  private $rewrite_url_cache;

  /**
   * Concatenate and wrap the e-mail body.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return
   *   The formatted $message.
   */
  public function format(array $message) {
    // Revert to the default mail system, it was overidden by our
    // hook_mail_alter() implementation.
    _mail_tracking_alter_mail_system();

    // Let the original mail system handle the formatting.
    return $this->getMailSystem($message)->format($message);
  }

  /**
   * Send an e-mail message, using Drupal variables and default settings.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return
   *   TRUE if the mail was successfully accepted, otherwise FALSE.
   */
  public function mail(array $message) {
    // Collect all recipients.
    $recipients = explode(',', $message['to']);
    if (!empty($message['headers']['Cc'])) {
      $recipients = array_merge($recipients, explode(',', $message['headers']['Cc']));
      unset($message['headers']['Cc']);
    }
    if (!empty($message['headers']['Bcc'])) {
      $recipients = array_merge($recipients, explode(',', $message['headers']['Bcc']));
      unset($message['headers']['Bcc']);
    }

    // Remove any duplicates.
    $unique = array_unique($recipients);

    // Create an array keyed by e-mail address (without name) and the
    // formatted e-mail address as value.
    $recipients = array();
    foreach ($unique as $to) {
      $recipients[preg_replace('#(^[^<]+<)|(>$)#', '', $to)] = $to;
    }

    // Indicates if the message should be processed. We set this to FALSE if
    // no URL's were rewritten, this way the processing overhead can be skipped.
    $process = TRUE;

    // Copy the message so we can revert the URL rewriting.
    $message_copy = $message;

    // Loop over all recipients and send their rewritten message.
    foreach ($recipients as $mail => $to) {
      if ($process) {
        // Revert the message.
        $message = $message_copy;

        // Set the clean e-mail address as recipient.
        $message['to'] = $mail;

        // Process the body.
        $this->processBody($message);

        // Check if at least one URL was rewritten.
        $process = (isset($message['mail_tracking']) && array_sum($message['mail_tracking']['rewritten']) > 0);

        // Save the token.
        if ($process) {
          $this->saveToken($message);
        }
      }

      // Set the formatted mail address as recipient.
      $message['to'] = $to;

      // Let the original mail system handle the sending.
      $result = $this->getMailSystem($message)->mail($message);
    }

    return (isset($result) ? $result : FALSE);
  }

  /**
   * Process the e-mail body.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   * @param $body
   *   Partial or full body.
   * @param $headers
   *   Headers string.
   *
   * @return
   *   Processed body.
   */
  private function processBody(array &$message, $body = NULL, $headers = NULL) {
    // Get the body.
    if (!isset($body)) {
      $body = &$message['body'];
    }

    // Get the Content-Type header.
    if (!isset($headers)) {
      $headers = $message['headers']['Content-Type'];
    }

    // Content type pattern.
    $pattern = "#(?:\n|^)" . '(?:Content-Type:[ ]*)?((?:multipart/(?:mixed|alternative))|(?:text/plain)|(?:text/html));(?:\s+boundary="([a-f0-9]+)")?#';

    // Check if a match was found.
    if (preg_match($pattern, $headers, $matches)) {
      // Add our custom parameters.
      if (!isset($message['mail_tracking'])) {
        $message['mail_tracking'] = array(
          // Unique user token.
          'token' => md5($message['id'] . '|' . $message['to'] . '|' . REQUEST_TIME),
          // Number of processed body parts (max 2, plaintext and HTML).
          'processed_parts' => 0,
          // Rewritten URL counts.
          'rewritten' => array('read' => 0, 'clicked' => 0, 'unsubscribed' => 0),
        );
      }

      switch ($matches[1]) {
        // Multipart body.
        case 'multipart/mixed':
        case 'multipart/alternative':
          if (!empty($matches[2])) {
            // We need a boundary to continue.
            $boundary = "\n--" . $matches[2] . "\n";

            // Get the multipart parts and reverse the order because the boundary
            // might be the same for all multiparts se we might have already splitted
            // the full message.
            $body_parts = explode($boundary, $body);
            $body_parts = array_reverse($body_parts);

            foreach ($body_parts as $key => $part) {
              // Split the headers and body.
              $part = explode("\n\n", $part, 2);

              if (!empty($part[1])) {
                // Process the current part.
                $part[1] = $this->processBody($message, $part[1], $part[0]);
                $body_parts[$key] = $part[0] . "\n\n" . $part[1];

                // Leave as soon as we can.
                if ($message['mail_tracking']['processed_parts'] == 2) {
                  break;
                }
              }
            }

            // Concatinate the parts again.
            $body = implode($boundary, array_reverse($body_parts));
          }
          break;

        // Plaintext message.
        case 'text/plain':
          $body = $this->processPlaintextBody($body, $message);
          $message['mail_tracking']['processed_parts']++;
          break;

        // HTML message.
        case 'text/html':
          $body = $this->processHtmlBody($body, $message);
          $message['mail_tracking']['processed_parts']++;
          break;
      }
    }

    return $body;
  }

  /**
   * Process a plain text e-mail body.
   *
   * @param $body
   *   E-mail body.
   * @param $message
   *   A message array, as described in hook_mail_alter(), with our additional
   *   variables.
   *
   * @return
   *   Processed body with rewritten URL's.
   */
  private function processPlaintextBody($body, array &$message) {
    // Set the URL rewriting cache.
    $this->rewriteUrlMatch(NULL, $message);

    // Rewrite all URL's.
    return preg_replace_callback($this->getUrlMatchPattern(), array($this, 'rewriteUrlMatch'), $body);
  }

  /**
   * Process a HTML e-mail body.
   *
   * @param $body
   *   E-mail body.
   * @param $message
   *   A message array, as described in hook_mail_alter(), with our additional
   *   variables.
   *
   * @return
   *   Processed body with rewritten URL's.
   */
  private function processHtmlBody($body, array &$message) {
    // Set the URL rewriting cache.
    $this->rewriteUrlMatch(NULL, $message, TRUE);

    // Rewrite all URL's.
    $body = preg_replace_callback($this->getUrlMatchPattern(TRUE), array($this, 'rewriteUrlMatch'), $body);

    // If missing, add a dummy image to detect reading.
    $mail_tracking = &$message['mail_tracking'];

    if ($mail_tracking['rewritten']['read'] == 0) {
      // Build the image URL and tag.
      $img = url(drupal_get_path('module', 'mail_tracking') . '/m.gif', array('absolute' => TRUE, 'alias' => TRUE));
      $img = '<img src="' . $this->getGatewayUrl($mail_tracking['token'], 'read', $img) . '" alt="" />' . "\n";

      // Inject it before the body closing tag.
      $body = str_replace('</body>', $img . '</body>', $body);

      $mail_tracking['rewritten']['read']++;
    }

    // Respect the maximum width as defined in RFC821.
    $body = wordwrap($body, 700);

    return $body;
  }

  /**
   * Rewrite an URL match.
   *
   * @param $matches
   *   Matches array.
   * @param $message
   *   A message array, as described in hook_mail_alter(), with our additional
   *   variables.
   * @param $html
   *   Set to TRUE to indicate that the match is an HTML string.
   *
   * @return
   *   Replacement string.
   */
  private function rewriteUrlMatch($matches, array &$message = NULL, $html = FALSE) {
    if (!isset($matches)) {
      // Cache the variables.
      $this->rewrite_url_cache = array($html, &$message);
      return;
    }

    // Get the cached variables.
    $html = $this->rewrite_url_cache[0];
    $message = &$this->rewrite_url_cache[1];

    // Get the URL we'll proxy.
    $url = ($html ? decode_entities($matches[4]) : $matches[1]);

    // Check if we're handling a plain text message (which cannot contain any
    // images to track that the e-mail is being read) or a HTML link.
    if (!$html || $matches[3] == 'a') {
      // Get the action.
      $action = 'clicked';
      if ($this->isSimplenewsUnsubscribeUrl($url, $message)) {
        $action = 'unsubscribed';
      }

      // Add the Google Analytics parameters.
      if (($html || !$this->isSiteUrl($url, TRUE)) && $this->needsGAParameters($url, $message)) {
        $url = $this->addGAParams($url, $message, $html);
      }
    }
    else {
      // Image tag in HTML message, used to detect if the message is being read.
      $action = 'read';

      if ($message['mail_tracking']['rewritten']['read'] > 0) {
        // We only alter the first image.
        return $matches[0];
      }
    }

    // Create the gateway URL.
    if ($html || !$this->isSiteUrl($url, TRUE)) {
      $url = $this->getGatewayUrl($message['mail_tracking']['token'], $action, $url);
    }

    // Increase the rewritten count.
    $message['mail_tracking']['rewritten'][$action]++;

    // Return the replacement.
    if (!$html) {
      return $url . $matches[2];
    }
    else {
      return $matches[1] . check_plain($url) . $matches[5];
    }
  }

  /**
   * Add the Google Analytics newsletter tracking parameters to the URL.
   *
   * @param $url
   *   URL to add the paremeters to.
   * @param $message
   *   A message array, as described in hook_mail_alter(), with our additional
   *   variables.
   * @param $html
   *   Indicates if the URL was extracted from an HTML string.
   *
   * @return
   *   URL with Google Analytics parameters.
   */
  private function addGAParams($url, array $message, $html = FALSE) {
    // Get the Simplenews node.
    $node = $this->getSimplenewsSource($message)->getNode();

    // Parse the URL and add the extra query parameters.
    $url = drupal_parse_url($url);
    $url['query'] += array(
      'utm_source' => (isset($node->mail_tracking_source) ? $node->mail_tracking_source : ('node-' . $node->nid)),
      'utm_medium' => 'email',
      'utm_content' => ($html ? 'html' : 'txt'),
      'utm_name' => $this->getGASimplenewsCategory($message),
    );

    return url($url['path'], $url);
  }

  /**
   * Rewrite an URL to pass through the gateway.
   *
   * @param $token
   *   Unique user token.
   * @param $action
   *   E-mail action linked to the URL, this can be "read", "clicked" or "unsubscribed".
   * @param $destination
   *   Destination URL, including "http://" or "https://" protocol.
   *
   * @return
   *   Gateway URL.
   */
  private function getGatewayUrl($token, $action, $destination) {
    // Prepare the action, destination and hash.
    $action = substr($action, 0, 1);
    $destination = _mail_tracking_url_encode($destination);
    $hash = _mail_tracking_hash($token, REQUEST_TIME, $action, $destination);

    // Get the path info.
    if (substr_count(rtrim($destination, '/'), '/') > 2) {
      $path_info = pathinfo($destination);
    }

    // Build the path.
    $path = 'mtg/' . $token . '/' . REQUEST_TIME . '/' . $action . '/' . $destination . '/' . $hash;

    // Add the filename.
    if (isset($path_info['extension'])) {
      $path .= '/' . $path_info['basename'];
    }

    // Create and return the gateway URL.
    return url($path, array('absolute' => TRUE, 'alias' => TRUE));
  }

  /**
   * Save the token in the database.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter(), with our additional
   *   variables.
   */
  private function saveToken(array $message) {
    // Make sure the token doesn't exist yet.
    $exists = db_select('mail_tracking_token', 't')
      ->fields('t', array('sid'))
      ->condition('token', $message['mail_tracking']['token'])
      ->execute()
      ->rowCount();

    if (!$exists) {
      // Update the global statistics and get the primary id.
      $sid = $this->createOrUpdateStatistics($message);

      // Get the rewritten URL counts.
      $rewritten = $message['mail_tracking']['rewritten'];

      // Save the token.
      db_insert('mail_tracking_token')
        ->fields(array(
          'token' => $message['mail_tracking']['token'],
          'recipient' => $message['to'],
          'is_clicked' => ($rewritten['clicked'] ? 0 : NULL),
          'is_unsubscribed' => ($rewritten['unsubscribed'] ? 0 : NULL),
          'timestamp' => REQUEST_TIME,
          'sender' => $message['from'],
          'subject' => $message['subject'],
          'sid' => $sid,
        ))
        ->execute();
    }
  }

  /**
   * Create or update the global e-mail message statistics.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter(), with our additional
   *   variables.
   *
   * @return int
   *   Primary id of the statistics record.
   */
  private function createOrUpdateStatistics(array $message) {
    // Get the rewritten URL counts.
    $rewritten = $message['mail_tracking']['rewritten'];

    // Get the period (default mails) and nid (newsletters).
    if ($this->getSimplenewsSource($message)) {
      // Simplenews newsletter.
      $period = NULL;
      $nid = $this->getSimplenewsSource($message)->getNode()->nid;
    }
    else {
      // Other mails.
      $nid = NULL;
      $period = variable_get('mail_tracking_statistics_period', 'week');
      switch ($period) {
        case 'day':
          $date = format_date(REQUEST_TIME, 'custom', 'Y-%-z');
          break;

        case 'week':
          $date = explode('-', format_date(REQUEST_TIME, 'custom', 'Y-n-W'));

          if ($date[1] == 1 && $date[2] >= 52) {
            // Correct the year if it's a week from last year.
            $date = ($date[0] - 1) . '-week-' . $date[2];
          }
          elseif ($date[1] == 12 && $date[2] == 1) {
            // Correct the year if it's a week from next year.
            $date = ($date[0] + 1) . '-week-' . $date[2];
          }
          else {
            // No changes needed.
            $date = $date[0] . '-week-' . $date[2];
          }
          break;

        case 'month':
          $date = format_date(REQUEST_TIME, 'custom', 'Y-%-n');
          break;

        case 'year':
          $date = format_date(REQUEST_TIME, 'custom', 'Y-%');
          break;
      }

      $period = str_replace('%', $period, $date);
    }

    // Try to load the existing statistics record.
    $query = db_select('mail_tracking_statistics', 's')
      ->fields('s', array('sid', 'clicked_count', 'unsubscribed_count'))
      ->condition('module', $message['module'])
      ->condition('message_key', $message['key']);

    if ($period) {
      $query->condition('period', $period)->isNull('nid');
    }
    else {
      $query->condition('nid', $nid)->isNull('period');
    }

    $info = $query->execute()->fetch();

    if ($info) {
      // Statistics already exist.
      $sid = $info->sid;

      // Update them.
      $query = db_update('mail_tracking_statistics')
        ->expression('send_count', 'send_count + 1');

      // Set the clicked and unsubscribed count if needed and not yet set. We use
      // the expression() method because the fields() method seems to override any
      // previous calls.
      if (!$info->clicked_count && $rewritten['clicked']){
        $query->expression('clicked_count', '0');
      }

      if (!$info->unsubscribed_count && $rewritten['unsubscribed']){
        $query->expression('unsubscribed_count', '0');
      }

      $query->condition('sid', $info->sid)->execute();
    }
    else {
      // Insert a new record.
      $sid = db_insert('mail_tracking_statistics')
        ->fields(array(
          'module' => $message['module'],
          'message_key' => $message['key'],
          'period' => $period,
          'nid' => $nid,
          'clicked_count' => ($rewritten['clicked'] ? 0 : NULL),
          'unsubscribed_count' => ($rewritten['unsubscribed'] ? 0 : NULL),
        ))
        ->execute();
    }

    return $sid;
  }

  /**
   * Get the pattern to match URL's.
   *
   * @param $html
   *   Set to TRUE to indicate matching in an HTML string.
   *
   * @return
   *   Pattern that can be used with preg_replace_callback().
   */
  private function getUrlMatchPattern($html = FALSE) {
    // Prepare the basic matching pattern.
    $auth = '[a-zA-Z0-9:%_+*~#?&=.,/;-]+@';
    $domain = '(?:[A-Za-z0-9._+-]+\.)?[A-Za-z]{2,64}\b';
    $ip = '(?:[0-9]{1,3}\.){3}[0-9]{1,3}';
    $trail = '[a-zA-Z0-9:%_+*~#&\[\]=/;?!\.,-]*[a-zA-Z0-9:%_+*~#&\[\]=/;-]';
    $pattern = '(https?://(?:' . $auth . ')?(?:' . $domain . '|' . $ip . ')/?(?:' . $trail . ')?)';

    if (!$html) {
      $pattern = '`' . $pattern . '([\.,?!]*?)`u';
    }
    else {
      $pattern = '`(<(?:(?:(img) (?:[^><]+? )?src)|(?:(a) (?:[^><]+? )?href))=")' . $pattern . '(")`u';
    }

    return $pattern;
  }

  /**
   * Check if a URL point to this website.
   *
   * @param $url
   *   URL to check.
   * @param $home
   *   Set to TRUE so the URL only matches the homepage.
   *
   * @return
   *   TRUE if the URL points to this website.
   */
  private function isSiteUrl($url, $home = FALSE) {
    global $base_url, $base_path;
    static $pattern;

    if (!isset($pattern)) {
      // Create the pattern to match the site's base URL.
      $pattern = str_replace(array('http://', 'https://'), '', $base_url);
      $pattern = '#^https?://' . preg_quote($pattern, '#') . '#';
    }

    // Homepage matching can be done without a regular expression.
    if ($home) {
      return (rtrim($url, '/') == rtrim($base_url . $base_path, '/'));
    }
    else {
      return preg_match($pattern, $url);
    }
  }

  /**
   * Check if an URL is a Simplenews unsubscribe URL.
   *
   * @param $url
   *   URL to check.
   * @param $message
   *   Optional a message array, as described in hook_mail_alter().
   *
   * @return
   *   TRUE if the URL is an unsubscribe URL.
   */
  private function isSimplenewsUnsubscribeUrl($url, array $message = NULL) {
    static $pattern;

    if (!isset($pattern)) {
      // Create the pattern to match the Simplenews unsubscribe URL.
      $pattern = url('newsletter/confirm/remove', array('absolute' => TRUE, 'alias' => TRUE));
      $pattern = str_replace(array('http://', 'https://'), '', $pattern);
      $pattern = '#^https?://' . preg_quote($pattern, '#') . '#';
    }

    // Additional check to see if we're handling a Simplenews newsletter.
    if ($message && !$this->getSimplenewsSource($message)) {
      return FALSE;
    }

    return preg_match($pattern, $url);
  }

  /**
   * Check if an URL needs Google Analytics parameters.
   *
   * @param $url
   *   URL to check.
   * @param $message
   *   Optional a message array, as described in hook_mail_alter().
   *
   * @return
   *   TRUE if the URL need Google Analytics parameters.
   */
  private function needsGAParameters($url, array $message) {
    return (module_exists('googleanalytics') && $this->getSimplenewsSource($message) && $this->isSiteUrl($url));
  }

  /**
   * Get the Simplenews source for a message.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return SimplenewsSourceInterface
   *   Simplenews source or FALSE if not set.
   */
  private function getSimplenewsSource(array $message) {
    if (module_exists('simplenews') && !empty($message['params']['simplenews_source'])) {
      return $message['params']['simplenews_source'];
    }

    return FALSE;
  }

  /**
   * Get the Google Analytics category.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return
   *   Category string.
   */
  private function getGASimplenewsCategory(array $message) {
    static $categories = array();

    $name = $this->getSimplenewsSource($message)->getCategory()->name;

    if (!isset($categories[$name])) {
      $categories[$name] = drupal_html_class($name);
    }

    return $categories[$name];
  }

  /**
   * Get the mail system for a message.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return MailSystemInterface
   *   Mail system implementing MailSystemInterface.
   */
  private function getMailSystem(array $message) {
    // Get the mail system.
    $system = drupal_mail_system($message['module'], $message['key']);

    // Make sure $system isn't an instance of MailTrackingSystem.
    if ($system instanceof MailTrackingMailSystem) {
      // This should be checked/verified immediately.
      watchdog(WATCHDOG_EMERGENCY, '%module %key seems to use the Mail Tracking implementation of MailSystemInterface to send e-mails, this is unwanted behaviour and will be reverted automatically.', array(
        '%module' => $message['module'],
        '%key' => $message['key'],
      ));

      // Revert to the default setting and try again.
      variable_del('mail_system');
      $system = drupal_mail_system($message['module'], $message['key']);
    }

    return $system;
  }
}
