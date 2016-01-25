<?php
/**
 * @file
 * Hooks provided by the WebClient proxy module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define WebClient request tags for custom proxy support.
 *
 * This hook enables modules to register request tags which can have alternative
 * proxy settings.
 *
 * hook_webclient_proxy_supported_request() implementations return an
 * associative array whose keys define the request tag and whose values are an
 * associative array of properties for each supported request. (The complete
 * list of properties is in the return value section below.)
 *
 * @return array
 *   An array of supported request tags. Each supported request has key
 *   corresponding to a tag being registered. The corresponding array value
 *   is an associative array that may contain the following key-value pairs:
 *   <ul>
 *    <li><b>"name"</b>: Required. The human readable name of the request tag.
 *   </ul>
 */
function hook_webclient_proxy_supported_request() {
  return array(
    'example_request_tag' => array(
      'name' => 'Example requests',
    ),
  );
}

/**
 * Allow modules to alter the supported requests.
 *
 * @param array $supported_requests
 *   An associative array containing the supported request tag information,
 *   keyed by the tag.
 *
 * @see hook_webclient_proxy_supported_request()
 */
function hook_webclient_proxy_supported_request_alter(&$supported_requests) {
  // Alter supported request information.
}

/**
 * @} End of "addtogroup hooks".
 */
