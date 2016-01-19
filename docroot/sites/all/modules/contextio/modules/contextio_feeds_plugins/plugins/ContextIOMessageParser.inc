<?php

/**
 * @file
 * ContextIOMessageParser related classes.
 */

/**
 * Parse Context.IO specific email message data.
 */
class ContextIOMessageParser extends ContextIOParser {

  /**
   * Implements parent::contextIOParse().
   */
  public function contextIOParse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $fetcher = $source->importer()->fetcher;
    // Check fetcher's type and message.
    if (get_class($fetcher) == 'ContextIOMessageFetcher') {
      $messages = $fetcher_result->getMessages();
      if (!empty($messages)) {
        // Init results.
        $results = array();
        // Process messages.
        foreach ($messages as $message) {
          // Result defaults.
          $result = array(
            'id' => $message['message_id'],
            'date' => $message['date'],
            'subject' => $message['subject'],
            'from_email' => empty($message['addresses']['from']['email']) ? '' : $message['addresses']['from']['email'],
            'from_name' => empty($message['addresses']['from']['name']) ? '' : $message['addresses']['from']['name'],
            'to_emails' => '',
            'flags' => '',
            'body_plain' => '',
            'body_html' => '',
          );

          // Make a list from "To" email addresses.
          if (!empty($message['addresses']['to'])) {
            $to_count = count($message['addresses']['to']) - 1;
            foreach ($message['addresses']['to'] as $index => $to) {
              $result['to_emails'] .= $to['email'];
              if ($index < $to_count) {
                $result['to_emails'] .= ', ';
              }
            }
          }

          // Make a list from flags.
          if (!empty($message['flags'])) {
            $result['flags'] = implode(', ', $message['flags']);
          }

          // Check message bodies. For now plain text and HTML type supported.
          if (!empty($message['body'])) {
            foreach ($message['body'] as $body) {
              switch ($body['type']) {
                case 'text/plain':
                  $result['body_plain'] = $body['content'];
                  break;

                case 'text/html':
                  $result['body_html'] = $body['content'];
                  break;
              }
            }
          }

          // Add result.
          $results[] = $result;
        }

        return new FeedsParserResult($results);
      }
    }

    // Unknown fetcher, return with empty result.
    return new FeedsParserResult(array());
  }

  /**
   * Implements parent::contextIOGetMappingSources().
   */
  public function contextIOGetMappingSources() {
    $sources = array();
    // Load the fetcher object.
    $fetcher = feeds_importer($this->id)->fetcher;
    // Check fetcher's type.
    if (get_class($fetcher) == 'ContextIOMessageFetcher') {
      // Default sources.
      $sources += array(
        'id' => array(
          'name' => t('Id'),
          'description' => t('The unique and persistent id assigned by Context.IO to the message.'),
        ),
        'date' => array(
          'name' => t('Date'),
          'description' => t('The Unix timestamp the message sent from the origin.'),
        ),
        'subject' => array(
          'name' => t('Subject'),
          'description' => t('The subject of the email message.'),
        ),
        'from_email' => array(
          'name' => t('From - Email'),
          'description' => t('The email address the message received from.'),
        ),
        'from_name' => array(
          'name' => t('From - Name'),
          'description' => t('The account name the message received from (not always available).'),
        ),
        'to_emails' => array(
          'name' => t('To emails'),
          'description' => t('A comma separated email address list the message sent to (not always available).'),
        ),
      );
      // Optionally add bodies related sources.
      if (!empty($fetcher->config['additional_options']['include_body'])) {
        $sources['body_plain'] = array(
          'name' => t('Body - Plain text'),
          'description' => t('The body of the email message in plain text format.'),
        );
        $sources['body_html'] = array(
          'name' => t('Body - HTML'),
          'description' => t('The body of the email message in HTML format.'),
        );
      }
      // Optionally add a flags related source.
      if (!empty($fetcher->config['additional_options']['include_flags'])) {
        $sources['flags'] = array(
          'name' => t('Flags'),
          'description' => t('A comma separated IMAP flag list the message marked with.'),
        );
      }
    }
    return $sources;
  }

}
