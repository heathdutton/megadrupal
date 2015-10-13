<?php

/**
 * @file
 * Various non-hook functions for the module are here.
 */

/**
 * Wrapper around Smsglobal\RestApiClient. Heavily inspired by sms_twilio.
 *
 * @param string $command
 *   The command to execute, one of 'send-sms' or 'schedule-sms'.
 * @param array $data
 *   An array of data required by the command. Keys:
 *     - number: int, (required) the number to send the SMS to.
 *     - message: string, (required) the SMS message.
 *     - schedule_time, int, (required if $command = 'schedule-sms') the time to
 *         schedule this SMS to send at.
 *
 * @return array
 *   An array with two keys:
 *     - status: bool, determines the success of this command.
 *     - message: string, a detailed error message (if error).
 *   For more info @see sms_handle_result()
 */
function sms_smsglobal_execute_send($command, array $data) {
  // Try and load the library.
  $library = libraries_load('smsglobal');

  // Error out early if we don't have the library.
  if (empty($library['installed'])) {
    return array(
      'status' => FALSE,
      'message' => t('The SMSGlobal REST client library is not installed, please see README.txt for installation instructions.'),
    );
  }

  // Load gateway config.
  $gateway = sms_gateways('gateway', 'smsglobal');
  $config = $gateway['configuration'];

  // Get sender ID.
  $data['sender_id'] = $config['sms_smsglobal_sender_id'];

  try {
    // Instantiate REST client.
    $rest = _sms_smsglobal_get_rest_client($config);

    // Attempt to instantiate the SMS resource object.
    $sms = _sms_smsglobal_get_sms_object($data, $command);

    // Try to send the SMS.
    $rest->save($sms);

    // See if it was sent successfully.
    // Sometimes an exception isn't thrown if insufficient funds are
    // available. We need to check if we have a SMS ID to catch this.
    if ($sms->getId() === NULL) {
      throw new \Smsglobal\RestApiClient\Exception\AuthorizationException(t('Could not send message, are you sure you have funds?'), 1);
    }
  }
  catch (Exception $e) {
    // Return early with a status array in the form that the SMS Framework
    // is expecting instead of just throwing the exception.
    return array(
      'status' => FALSE,
      'message' => t('Exception occurred: @msg.', array(
        '@msg' => $e->getMessage(),
      )),
    );
  }

  // Everything went well.
  return array(
    'status' => TRUE,
    'message' => t('SMS sent to @number.', array('@number' => $data['number'])),
  );
}

/**
 * Handle an incoming message POSTback.
 *
 * This prints 'OK' if everything went well, 'ERROR' if not.
 * @see http://www.smsglobal.com/http-api/#Incoming_SMS
 */
function sms_smsglobal_incoming() {
  // Instantiate the parser and get it to handle this request.
  $parser = new SmsSmsglobalParser($_REQUEST);
  $parser->parseRequest();

  // Queue this in the sms framework.
  sms_incoming($parser->getNumber(), $parser->getMessage, $parser->getOptions());

  // If we got here, it's all good, send off a success message in the form
  // that SMSGlobal expects.
  print 'OK';
}

/**
 * Instantiate the REST client and return it.
 *
 * @param array $config
 *   The gateway config variables.
 *
 * @return RestApiClient
 *   The instantiate SMSGlobal REST client.
 */
function _sms_smsglobal_get_rest_client($config) {
  $api_key = new \Smsglobal\RestApiClient\ApiKey($config['sms_smsglobal_api_key'], $config['sms_smsglobal_api_secret']);
  return new \Smsglobal\RestApiClient\RestApiClient($api_key);
}

/**
 * Instantiate and return the correct SMS message resource type.
 *
 * @param array $data
 *   An array of data to instantiate the SMS resource object with, keys:
 *     - number: int, (required) the number to send the SMS to.
 *     - sender_id: string, (required) the sender ID the SMS originates from.
 *     - message: string, (required) the SMS message.
 *     - schedule_time, DateTime, (required if $type = 'scheduled') the time to
 *         schedule this SMS to send at (must be in UTC).
 * @param string $command
 *   The type of command that is being run. This controls what kind of SMS
 *   resource is instantiated. Should be one of 'send-sms' (a standard SMS) or
 *   'schedule-sms' (a scheduled SMS). @see sms_smsglobal_execute_send()
 *
 * @return \Smsglobal\RestApiClient\Resource
 *   An instantiated object of type Smsglobal\RestApiClient\Resource\Sms or
 *   Smsglobal\RestApiClient\Resource\SmsScheduled,
 *   with the correct options set.
 */
function _sms_smsglobal_get_sms_object($data, $command = 'send-sms') {
  // Instantiate the correct object type and set type-specific options.
  switch ($command) {
    case 'send-sms':
      $sms = new \Smsglobal\RestApiClient\Resource\Sms();
      break;

    case 'schedule-sms':
      // Make sure a scheduled time is set.
      if (!isset($data['scheduled_time'])) {
        throw new \Smsglobal\RestApiClient\Exception\InvalidDataException(array(
          'A scheduled time was not provided.'
        ), 1);
      }

      $sms = new \Smsglobal\RestApiClient\Resource\SmsScheduled();
      $sms->setDateTime($data['scheduled_time']);
      break;

    default:
      // Invalid command.
      throw new \Smsglobal\RestApiClient\Exception\MethodNotAllowedException('Invalid command.', 1);
  }

  // Standard options valid for all types.
  $sms->setDestination($data['number']);
  $sms->setOrigin($data['sender_id']);
  $sms->setMessage($data['message']);

  return $sms;
}

/**
 * Gets a sanitised phone number.
 *
 * @param int $number
 *   The number to be sanitised.
 * @param int $country_code
 *   The country code that this number belongs to (if any).
 *
 * @return int
 *   A sanitised number.
 */
function _sms_smsglobal_get_clean_number($number, $country_code = NULL) {
  // Make sure the number is an integer (strip punctuation).
  $number = preg_replace("/[^0-9]/", '', $number);
  // Strip any whitespace.
  $number = trim($number);
  // Strip any leading zeroes from the number.
  $number = ltrim($number, '0');

  // Prepend the country code if it's set.
  if ($country_code) {
    // Check if the country code has already been prepended.
    $index = strpos($number, $country_code);
    if ($index === FALSE || $index > 0) {
      $number = $country_code . $number;
    }
  }

  return $number;
}

/**
 * Gets a list of sender ID types supported by this module.
 *
 * @return array
 *   A list of sender ID types in a form that Drupal's Form API select type
 *   understands.
 */
function _sms_smsglobal_get_sender_id_types() {
  return array(
    1 => t('Custom word'),
    2 => t('Dedicated number'),
  );
}
