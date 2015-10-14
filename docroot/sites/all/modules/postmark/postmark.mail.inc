<?php
/**
 * @file
 * Implements postmark support on behalf of Drupal core.
 */
define('POSTMARKAPP_API_KEY', variable_get('postmark_api_key', ''));

if (($library = libraries_load('postmark-php')) && !empty($library['loaded'])) {
  $debug_mode = variable_get('postmark_debug_mode', 0);
  if ($debug_mode) {
    watchdog('postmark', '<pre>Library loaded successfully: '.print_r($library,1).'</pre>');
  }
} else {
  $debug_mode = variable_get('postmark_debug_mode', 0);
  if ($debug_mode) {
    watchdog('postmark', '<pre>Unable to load library: '.print_r($library,1).'</pre>', NULL, WATCHDOG_ERROR);
  }
  if ((arg(0) == 'admin') && (arg(1) != 'config') && (arg(1) != 'reports')) {
    drupal_set_message(t('The Postmark module is enabled but the Postmark PHP library has not been installed. See module README file for instructions.'), 'error');
  }
}

/**
* Modify the drupal mail system to use smtp when sending emails.
* Include the option to choose between plain text or HTML
*/
class PostmarkMailSystem implements MailSystemInterface {

  protected $AllowHtml;
  /**
   * Concatenate and wrap the e-mail body for either
   * plain-text or HTML emails.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return
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
   * @param $message
   *   A message array, as described in hook_mail_alter().
   * @return
   *   TRUE if the mail was successfully accepted, otherwise FALSE.
   */
  public function mail(array $message) {
		$postmark_mail = new Mail_Postmark();

		// Parse 'From' e-mail address.
		$address = postmark_parse_address($message['from']);
		$sender = $address[0]['mail'];
		$name = $address[0]['name'];
		$from = variable_get('site_mail', ini_get('sendmail_from'));
		$postmark_mail->from($from);
		$postmark_mail->subject($message['subject']);

		// Handle reply to and from name
		if ($sender != $from) {
			// replacing sender, move to reply-to, replacing original reply-to
			$postmark_mail->replyTo($sender, ($name == '') ? NULL : $name);
			unset($message['headers']['Reply-To']);
		} else {
			// using original sender, set from name
			if ($name != '') {
				$postmark_mail->fromName($name);
			}
		}

		// Prepare sender name for inclusion in email footer
		$fullsender = $sender;
		if ($name != '') {
			$fullsender = $name . ' [' . $sender . ']';
		}

		// Handle custom headers
		foreach ($message['headers'] as $name => $value) {
			if ($name == 'Reply-To') {
				$reply_to = postmark_parse_address($message['headers']['Reply-To']);
				$postmark_mail->replyTo($reply_to[0]['mail'], $reply_to[0]['name']);
			}
			// @todo support other custom headers
		}

		// Set up our debugging mode vars
		$debug_mode = variable_get('postmark_debug_mode', 0);
		$debug_email = variable_get('postmark_debug_email', '');
		if (!$debug_mode) {
			// Set recipients.
			foreach (postmark_parse_address($message['to']) as $id => $address) {
				// For the first email set as initial 'To'
				if ($id == 0) {
					$postmark_mail->to($address['mail'], $address['name']);
				}
				// Otherwise we must add to the array
				else {
					$postmark_mail->addTo($address['mail'], $address['name']);
				}
			}
		}
		else {
			// Reroute to debug e-mail address.
			if ($debug_email != '') {
				drupal_set_message(t('Debugging email used @email', array('@email' => $debug_email)), 'warning');
				$postmark_mail->to($debug_email);
			}
		}

		// Check the header content type to see if email is plain text
		// if not we send as HTML
		if (strpos($message['headers']['Content-Type'], 'text/plain') !== FALSE) {
			if ($sender != $from) {
				$message['body'] .= "\r\n\r\n";
				$message['body'] .= t('Message sent by').":\r\n";
				$message['body'] .= $fullsender;
			}

			$postmark_mail->messagePlain($message['body']);
		}
		else {
			if ($sender != $from) {
				$message['body'] .= "<p>".t('Message sent by').":<br />".$fullsender."</p>";
			}
			$postmark_mail->messageHtml($message['body']);
		}

		// If debug mode is on, output the message
		if ($debug_mode) {
			drupal_set_message('Message array: <pre>'. print_r($message, TRUE) .'</pre>', 'warning');    
		}

		// If the debug option of not sending a credit, i.e.
		// for testing, is switched on just return TRUE,
		// otherwise send the email via Postmark
		if (variable_get('postmark_debug_no_send', 0)) {
			drupal_set_message('Email successfully tested, no email has been sent (no credits used)', 'warning');
			return TRUE;
		}
		else {
			try {
				if (!($result = $postmark_mail->send())) {
					watchdog('postmark', "Mail sending error: $postmark_mail->ErrorInfo", NULL, WATCHDOG_ERROR);
				}
			}
			catch (Exception $e) {
				watchdog('postmark', 'Exception message: '. $e->getMessage(), NULL, WATCHDOG_ERROR);
				drupal_set_message('Mail sending error: '. $e->getMessage(), 'error');
				// If debugging is on let's put the whole exception into watchdog
				// to enable closer inspection
				if ($debug_mode) {
					watchdog('postmark', 'Exception caught: <pre>'. print_r($e, TRUE) .'</pre>', NULL, WATCHDOG_ERROR);
				}
			}
		}
  return $result;
  }
}
