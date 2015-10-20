<?php
/**
 * @file
 * Contains the Cakemail Relay class.
 */

namespace Drupal\cakemail_relay;
use Drupal\cakemail_api\APIException;

/**
 * A Drupal mail system which uses CakeMail's Relay API to send individual mail.
 */
class CakeMailSystem implements \MailSystemInterface {

  private $user_key;

  private $client_id;

  private $template_id;

  public function __construct() {
    $this->user_key = variable_get('cakemail_relay_user_key', NULL);
    $this->client_id = variable_get('cakemail_relay_client_id', NULL);
    $this->template_id = variable_get('cakemail_relay_template_id', NULL);
  }

  /**
   * {@inheritdoc}
   */
  public function format(array $message) {
    $message['body'] = implode("<br/>\n\n", $message['body']);
    $template_id = $this->template_id;
    drupal_alter('cakemail_relay_template_id', $template_id, $message);
    if ($template_id) {
      try {
        $template = new Template(cakemail_api(), $template_id, $this->user_key, $this->client_id);
        $template_variables = array(
          '[id]' => $message['id'],
          '[subject]' => $message['subject'],
          '[body]' => $message['body'],
        );
        $template_tokens = token_scan($template->content);
        foreach ($template_tokens as $type => $tokens) {
          $template_variables += token_generate($type, $tokens);
        }
        drupal_alter('cakemail_relay_template_variables', $template_variables, $message);
        $message['body'] = $template->apply($template_variables);
      }
      catch (APIException $e) {
        cakemail_api_watchdog_exception('cakemail_relay', $e);
      }
    }
    return $message;
  }

  /**
   * {@inheritdoc}
   */
  public function mail(array $message) {

    $args = array(
      'user_key' => $this->user_key,
      'encoding' => 'utf-8',
      'subject' => $message['subject'],
      'html_message' => $message['body'],
      'text_message' => $this->textMessage($message['body']),
      'track_opening' => !empty($message['cakemail_track_opening']),
      'track_clicks_in_html' => !empty($message['cakemail_track_clicks_in_html']),
      'track_clicks_in_text' => !empty($message['cakemail_track_clicks_in_text']),
      'data' => $message['headers'],
    );
    if ($this->client_id) {
      $args['client_id'] = $this->client_id;
    }
    list($sender) = $this->rfc822ParseAddresses($message['from']);
    $args['sender_email'] = $sender['address'];
    $args['sender_name'] = ($sender['address'] == $sender['display']) ? $this->senderName($sender['address']) : $sender['display'];
    $failed = array();
    foreach ($this->rfc822ParseAddresses($message['to']) as $recipient) {
      try {
        cakemail_api()->Relay->Send($args + array('email' => $recipient['address']));
      }
      catch (APIException $e) {
        $failed[] = $recipient;
        cakemail_api_watchdog_exception('cakemail_relay', $e);
        watchdog('cakemail_relay', 'Unable to send mail to @name <@address>', array(
          '@name' => $recipient['name'],
          '@address' => $recipient['address'],
        ), WATCHDOG_ERROR);
      }
    }
    if (empty($failed)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Parses a @link http://www.faqs.org/rfcs/rfc822 RFC 822 @endlink compliant
   * recipient list.
   *
   * @param $addresses
   *   A string containing addresses, like in: Wez Furlong <wez@example.com>,
   *   doe@example.com
   *
   * @return array
   *   An array of associative arrays with the following keys for each
   *   recipient:
   *   - display: The recipient name, for display purpose. If this part is not
   *    set for a recipient, this key will hold the same value as address.
   *   - address: The email address
   *
   * @throws \Exception
   *   If no email address parser functions is available.
   */
  protected function rfc822ParseAddresses($addresses) {
    if (function_exists('mailparse_rfc822_parse_addresses')) {
      return mailparse_rfc822_parse_addresses($addresses );
    }
    else if (function_exists('imap_rfc822_parse_adrlist')) {
      $parsed_addresses = array();
      foreach (imap_rfc822_parse_adrlist($addresses, 'localhost') as $key => $address) {
        $parsed_addresses[$key] = array(
          'address' => "{$address->mailbox}@{$address->host}",
        );
        $parsed_addresses[$key]['display'] = empty($address->personal) ? $parsed_addresses[$key]['address'] : $address->personal;
      }
      return $parsed_addresses;
    }
    else {
      throw new \Exception('The PECL Mailparse library or the IMAP extension must be installed in order to parse email addresses.');
    }
  }

  /**
   * Returns the sender name for a given address.
   *
   * Drupal's mail API only provide a sender email address. The function tries
   * to map email address to name in order to provide a better user experience
   * for recipients.
   *
   * @param string $address
   *   An email address.
   *
   * @return string
   *   An unsanitized string with the sender name matching the email address, or
   *   the email address itself if no matching name is found.
   */
  protected function senderName($address) {
    if ($account = user_load_by_mail($address)) {
      // The sender is a  Drupal user, use his/her username.
      $name = format_username($account);
    }
    else {
      $name = $address;
    }
    // Allow third-party module to provide sender name.
    drupal_alter('cakemail_relay_sender_name', $name, $address);
    return $name;
  }

  /**
   * Create the text version of an HTML message, ready for usage as mail body.
   *
   * @param string $body
   *   An HTML message.
   *
   * @return string
   *   A string with text version of the HTML mail.
   */
  protected function textMessage($body) {
    // Convert any HTML to plain-text.
    $body = drupal_html_to_text($body);
    // Wrap the mail body for sending.
    $body = drupal_wrap_mail($body);
    return $body;
  }

}