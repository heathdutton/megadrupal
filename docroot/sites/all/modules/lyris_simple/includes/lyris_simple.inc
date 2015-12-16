<?php
/**
 * @file
 * Custom functionality for Lyris Simple Forms.
 */

/**
 * Convert an array into a URL safe data string.
 *
 * @param array $data
 *   An array of data to convert
 *
 * @return string
 */
function lyris_simple_prepare_data_string(array $data) {
  $items = array();

  foreach ($data as $k => $v) {
    $items[] = $k . '=' . urlencode(check_plain($v));
  }

  return implode('&', $items);
}

/**
 * Send request to Lyris.
 *
 * @param const $method
 *   LYRIS_SIMPLE_SUBSCRIBE | LYRIS_SIMPLE_UNSUBSCRIBE
 * @param array $data
 *   The data to send.
 *
 * @return object
 *   The response object from drupal_http_request().
 */
function lyris_simple_request($method, $data) {
  $action = $method == LYRIS_SIMPLE_SUBSCRIBE ? 'subscribe' : 'unsubscribe';
  $url = trim(variable_get('lyris_simple_host', ''), '/') . "/subscribe/{$action}.tml";

  $options = array(
    'method' => 'POST',
    'data' => lyris_simple_prepare_data_string($data),
  );

  return drupal_http_request($url, $options);
}

/**
 * Determine the response from Lyris.
 *
 * The Lyris response is an HTML page, not any type of useful data, so we try to
 * determine the result of the request by looking for specific phrases.
 *
 * @param string $response
 *   The response data from Lyris.
 *
 * @return const
 */
function lyris_simple_detect_lyris_response($response) {
  $options = array(
    LYRIS_SIMPLE_INVALID_EMAIL => 'is not a valid email address',
    LYRIS_SIMPLE_SUBSCRIBE_ALREADY_SUBSCRIBED => 'You are already a member',
    LYRIS_SIMPLE_SUBSCRIBE_SUCCESS => 'You have been subscribed',
    LYRIS_SIMPLE_SUBSCRIBE_PENDING => 'Subscription requested',
    LYRIS_SIMPLE_UNSUBSCRIBE_SUCCESS => 'has been received and will be processed shortly',
  );

  // Return the first status we can define.
  foreach ($options as $val => $contains) {
    if (stristr($response, $contains)) {
      return $val;
    }
  }

  return 'undefined';
}
