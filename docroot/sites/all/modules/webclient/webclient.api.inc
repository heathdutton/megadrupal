<?php
/**
 * @file
 * Hooks provided by the WebClient module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define response formatters.
 *
 * This hook enables modules to register response formatters which auto convert
 * response data for a specific MIME type.
 *
 * hook_webclient_response_formatter_info() implementations return an
 * associative array whose keys define the MIME type and whose values are an
 * associative array of properties for each response formatter. (The complete
 * list of properties is in the return value section below.)
 *
 * Each response formatter should inherit from the 'WebResponseFormatter' class
 * and override the 'format' function.
 *
 * @return array
 *   An array of response formatters. Each response formatter has key
 *   corresponding to a MIME type being registered. The corresponding array
 *   value is an associative array that may contain the following key-value
 *   pairs:
 *   <ul>
 *    <li><b>"name"</b>: Required. The name of the response formatter.</li>
 *    <li><b>"description"</b>: Description of the response formatter.</li>
 *    <li><b>"class"</b>: Required. The response formatter class name.</li>
 *   </ul>
 */
function hook_webclient_response_formatter_info() {
  return array(
    'application/json' => array(
      'name' => 'JSON formatter',
      'description' => 'Format response data as decoded JSON string.',
      'class' => 'WebResponseJSONFormatter',
    ),
  );
}

/**
 * Allow modules to alter the response formatters info.
 *
 * @param array $formatters
 *   An associative array containing the formatter information, keyed by the
 *   MIME type.
 *
 * @see hook_webclient_response_formatter_info()
 */
function hook_webclient_response_formatter_info_alter(&$formatters) {
  // Alter formatter information.
}

/**
 * Allow modules to change the request class depending on the URL.
 *
 * @return mixed
 *   A string containing the class name of an request class which implements the
 *   WebRequest class. Returning NULL will ignore the result from this hook and
 *   move on to the next one.
 */
function hook_webclient_discover_request_class($url) {
  // Use the GoogleWebRequest for request to google otherwise ignore this hook.
  return (strpos($url, 'http://www.google.be') === 0 ? 'GoogleWebRequest' : NULL);
}

/**
 * Allow modules to alter the request before it is send to the server.
 */
function hook_webclient_request_alter(WebRequest &$request) {
  // Alter the request object.
}

/**
 * Allow modules to alter the request before it is send to the server.
 *
 * This hook will only allow altering of requests which have a specific TAG
 * linked to them.
 */
function hook_webclient_request_TAG_alter(WebRequest &$request) {
  // Alter the request object.
}

/**
 * @} End of "addtogroup hooks".
 */
