<?php
// $Id: indymedia_cities.cron.inc,v 1.1.4.2 2010/08/14 07:38:58 mfb Exp $

/**
 * @file
 * Cron functions for Indymedia cities module.
 */

/**
 * Cycle through languages and update if refresh period has expired.
 */
function indymedia_cities_refresh() {
  $refresh = variable_get('indymedia_cities_refresh', 86400);
  $languages = variable_get('indymedia_cities_languages', array('en' => 1));
  foreach ($languages as $language => $status) {
    if ($status && variable_get('indymedia_cities_checked_' . $language, 0) + $refresh < REQUEST_TIME) {
      indymedia_cities_update($language, variable_get('indymedia_cities_modified_' . $language, 0));
      variable_set('indymedia_cities_checked_' . $language, time());
    }
  }
}

/**
 * Gets URI and updates list if required.
 */
function indymedia_cities_update($language, $modified = 0) {
  $languages = indymedia_cities_languages();
  $uri = $languages[$language];
  $options = $modified ? array('headers' => array('If-Modified-Since' => gmdate(DATE_RFC1123, $modified))) : array();
  $result = drupal_http_request($uri, $options);
  switch ($result->code) {
    case 304:
      watchdog('cron', 'Cities list at %uri not modified since last update.', array('%uri' => $uri));
      break;
    case 301:
      // Deal with permenent redirections? indymedia_log note?
    case 200:
    case 302:
    case 307:
      if (isset($result->headers['Last-Modified'])) {
        variable_set('indymedia_cities_modified_'. $language, strtotime($result->headers['Last-Modified']));
      }
      if (simplexml_load_string($result->data)) {
        cache_set('indymedia_cities:xml:' . $language, $result->data);
        watchdog('cron', 'Cities list updated from %uri.', array('%uri' => $uri));
        return $result;
      }
      else {
        watchdog('cron', 'Error reading XML from %uri.', array('%uri' => $uri), WATCHDOG_WARNING);
      }
      break;
    default:
      watchdog('cron', 'Received error code %error from %uri.', array('%uri' => $uri, '%error' => $result->code . ' ' . $result->error), WATCHDOG_WARNING);
  }
}
