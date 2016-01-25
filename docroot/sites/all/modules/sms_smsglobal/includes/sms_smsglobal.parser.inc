<?php

/**
 * @file
 * Incoming SMS parser for SMSGlobal.
 */

/**
 * SMSGlobal parser class.
 */
class SmsSmsglobalParser extends SmsParserBase {

  /**
   * {@inheritdoc}
   */
  public static function checkAccess($originator, $request) {
    // Make sure we have the data we're expecting.
    // @see http://www.smsglobal.com/http-api/#Incoming_SMS
    if (!isset($request['to']) || !isset($request['from']) || !isset($request['msg'])) {
      return FALSE;
    }

    // Get a list of allowed IP addresses and whether we should check them.
    $gateway = sms_gateways('gateway', 'smsglobal');
    $config = $gateway['configuration'];
    $check_ip = isset($config['sms_smsglobal_ip_restrict']) ? $config['sms_smsglobal_ip_restrict'] : FALSE;

    if ($check_ip) {
      $allowed_ips = isset($config['sms_smsglobal_allowed_ips']) ? $config['sms_smsglobal_allowed_ips'] : SMS_SMSGLOBAL_DEFAULT_IP;
      $allowed_ips = preg_split('/$\R?^/m', $allowed_ips);

      if (!in_array($originator, $allowed_ips)) {
        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * Parse the incoming request.
   */
  public function parseRequest() {
    // Get scrubbed info from the request.
    $this->number = filter_var($this->request['from'], FILTER_SANITIZE_NUMBER_INT);
    $this->message = filter_var($this->request['msg'], FILTER_SANITIZE_STRING);
    $this->options = array(
      'receiver' => filter_var($this->request['to'], FILTER_SANITIZE_NUMBER_INT),
      'date' => filter_var($this->request['date']),
    );
  }

}
