<?php
/**
 * @file
 *  wildfire.rpc.inc
 *
 *  Include file containing the methods for interacting with an RPC server
 *
 * @author Craig Jones <craig@tiger-fish.com>
 */

/**
 * Performs an XML-RPC request with Wildfire HQ
 *
 * @param string $method
 *  Method to call with the server
 * @param array $params
 *  Params to send to the server
 * @param string $auth_key
 *  Auth key for the request. If not given, will use the stored variable
 * @param string $system_name
 *  System name for the request. If not given, will use the stored variable
 *
 * @throws WildfireInsufficientQuotaException
 *  If the request failed due to insufficient quota
 * @throws UnexpectedValueException
 *  If the XML-RPC call returns a failure code for the call rather than the
 *  result.
 *
 * @return array
 *  Result of the request
 */
function wildfire_rpc_send_request($method, $params, $auth_key = NULL, $system_name = NULL) {

  /**
   * This is a bit messy, but will suffice for now. If we are in a test,
   * send the request to ourselves, else send it to WildfireHQ
   */
  $test_info = &$GLOBALS['drupal_test_info'];
  if (!empty($test_info['test_run_id'])) {
    global $base_url;
    $url = $base_url . '/xmlrpc.php';
  }
  else {
    $url = 'http://wildfirehq.co.uk/xmlrpc.php';
  }

  //
  // Recursively check $params does not contain any NULL values, as these
  // cannot be encoded for XMLRPC requests, and they end up being interpreted
  // as a blank string at the receiving end. This can cause the calculated
  // hash to not match the checksum the server performs on the incoming data.
  //
  // Arguably, the NULL values should never get into the $params array in the
  // first place, but this is primarily caused by obtaining fields off a user
  // entity to add them to the users metadata, where those fields have yet to
  // be assigned a value, and doing it this way caters for an unexpected
  // introduction of NULLs anywhere else.
  //
  array_walk_recursive($params, 'wildfire_rpc_null_to_empty_string');

  $input = array(
    'api' => WILDFIRE_RPC_API_VERSION,
    'system' => !empty($system_name) ? $system_name : variable_get('wildfire_system_name', ''),
    'hash' => '',
    'params' => $params
  );

  if (empty($auth_key)) {
    $auth_key = variable_get('wildfire_rpc_authkey', '');
  }
  $params = serialize($input['params']);

  $input['hash'] = sha1($auth_key . $params);

  $result = xmlrpc($url, array($method => $input));

  $error_message = xmlrpc_error_msg();
  $error_code = xmlrpc_errno();

  if (!empty($error_message)) {

    /**
     * If an error was raised, throw the relevant exception for the error
     * code that was returned.
     */
    switch ($error_code) {

      case WILDFIRE_RPC_ERROR_INSUFFICIENT_QUOTA:
        throw new WildfireInsufficientQuotaException($error_message);
      break;

      default:
        // TODO: A better exception object is needed to represent outright
        // unknown XML-RPC errors
        throw new UnexpectedValueException(
          t(
            'XML-RPC call failed, Code=!code, Message="!message"',
            array(
              '!code' => $error_code,
              '!message' => $error_message,
            )
          )
        );
      break;

    }
  }
  else {

    /**
     * Request was a success, return the result
     */
    return $result;

  }

}

/**
 * Callback to convert any NULL values in the input params to an empty string.
 *
 * @param string &$item
 *   The item from the source array.
 *
 * @param string &$key
 *   The key for the item in the source array.
 *
 * @return string
 *   The converted string.
 *
 * @see wildfire_rpc_send_request()
 */
function wildfire_rpc_null_to_empty_string(&$item, &$key) {

  if (is_null($item)) {
    $item = '';
  }

}
