<?php
/**
 * @file
 * Code for the ngApp module.
 */

/**
 * Allows modules to provide responses to requests from the Angular app.
 *
 * The X-XSRF-TOKEN request header has already been validated if this hook is
 * invoked.
 *
 * @param string $key
 *   The menu argument identifying the response to be returned.
 * @param array $payload
 *   The json-decoded POST arguments supplied by the Angular app.
 *
 * @return array
 *   The data to return to the Angular app.
 */
function hook_ngapp_response($key, array $payload) {
  $response = array();

  return $response;
}
/**
 * Allows modules to save drupalLocalStorage data when config option is set.
 *
 * @param array $payload
 *   The json-decoded POST arguments supplied by the Angular app.
 *
 * @return array
 *   The data to return to the Angular app.
 */
function hook_ngapp_local_storage_save(array $payload) {
  $response = array();

  return $response;
}
