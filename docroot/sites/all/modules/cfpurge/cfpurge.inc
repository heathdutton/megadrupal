<?php

/**
 * @file
 * Contains the main purging functionality and error handling
 */

/**
 * Purges urls from CloudFlare cache.
 *
 * @param array $cfpurge_urls
 *   Array of urls to purge from CloudFlare cache using secure POST method.
 *
 * @return array
 *   Array of urls and their http status codes after purging.
 */
function cfpurge_urls($cfpurge_urls) {

  static $url_cache;
  $urls = array();

  // Initialize static URL array, used for preventing duplicate purges.
  if (!isset($url_cache)) {
    $url_cache = array();
  }

  // Info required for each purge request.
  $params["a"]        = "zone_file_purge";
  $params["tkn"]      = variable_get('cfpurge_api_key', '');
  $params["email"]    = variable_get('cfpurge_email', '');
  $params["z"]        = variable_get('cfpurge_zone', '');

  // Attach the obove info to each URL.
  foreach ($cfpurge_urls as $url) {

    // Skips URLs which have already been purged.
    if (in_array($url, $url_cache)) {
      continue;
    }

    // Append absolute URL to data array.
    $absolute = url($url, array('alias' => 1, 'absolute' => 1));
    $prefixed = variable_get('cfpurge_url_prefix', url('', array('absolute' => TRUE))) .
      str_replace(url('', array('absolute' => TRUE)), '', $url);

    // Set URL to purge from CF.
    $params['url'] = $prefixed;

    // Make sure page is cleared from local cache if enabled.
    if (variable_get('cache', 0)) {
      cache_clear_all($absolute, 'cache_page');
    }

    // Save the URL to a static variable.
    $url_cache[] = $url;

    // Append the URL to the purge queue.
    $urls[] = $params;
  }

  // Get rid of blocks' content from Drupal's cache.
  if (count($urls) && module_exists('block') && variable_get('block_cache', 0)) {
    cache_clear_all('*', 'cache_block', 1);
  }

  // Check if requests should be handled in parallel.
  if (variable_get('cfpurge_parallel', 0)) {
    return cfpurge_parallel_request($urls);
  }

  // Else use the less CPU intensive method.
  return cfpurge_serial_request($urls);
}


/**
 * Issue cfpurge request using serial curl requests.
 *
 * @param array $urls
 *   Array of purge requests ready to be sent using curl.
 *
 * @return array
 *   Array of purge requests along with their responses after purging.
 */
function cfpurge_serial_request($urls) {

  // If no urls don't run.
  if (empty($urls)) {
    return $urls;
  }

  // Initialize a curl request for each URL.
  foreach ($urls as $key => $url) {

    $curl_cfpurge = curl_init();

    curl_setopt($curl_cfpurge, CURLOPT_URL, variable_get('cfpurge_api_url', 'https://www.cloudflare.com/api_json.html'));
    curl_setopt($curl_cfpurge, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($curl_cfpurge, CURLOPT_POST, 1);
    curl_setopt($curl_cfpurge, CURLOPT_POSTFIELDS, $url);
    curl_setopt($curl_cfpurge, CURLOPT_TIMEOUT, 5);
    curl_setopt($curl_cfpurge, CURLOPT_FOLLOWLOCATION, TRUE);
    curl_setopt($curl_cfpurge, CURLOPT_AUTOREFERER, TRUE);
    curl_setopt($curl_cfpurge, CURLOPT_SSL_VERIFYPEER, FALSE);

    $response = curl_exec($curl_cfpurge);
    $body = array_pop(explode("\r\n\r\n", $response));

    $info = curl_getinfo($curl_cfpurge);
    $urls[$key]['http_code'] = $info['http_code'];
    $urls[$key]['content'] = drupal_json_decode($body);

  }
  return $urls;

}

/**
 * Issue cfpurge request using parallel curl requests.
 *
 * @param array $urls
 *   Array of purge requests ready to be sent using curl.
 *
 * @return array
 *   Array of purge requests along with their responses after purging.
 */
function cfpurge_parallel_request($urls){
  $api = variable_get('cfpurge_api_url', 'https://www.cloudflare.com/api_json.html');
  $mh = curl_multi_init();
  $curl_array = array();
  foreach($urls as $i => $url) {
    $curl_array[$i] = curl_init($api);
    curl_setopt($curl_array[$i], CURLOPT_RETURNTRANSFER, true);
    curl_setopt($curl_array[$i], CURLOPT_POSTFIELDS, $url);
    curl_multi_add_handle($mh, $curl_array[$i]);
  }
  $running = NULL;
  do {
    usleep(10000);
    curl_multi_exec($mh,$running);
  } while($running > 0);

  $res = array();
  foreach($urls as $i => $url) {
    $res[$url['url']] = curl_multi_getcontent($curl_array[$i]);
  }

  $info = curl_getinfo($curl_array[0]);

  if ( $info['http_code'] != 200 || $info['download_content_length'] < 0){
    watchdog(
      "cfpurge",
      "There was a problem with CURL when trying to connect to $api. " . ucfirst(curl_error($curl_array[0]))
    );
  }

  // Result collection. Collects the http code returned for each url purged.
  foreach ($urls as $i => $url) {
    $info = curl_getinfo($curl_array[$i]);
    $urls[$i]['http_code'] = $info['http_code'];
    $urls[$i]['content'] = drupal_json_decode(curl_multi_getcontent($curl_array[$i]));
    curl_multi_remove_handle($mh, $curl_array[$i]);
  }

  foreach($urls as $i => $url) {
    curl_multi_remove_handle($mh, $curl_array[$i]);
  }
  curl_multi_close($mh);

  return $urls;
}


/**
 * Logs successful CloudFlare purges and errors to the watchdog.
 *
 * @param array $responses
 *   Array of purge requests along with their responses.
 */
function cfpurge_logging($responses) {
  $purges = array();
  $errors = array();
  foreach ($responses as $response) {
    if ($response['http_code'] == 200 && $response['content']['result'] == 'success') {
      $purges[] = $response['url'];
    }
    elseif ($response['http_code'] == 200 && $response['content']['result'] != 'success') {
      $errors[] = $response['url'] . ' -- ' . $response['content']['msg'];
    }
    else {
      $errors[] = 'Error, There was a problem connecting to CloudFlare, here\'s the HTTP error code: ' . $response['http_code'];
    }
  }

  // Log number of succesful purges along with the URLs.
  if (count($purges)) {
    watchdog(
      'cfpurge',
      '!success_count URLs have been successfully purged from CloudFlare:<br /> !purge_log',
      array(
        '!success_count' => count($purges),
        '!purge_log' => implode('<br />', $purges),
      )
    );
  }

  // Log number of errors encountered along with the URLs.
  if (count($errors)) {
    watchdog(
        'cfpurge',
        '!errors_count errors have been encountered when purging these URLs:<br /> !cfpurge_log',
        array(
          '!errors_count' => count($errors),
          '!cfpurge_log' => implode('<br />', $errors),
        ),
        $severity = WATCHDOG_ERROR
      );
  }
}
