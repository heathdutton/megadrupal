<?php

/**
 * @file
 *   Class file for MailSystemInterface implementation.
 */

/**
 * Modify the drupal mail system to send HTML emails.
 */
class NewsletterMailSystem implements MailSystemInterface {

  /**
   * Add the source and nnid to newsletter URLs so we can gather clicks.
   */
  public function addNewsletterUrlQuery($message) {
    if (isset($message['params']['newsletter']->nnid)) {
      global $base_url;
      $nnid = $message['params']['newsletter']->nnid;

      $dom = new DomDocument;
      // Convert to html entities to avoid text scrumbling
      // due to encoding especially for non-latin characters.
      $message['body'] = mb_convert_encoding($message['body'], 'HTML-ENTITIES', 'UTF-8');

      @$dom->loadHTML($message['body']);
      $links = $dom->getElementsByTagName('a');

      foreach ($links as $link) {
        $href = $link->getAttribute('href');
        if (strpos($href, $base_url) !== FALSE) {
          $href = drupal_parse_url($href);
          $query = array_merge($href['query'], array('source' => 'newsletter', 'nnid' => $nnid));
          $link->setAttribute('href', url($href['path'], array('query' => $query)));
        }
      }

      // Converts back from hmtl entities
      return decode_entities($dom->saveHTML());
    }
    return $message['body'];
  }

  public function format(array $message) {
    $message['body'] = implode("\n\n", $message['body']);
    // Format the body according to text format
    // or strip all the tags if email format is plain text
    if (!empty($message['body'])) {
      // Add query to all urls so we can gather clicks
      $message['body'] = $this->addNewsletterUrlQuery($message);
      $message['body'] = ($message['format'] == 'html')
        ? check_markup($message['body'], $message['body_format'])
        : strip_tags($message['body']);
      // Remove empty tokens
      $message['body'] = token_replace($message['body'], array(), array('clear' => TRUE));
    }
    if (!empty($message['subject'])) {
      $message['subject'] = strip_tags($message['subject']);
      // Remove empty tokens here as well
      $message['subject'] = token_replace($message['subject'], array(), array('clear' => TRUE));
    }

    return $message;
  }

  public function mail(array $message) {
    if ($message['format'] == 'html') {
      $message['headers']['Content-Type'] = 'text/html; charset=UTF-8; format=flowed';
    }
    // SMTP check
    if (variable_get('newsletter_use_smtp', FALSE) && module_exists('smtp')) {
      $smtp = new SmtpMailSystem();
      $message['body'] = array($message['body']);
      $message = $smtp->format($message);
      $mail_result = $smtp->mail($message);
    }
    else {
      if (isset($message['headers']['Return-Path']) && !ini_get('safe_mode')) {
        $return_path_set = strpos(ini_get('sendmail_path'), ' -f');
        if (!$return_path_set) {
          $message['Return-Path'] = $message['headers']['Return-Path'];
          unset($message['headers']['Return-Path']);
        }
      }
      $mimeheaders = array();
      foreach ($message['headers'] as $name => $value) {
        $mimeheaders[] = $name . ': ' . mime_header_encode($value);
      }
      $line_endings = variable_get('mail_line_endings', MAIL_LINE_ENDINGS);
      $mail_subject = mime_header_encode($message['subject']);
      $mail_body = $message['body'];
      $mail_headers = implode("\n", $mimeheaders);
      if (isset($message['Return-Path']) && !ini_get('safe_mode')) {
        $mail_result = mail($message['to'], $mail_subject, $mail_body, $mail_headers, '-f ' . $message['Return-Path'] );
      }
      else {
        $mail_result = mail($message['to'], $mail_subject, $mail_body, $mail_headers );
      }
    }
    // Integration with maillog
    if (module_exists('maillog') && variable_get('maillog_log', TRUE)) {
      $record = new stdClass;

      $record->header_message_id = isset($message['headers']['Message-ID']) ? $message['headers']['Message-ID'] : NULL;
      $record->subject = $message['subject'];
      $record->body = $message['body'];
      $record->header_from = isset($message['from']) ? $message['from'] : NULL;
      $record->header_to = isset($message['to']) ? $message['to'] : NULL;
      $record->header_reply_to = isset($message['headers']['Reply-To']) ? $message['headers']['Reply-To'] : '';
      $record->header_all = serialize($message['headers']);
      $record->sent_date = REQUEST_TIME;

      drupal_write_record('maillog', $record);
    }
    return $mail_result;
  }
}
