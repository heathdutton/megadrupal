<?php

/**
 * @file
 * Non hook implementations for Services Gov Format module.
 */

/**
 * Wrapper for services_error(), using WH API Standards parameters.
 *
 * @param int $status_code
 *   The HTTP status code to be returned to the client, e.g. 200, 400, 500.
 * @param string $developer_message
 *   A verbose, plain language description of the problem. Provide developers
 *   suggestions about how to solve their problems here.
 * @param string $user_message
 *   (optional) A message that can be passed along to end-users, if needed.
 *   Defaults to NULL.
 * @param string $more_info
 *   (optional) A URL to visit for more information about the error. Defaults
 *   to NULL.
 * @param int $error_code
 *   (optional) An internally determined error number. Defaults to NULL.
 *
 * @return array
 *   In the case of a 5xx error, an reponse array is returned directly.
 *
 * @see https://github.com/WhiteHouse/api-standards#error-handling
 *
 * @throws ServicesException()
 */
function services_govformat_throw_error($status_code, $developer_message, $user_message = NULL, $more_info = NULL, $error_code = NULL) {

  // Load services.runtime.inc from the Services module.
  module_load_include('inc', 'services', 'services.runtime');

  // Store our custom error response variables in an arbitrary data object,
  // to be passed with other errors.
  $data = array(
    'userMessage' => $user_message,
    'errorCode' => $error_code,
    'moreInfo' => $more_info,
  );

  // Throwing 5xx errors will cause varnish to override our response, so we
  // can't return a 500 error in the HTTP header, only in the response body.
  if (substr($status_code, 0, 1) == 5) {
    $response_params = array(
      'developer_message' => "{$status_code}: {$developer_message}",
      'status_code' => $status_code,
      'error_code' => $error_code,
      'more_info' => $more_info,
      'user_message' => $user_message,
    );
    $response = services_govformat_format($response_params);

    return $response;
  }

  // Throw the error. This will be formatted via
  // services_govformat_rest_server_execute_errors_alter().
  services_error($developer_message, $status_code, $data);
}
