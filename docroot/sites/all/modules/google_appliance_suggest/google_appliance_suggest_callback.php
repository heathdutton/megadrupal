<?php

/**
 * @file 
 * The callback for the Google Appliance Suggest module's autocomplete.
 *
 * We do this rather than a Drupal menu callback to use as lightweight a Drupal
 * bootstrap as possible.
 */


// Try and load Drupal's bootstrap.inc.
define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once DRUPAL_ROOT . '/includes/common.inc';

// Bootstrap Drupal to the variables stage.
if (function_exists('drupal_bootstrap')) {
  drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES);
  $gsa_suggest_cache = variable_get('google_appliance_suggest_cache', 1);

  // Attempt to pull the query from cache.
  $cache = drupal_page_get_cache();
  if ($gsa_suggest_cache && is_object($cache)) {
    header('X-Drupal-Cache: HIT');
    drupal_serve_page_from_cache($cache);
    exit();
  }

  // Now that we're in Drupal, pull some configuration values.
  $gsa_hostname = variable_get('google_appliance_hostname', FALSE);
  $gsa_timeout = variable_get('google_appliance_timeout', 10);
  $gsa_collection = variable_get('google_appliance_collection', 'default_collection');
  $gsa_frontend = variable_get('google_appliance_frontend', 'default_frontend');
  $gsa_max_autosuggest = variable_get('google_appliance_suggest_max', 5);

  // Make sure we have a valid GSA host to connect to.
  if ($gsa_hostname) {
    // Build the search term to pass to GSA
    $search_query = $_GET['q'];

    // Create a list of query parameters from configurations.
    $get = array(
      'q' => $search_query,
      'max' => $gsa_max_autosuggest,
      'site' => $gsa_collection,
      'client' => $gsa_frontend,
      'access' => 'p',
      'format' => 'os',
    );

    // Fire off the GET request.
    $curl_defaults = array(
      CURLOPT_URL => $gsa_hostname . '/suggest?' . http_build_query($get, '', '&'),
      CURLOPT_HEADER => 0,
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_TIMEOUT => check_plain($gsa_timeout)
    ); 
    $ch = curl_init();
    curl_setopt_array($ch, $curl_defaults);
    $result = array(
      'is_error' => FALSE,
      'response' => curl_exec($ch),
    );

    // If we got a valid response, cache it and print it.
    if ($result['response'] !== FALSE) {
      // Format JSON to what Drupal expects on autocomplete.
      $json_source = drupal_json_decode($result['response']);
      $response = array();
      $strong = '<strong>' . $search_query . '</strong>';
      foreach ($json_source[1] as $item) {
        $response[$item] = str_replace($search_query, $strong, $item);
      }

      // Prepare content for caching and display it.
      ob_start();
      drupal_json_output($response);
      if ($gsa_suggest_cache) {
        header('X-Drupal-Cache: MISS');
        $cache = drupal_page_set_cache();
        drupal_serve_page_from_cache($cache);
      }
    }
    else {
      drupal_json_output(array());
    }

    curl_close($ch);
  }
}
else {
  // Fallback to returning nothing.
  drupal_json_output(array());
}
