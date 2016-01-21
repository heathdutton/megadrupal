<?php

/**
 * @file
 * Page callbacks for the SharedVue module.
 */

/**
 * Page callback for the main syndicated content page
 */
function _sharedvue_content($sharedvue_endpoint_key) {

  // Grab the our base url either from config or from Drupal's global
  $base_url = variable_get('sharedvue_base_url', $GLOBALS['base_url']);

  // Put our request uri on the end of it
  $url = $base_url . request_uri();

  // And strip out the url scheme as SharedVue do not want this
  $url = str_replace(parse_url($url, PHP_URL_SCHEME) . '://', '', $url);

  // Make our request
  $response = drupal_http_request('http://' . $sharedvue_endpoint_key . '.sharedvue.net/Sharedvue/pull/?' . drupal_http_build_query(array('svhost' => $url)));

  // Did we get a response ok?
  if ($response->code == 200) {

    // Strip stylesheets from the returned content and place them in the <head>
    $content = preg_replace_callback('(<link.*\/>)', '_sharedvue_strip_stylesheets', $response->data);

    return $content;
  }
  else {
    // Otherwise log an error
    watchdog('sharedvue', 'Failed to retrieve content from SharedVue. The error was "@error"', array('@error' => $response->error), WATCHDOG_ERROR);
  }

  // If we've fallen through to here, we've failed to get any content back so return a 404
  return MENU_NOT_FOUND;
}
