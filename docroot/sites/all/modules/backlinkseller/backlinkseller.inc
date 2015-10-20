<?php
/**
 * @file
 * Original from backlinkseller.de.
 */

/**
 * Backlinkseller website-code v2.
 */
function _backlinkseller_fetch_links() {
  $request_uri = request_uri();
  if (drupal_substr($request_uri, 0, 1) == '/') {
    $request_uri = drupal_substr($request_uri, 1);
  }

  $backlinkseller_site_url = variable_get('backlinkseller_site_url', url('', array('absolute' => TRUE)));
  $page_url = url($backlinkseller_site_url . $request_uri);

  $scheme = 'http://';
  $host = 'channel8.backlinkseller';
  $tdl = '.de';
  $port = 80;
  $path = '/channel/';
  $query = array(
    'id' => check_plain(variable_get('backlinkseller_id')),
    'page' => $page_url,
    'ip' => ip_address(),
  );

  $debug_flag = (bool) variable_get('backlinkseller_debug_flag', 1);
  if ($debug_flag) {
    $query['debug'] = 1;
  }

  $result = NULL;

  // Try to connect .de Domain.
  $response = _backlinkseller_send_request($scheme, $host, $tdl, $port, $path, $query);

  // If failed, try to connect .com Domain.
  if (!$response) {
    $tdl = '.com';
    $response = _backlinkseller_send_request($scheme, $host, $tdl, $port, $path, $query);

    // .de & .com failed.
    if (!$response) {
      $result = '<!-- UNABLE_TO_CONNECT -->';
    }
  }

  if ($response) {
    // Remove quotes.
    $response->data = str_replace(array('\"', "\\'"), array('"', "'"), $response->data);

    if (strpos($response->data, '<response>') === FALSE || strpos($response->data, '</response>') === FALSE) {
      $result = '<!-- INVALID_RESPONSE -->';
    }
    else {
      // The HTML is in between of "response"-Tags. I must remove them.
      $result = str_replace(array('<response>', '</response>'), '', $response->data);
    }
  }

  return $result;
}


/**
 * Connect to the Server using drupal_http_request function.
 *
 * @param string $scheme
 *   Default http://.
 * @param string $host
 *   Hostname of the provider.
 * @param string $tdl
 *   Top-Level domain of the provider.
 * @param int $port
 *   The port number.
 * @param string $path
 *   Path where to send the paramenter.
 * @param array $query
 *   An array with parameter passed to the provider.
 *
 * @return bool|object
 *   Try to get data from Server. Return FALSE if fails.
 */
function _backlinkseller_send_request($scheme, $host, $tdl, $port, $path, array $query) {
  $url = url($scheme . $host . $tdl . $path, array('query' => $query));
  $response = drupal_http_request($url, array(
    'timeout' => 5,
  ));

  if ($response->code == 200) {
    return $response;
  }

  return FALSE;
}
