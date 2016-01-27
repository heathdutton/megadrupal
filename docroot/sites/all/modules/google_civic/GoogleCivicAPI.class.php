<?php

/**
 * @file Google Civic API class
 */

define('GOOGLE_CIVIC_API', "https://www.googleapis.com/civicinfo/v1");

/**
 * Consumes the Google Civic REST API.
 */
class GoogleCivicAPI {
  private $_api_key;

  /**
   * Constructor...
   *
   * @param string $api_key
   *   The Google API key under which requests via this API will be issued.
   */
  public function __construct($api_key) {
    $this->_api_key = $api_key;
  }

  /**
   * Attaches $this API's Google API key to a URL.
   *
   * @param string $url
   *   A URL to a Google Civic REST endpoint.
   */
  private function attach_key(&$url) {
    if ($this->_api_key) {
      $url .= sprintf("?key=%s", $this->_api_key);
    }
  }

  /**
   * Issues a request to the Google Civic REST API.
   *
   * @param string $url
   *   A URL (the Google Civic JSON endpoint to request).
   * @param Array $payload
   *   An ASSOCIATIVE array (of data to attach to the request).
   *
   * @return Object
   *   The response (decoded as an object) from Google Civic's API.
   */
  private function request($url, $payload) {
    $this->attach_key($url);

    $jsonPayload = '';
    if ($payload) {
      $jsonPayload = json_encode($payload);
    }

    $options = array(
		     'method' => 'POST',
		     'data' => $jsonPayload,
		     'headers' => array('Content-Type' =>  'application/json')
		     );

    $resp = drupal_http_request($url, $options);
    $answer = json_decode($resp->data);

    if (!empty($answer->error) && $answer->error && is_object($answer->error)) {
      $params = array(
        '@code' => $answer->error->code,
        '@message' => $answer->error->message,
      );
      watchdog('google_civic',
        'Google Civic API error: code @code, message @message', $params,
        WATCHDOG_ERROR);

      if (is_array($answer->error->errors)) {
        foreach ($answer->error->errors as $error) {
          $params = array(
            '@domain' => $error->domain,
            '@reason' => $error->reason,
            '@message' => $error->message,
          );
          watchdog('google_civic',
            'Google Civic API error detail: ' .
              'domain @domain, reason @reason, message @message',
            $params, WATCHDOG_ERROR);
        }
      }
    }

    return $answer;
  }

  /**
   * Requests voterinfo for an address for a given year.
   *
   * @param int $election_id
   *   An ID (of the election to request voter information).
   * @param string $address
   *   An address (for which to request voter information).
   *
   * @return Object|NULL
   *   The voter information reponse from Google Civic for the requested
   *   year and address--or NULL in case of failure.
   */
  public function request_voterinfo($election_id, $address) {
    if (!is_int($election_id)) {
      return NULL;
    }

    if (!$address) {
      return NULL;
    }

    $url = sprintf("%s/voterinfo/%d/lookup", GOOGLE_CIVIC_API, $election_id);
    $payload = array('address' => $address,);

    return $this->request($url, $payload);
  }
}
