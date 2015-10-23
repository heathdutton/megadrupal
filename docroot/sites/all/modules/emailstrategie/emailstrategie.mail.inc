<?php

/**
 * @file
 *   Mailing and SMS sending platform in SaaS, webservice & API mode.
 */

/**
 * Class EmailStrategieMailSystem
 */
class EmailStrategieMailSystem extends DefaultMailSystem {

  /**
   * @var EmailStrategie_CampaignService
   */
  protected $client;

  /**
   * @var string
   */
  protected static $token = '';

  /**
   * Concatenate and format the e-mail body.
   *
   * @param array $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return string
   *   The formatted $message.
   */
  public function format(array $message) {

    // Join the body array into one string.
    if (is_array($message['body'])) {
      $message['body'] = implode(PHP_EOL . PHP_EOL, $message['body']);
    }

    // Override for SimpleNews.
    if ($message['module'] == 'simplenews') {
      /** @var SimplenewsSourceNode $simplenewsSource */
      $simplenewsSource = $message['params']['simplenews_source'];
      $message['body']  = $simplenewsSource->getBody();

      if ($simplenewsSource->getFormat() == 'html') {
        $message['headers']['Content-Type'] = 'text/html; charset=UTF-8';
      }
      else {
        $message['headers']['Content-Type'] = 'text/plain; charset=UTF-8';
      }
    }

    // Format message according to header content-type.
    if (preg_match('/plain/', $message['headers']['Content-Type'])) {
      $message['body'] = drupal_html_to_text($message['body']);
    }
    elseif (!preg_match('/(charset=["\']?UTF-8["\']?)/mis', $message['body'])) {
      if (preg_match('/<head[^>]*>/mis', $message['body'])) {
        // Add just content-type metatag.
        $message['body'] = preg_replace(
          '/(<head[^>]*>)/mis',
          '$1<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />',
          $message['body']
        );
      }
      elseif (preg_match('/<html[^>]*>/mis', $message['body'])) {
        // Add head node.
        $message['body'] = preg_replace(
          '/(<html[^>]*>)/mis',
          '$1<head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8" /></head>',
          $message['body']
        );
      }
      else {
        // Full wrap.
        $message['body'] = '<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8" /></head><body>' . $message['body'] . '</body></html>';
      }
    }

    return $message;
  }

  /**
   * Send a message composed by drupal_mail().
   *
   * @param array $data
   *     Message array with at least the following elements:
   *     - id: A unique identifier of the e-mail type. Examples: 'contact_user_copy',
   *     'user_password_reset'.
   *     - to: The mail address or addresses where the message will be sent to.
   *     The formatting of this string will be validated with the
   *
   * @link http://php.net/manual/filter.filters.validate.php PHP e-mail validation filter. @endlink
   *       Some examples are:
   *       - user@example.com
   *       - user@example.com, anotheruser@example.com
   *       - User <user@example.com>
   *       - User <user@example.com>, Another User <anotheruser@example.com>
   *       - subject: Subject of the e-mail to be sent. This must not contain any
   *       newline characters, or the mail may not be sent properly.
   *       - body: Message to be sent. Accepts both CRLF and LF line-endings.
   *       E-mail bodies must be wrapped. You can use drupal_wrap_mail() for
   *       smart plain text wrapping.
   *       - headers: Associative array containing all additional mail headers not
   *       defined by one of the other parameters.  PHP's mail() looks for Cc and
   *       Bcc headers and sends the mail to addresses in these headers too.
   *
   * @return bool
   *   TRUE if the mail was successfully accepted for delivery, otherwise FALSE.
   */
  public function mail(array $data) {

    try {
      ini_set('soap.wsdl_cache_enabled', 1);
      ini_set('soap.wsdl_cache_ttl', 86400);

      $this->client = new EmailStrategie_CampaignService();

      $login      = variable_get('emailstrategie_login', '');
      $password   = variable_get('emailstrategie_password', '');
      $token      = variable_get('emailstrategie_token', self::$token);
      $domain     = variable_get('emailstrategie_domain', 'mu0t000.com');
      $campaignID = (isset($data['params']['campaign']) ? $data['params']['campaign'] : 0);

      // Generate a new token if not provided.
      self::$token = ($token ? $token : $this->getSecurityToken($login, $password));
      watchdog('mail', 'EmailStrategie - Security token: @token', array('@token' => self::$token), WATCHDOG_DEBUG);

      if (!$campaignID) {
        $campaignID = $this->createCampaign($data, $domain);
        watchdog(
          'mail',
          'EmailStrategie - Created Campaign ID: @campaignId',
          array('@campaignId' => $campaignID),
          WATCHDOG_DEBUG
        );
      }

      $emailsTo = explode(',', $data['to']);
      foreach ($emailsTo as $emailTo) {
        list(, $emailTo) = $this->splitNameEmail($emailTo);

        // Dynamic values to use with #{key}# which will be replaced with {value}.
        $values = (isset($data['params']['values'][$emailTo]) ? $data['params']['values'][$emailTo] : array());
        $this->sendUnitCampaign($campaignID, $emailTo, $values);

        watchdog(
          'mail',
          'EmailStrategie - mail sent to @mail for campaign #@campaign',
          array('@mail' => $emailTo, '@campaign' => $campaignID),
          WATCHDOG_DEBUG
        );
      }

      return TRUE;
    } catch (Exception $e) {
      watchdog('mail', 'EmailStrategie - @error', array('@error' => $e->getMessage()), WATCHDOG_ERROR);
    }

    return FALSE;
  }

  /**
   * @param string $login
   * @param string $password
   *
   * @return string
   * @throws Exception
   */
  protected function getSecurityToken($login, $password) {
    // Authentication.
    $auth           = new EmailStrategie_GenerateAuthentification();
    $auth->login    = $login;
    $auth->password = $password;
    $result         = $this->client->generateAuthentification($auth);

    if ($result->GenerateAuthentificationResult->status == EmailStrategie_AuthentificationStatus::SUCCESS) {
      return $result->GenerateAuthentificationResult->token;
    }

    throw new Exception('EmailStrategie: authentication failed');
  }

  /**
   * @param array $data
   *
   * @return EmailStrategie_SubjectStandard
   */
  protected function buildSubject(array $data) {

    $campaignSubject              = new EmailStrategie_SubjectStandard();
    $campaignSubject->priority    = EmailStrategie_Priority::NORMAL;
    $campaignSubject->subjectName = $data['subject'];

    // If specified, set priority message.
    if (isset($data['headers']['X-Priority']) && !empty($data['headers']['X-Priority'])) {
      $priority = $data['headers']['X-Priority'];

      if ($priority < 3) {
        $campaignSubject->priority = EmailStrategie_Priority::HIGH;
      }
      else {
        $campaignSubject->priority = EmailStrategie_Priority::LOW;
      }
    }

    return $campaignSubject;
  }

  /**
   * @param array $data
   *
   * @return EmailStrategie_Message
   */
  protected function buildMessage(array $data) {
    $campaignMessage = new EmailStrategie_Message();

    if (preg_match('/plain/', $data['headers']['Content-Type'])) {
      $campaignMessage->contentText = drupal_html_to_text(preg_replace('/\<br(\s*)?\/?\>/i', "\n", $data['body']));
    }
    else {
      $campaignMessage->contentHtml = $data['body'];
    }

    if (isset($data['params']['attachments'])) {
      $campaignMessage->attachments = array();

      foreach ($data['params']['attachments'] as $file) {
        $attachment           = new EmailStrategie_Attachment();
        $attachment->fileName = $file['filename'];
        $attachment->bytes    = $file['filecontent'];

        $campaignMessage->attachments[] = $attachment;
      }
    }

    return $campaignMessage;
  }

  /**
   * @param array  $data
   * @param string $domain
   *
   * @return EmailStrategie_CampaignEmail
   */
  protected function buildCampaign(array $data, $domain) {
    $subject = $this->buildSubject($data);
    $message = $this->buildMessage($data);

    $campaignEmail                    = new EmailStrategie_CampaignEmail();
    $campaignEmail->name              = isset($data['params']['campaign_name']) ? $data['params']['campaign_name'] : 'Transactional email';
    $campaignEmail->isMassRecipients  = FALSE;
    $campaignEmail->encoding          = EmailStrategie_EncodingCampaign::UTF_8;
    $campaignEmail->fileOption        = EmailStrategie_FileOption::NONE;
    $campaignEmail->subject           = $subject;
    $campaignEmail->message           = $message;
    $campaignEmail->replyEmailAddress = $data['headers']['Return-Path'];
    $campaignEmail->domain            = $domain;

    list($campaignEmail->fromName, $campaignEmail->fromEmailAddress) = $this->splitNameEmail($data['from']);

    return $campaignEmail;
  }

  /**
   * @param array  $data
   * @param string $domain
   *
   * @return int
   * @throws Exception
   */
  protected function createCampaign(array $data, $domain) {
    $campaign = $this->buildCampaign($data, $domain);

    $campaignCreate           = new EmailStrategie_CreateCampaign();
    $campaignCreate->campaign = $campaign;
    $campaignCreate->token    = self::$token;

    // Create email campaign.
    $result = $this->client->CreateCampaign($campaignCreate);

    if ($result->CreateCampaignResult->status == EmailStrategie_CampaignResultStatus::SUCCESS) {
      /**
       * Can save the campaignID in your sql table for example in order
       * to retrieve the campaignID to send to another recipient and
       * thus do not create again a new campaign.
       */
      return $result->CreateCampaignResult->campaignID;
    }

    throw new Exception(t(
      'EmailStrategie: create campaign failed: @text',
      array('@text' => $result->CreateCampaignResult->errorContent)
    ));
  }

  /**
   * @param int    $campaignID
   * @param string $to
   * @param array  $fields
   *
   * @return bool
   * @throws Exception
   */
  protected function sendUnitCampaign($campaignID, $to, array $fields = array()) {
    $campaignUnit                     = new EmailStrategie_SendUnitCampaign();
    $campaignUnit->token              = self::$token;
    $campaignUnit->campaignID         = $campaignID;
    $campaignUnit->emailOrPhoneNumber = $to;
    $campaignUnit->customFieldHeaders = array_keys($fields);
    $campaignUnit->customFieldValues  = array_values($fields);

    $result = $this->client->SendUnitCampaign($campaignUnit);

    if ($result->SendUnitCampaignResult == EmailStrategie_SendUnitCampaignStatus::SUCCESS) {
      return TRUE;
    }

    throw new Exception('EmailStrategie: send unit campaign failed');
  }

  /**
   * @param string $text
   *
   * @return array
   */
  protected function splitNameEmail($text) {
    $match = array();

    if (preg_match('/"(.*?)"\s+<\s*(.*?)\s*>/', $text, $match)) {
      return array(trim($match[1]), trim($match[2]));
    }
    elseif (preg_match('/(.*?)<\s*(.*?)\s*>/', $text, $match)) {
      return array(trim($match[1]), trim($match[2]));
    }
    else {
      return array('', $text);
    }
  }
}
