<?php
/**
 * @file
 * Function called on callers side to invoke remote function.
 */

/**
 * Call secure RPC on remote instance.
 *
 * It optionally notifies a D2D Server as specified by the module d2d_passport.
 *
 * @param array $friend
 *   The friend instance to which the request is sent
 *
 * @param string $method_name
 *   The name of the remote procedure
 *
 * @param array $arguments
 *   The array of arguments to remote procedure
 *
 * @param string &$error_string
 *   A string containing a message in case of failure
 *
 * @param bool $notify
 *   If TRUE, and d2d_passport module is enabled, and D2D_PASSPORT_NOTIFY 
 *   is TRUE, then the specified D2D server is notified of the request.
 *   Defaults, TRUE
 *
 * @return bool|string
 *   The result of the execution of the remote procedure, or FALSE on failure
 */
function d2d_call_secure_rpc($friend, $method_name, $arguments, &$error_string, $notify = TRUE) {
  if (!d2d_is_online()) {
    $error_string = t('cannot call secure rpc if instance is not online');
    return FALSE;
  }
  if (is_null($friend['public_key'])) {
    $error_string = t('no public key associated with friend');
    return FALSE;
  }
  $my_instance = d2d_api_own_instance_get(TRUE);
  $imploded_arguments = d2d_implode($arguments);
  $nonce = d2d_random_nonce();
  $request = array(
    'type' => 'secure_rpc',
    'version' => d2d_get_version(),
    'method_name' => $method_name,
    'arguments' => $imploded_arguments,
    'callee' => $friend['d2d_id'],
    'caller' => $my_instance['d2d_id'],
    'timestamp' => d2d_get_time(),
    'nonce' => $nonce,
  );
  $imploded_request = d2d_implode($request);
  if ($imploded_request === FALSE) {
    $error_string = t('error imploding request');
    return FALSE;
  }
  // Encrypt request.
  if (!d2d_encrypt($imploded_request, $encrypted_request, $friend['public_key'])) {
    $error_string = t('failed encryption');
    return FALSE;
  }
  // Sign encrypted request.
  $signed_request = d2d_sign($encrypted_request, $my_instance['private_key'], $my_instance['d2d_id']);
  if ($signed_request === FALSE) {
    $error_string = t('failed signing');
    return FALSE;
  }

  // If d2d_passport is enabled notify it.
  if ($notify && function_exists('d2d_passport_notify') 
      && variable_get('D2D_PASSPORT_NOTIFY', FALSE)) {
      d2d_passport_notify($my_instance, $friend, $method_name, strlen($imploded_arguments));
  }

  $result64 = xmlrpc($friend['url'], array('d2d.secureRPC' => array(base64_encode($signed_request), d2d_get_version())), d2d_xmlrpc_options());
  if ($result64 === FALSE) {
    $error_string = t('remote error on !friend:', array('!friend' => $friend['url'])) . ' ' . xmlrpc_error_msg();
    return FALSE;
  }
  // Convert $result64 in MIME base64.
  $result = base64_decode($result64, TRUE);
  if ($result === FALSE) {
    $error_string = t('failed base64 decoding');
    return FALSE;
  }
  // Explode $result.
  $exploded_result = d2d_explode($result);
  if ($exploded_result === FALSE) {
    $error_string = t('could not explode result');
    return FALSE;
  }
  // Check if $exploded_result is of desired structure.
  if (!d2d_check_array(
    $exploded_result,
    array(
      'data' => 'is_string',
      'signature' => 'is_string',
      'signer' => 'is_string',
    )
  )) {
    $error_string = t('exploded result is not of desired strucutre');
    return FALSE;
  }
  $signer = $exploded_result['signer'];
  if ($signer !== $friend['d2d_id']) {
    $error_string = t('signer of return result mismatch');
  }
  $encrypted_data = $exploded_result['data'];
  $signature = $exploded_result['signature'];
  if (openssl_verify($encrypted_data, $signature, $friend['public_key']) !== 1) {
    $error_string = t('wrong signature');
    return FALSE;
  }
  // Explode $encrypted_data.
  $cipher_envkey = d2d_explode($encrypted_data);
  if ($cipher_envkey === FALSE) {
    $error_string = t('failed exploding encrypted data');
    return FALSE;
  }
  // Check if exploded $encrypted_data is of desired structure.
  if (!d2d_check_array($cipher_envkey, array('cipher' => 'is_string', 'env_key' => 'is_string'))) {
    $error_string = t('encrypted data not of desired structure');
    return FALSE;
  }
  // Finally decrypt.
  if (!openssl_open($cipher_envkey['cipher'], $data, $cipher_envkey['env_key'], $my_instance['private_key'])) {
    $error_string = t('failed decrypytion');
    return FALSE;
  }
  // Explode $data.
  $exploded_data = d2d_explode($data);
  if ($exploded_data === FALSE) {
    $error_string = t('failed explosion');
    return FALSE;
  }
  // Check if $exploded_data is of desired structure.
  $check = array(
    'type' => 'is_string',
    'version' => 'is_string',
    'method_name' => 'is_string',
    'return' => 'is_string',
    'callee' => 'is_string',
    'caller' => 'is_string',
    'timestamp' => 'd2d_check_convert_int',
    'nonce' => 'd2d_check_nonce',
  );
  if (!d2d_check_array($exploded_data, $check)) {
    $error_string = t('exploded data not of desired structure');
    return FALSE;
  }
  if ($exploded_data['type'] !== 'secure_rpc_return') {
    $error_string = t('type != secure_rpc_return');
    return FALSE;
  }
  if ($exploded_data['version'] !== d2d_get_version()) {
    $error_string = t('version in exploded data mismatch');
    return FALSE;
  }
  if ($method_name !== $exploded_data['method_name']) {
    $error_string = t('method_name mismatch');
    return FALSE;
  }
  if ($exploded_data['caller'] !== $my_instance['d2d_id']) {
    $error_string = t('caller in exploded data mismatch');
    return FALSE;
  }
  if ($exploded_data['callee'] !== $friend['d2d_id']) {
    $error_string = t('callee in exploded data mismatch');
    return FALSE;
  }
  if (!d2d_verify_timestamp(d2d_get_time(), $exploded_data['timestamp'])) {
    $error_string = t('timestamp too old or too new');
    return FALSE;
  }
  if ($exploded_data['nonce'] !== $nonce) {
    $error_string = t('returned nonce not equal to nonce sent');
    return FALSE;
  }
  return $exploded_data['return'];
}

/**
 * D2D RPC callback; handles a secure RPC.
 *
 * Only friends with the right privileges can use this method.
 *
 * @param string $request64
 *   Specially MIME base64 encoded string
 *
 * @param string $version
 *   The D2D version of the caller
 *
 * @return StdClass|string
 *   An xmlrpc_error on error, or encrypted and signed string
 *   containing the result of the remote procedure on success
 *
 * @see d2d_secure_request()
 */
function d2d_secure_rpc($request64, $version) {
  $t = d2d_get_time();
  if (!d2d_is_online()) {
    return xmlrpc_error(0, 'instance is not online');
  }
  // Check version.
  if ($version !== d2d_get_version()) {
    return xmlrpc_error(0, 'wrong version');
  }
  // Convert $request64 from MIME base64.
  // Note that if $request64 is binary, an xml-rpc-error is returned.
  $request = base64_decode($request64, TRUE);
  if ($request === FALSE) {
    return xmlrpc_error(0, 'failed base64 decoding');
  }
  // Explode $request.
  $message = d2d_explode($request);
  if ($message === FALSE) {
    return xmlrpc_error(0, 'could not explode request');
  }
  // Check if exploded $request is of desired structure.
  if (!d2d_check_array(
    $message,
    array(
      'data' => 'is_string',
      'signature' => 'is_string',
      'signer' => 'd2d_check_d2d_id',
    )
  )) {
    return xmlrpc_error(0, 'request is not of desired strucutre');
  }
  $signer = $message['signer'];
  $my_instance = d2d_api_own_instance_get(TRUE);
  if ($signer === $my_instance['d2d_id']) {
    return xmlrpc_error(0, 'cannot process requests sent by myself');
  }
  $encrypted_data = $message['data'];
  $signature = $message['signature'];
  $friend = d2d_api_instance_get($signer);
  if (!$friend || !$friend['is_friend'] || is_null($friend['public_key']) || openssl_verify($encrypted_data, $signature, $friend['public_key']) !== 1) {
    return xmlrpc_error(0, 'no friendship found or wrong signature');
  }
  // Explode $encrypted_data.
  $cipher_envkey = d2d_explode($encrypted_data);
  if ($cipher_envkey === FALSE) {
    return xmlrpc_error(0, 'failed exploding encrypted data');
  }
  // Check if exploded $encrypted_data is of desired structure.
  if (!d2d_check_array($cipher_envkey, array('cipher' => 'is_string', 'env_key' => 'is_string'))) {
    return xmlrpc_error(0, 'encrypted data not of desired structure');
  }
  // Finally decrypt.
  if (!openssl_open($cipher_envkey['cipher'], $data, $cipher_envkey['env_key'], $my_instance['private_key'])) {
    return xmlrpc_error(0, 'failed decrypytion');
  }
  // Explode $data.
  $exploded_data = d2d_explode($data);
  if ($exploded_data === FALSE) {
    return xmlrpc_error(0, 'failed explosion');
  }
  // Check if $exploded_data is of desired structure.
  $check = array(
    'type' => 'is_string',
    'version' => 'is_string',
    'method_name' => 'is_string',
    'arguments' => 'is_string',
    'callee' => 'd2d_check_d2d_id',
    'caller' => 'd2d_check_d2d_id',
    'timestamp' => 'd2d_check_convert_int',
    'nonce' => 'd2d_check_nonce',
  );
  if (!d2d_check_array($exploded_data, $check)) {
    return xmlrpc_error(0, 'exploded data not of desired structure');
  }
  if ($exploded_data['type'] !== 'secure_rpc') {
    return xmlrpc_error(0, 'type != secure_rpc');
  }
  if ($exploded_data['version'] !== $version) {
    return xmlrpc_error(0, 'version in exploded data mismatch');
  }
  $method_name = $exploded_data['method_name'];
  $arguments = $exploded_data['arguments'];
  if ($exploded_data['callee'] !== $my_instance['d2d_id']) {
    return xmlrpc_error(0, 'callee in exploded data mismatch');
  }
  if ($exploded_data['caller'] !== $friend['d2d_id']) {
    return xmlrpc_error(0, 'caller in exploded data mismatch');
  }
  if (!d2d_verify_timestamp($t, $exploded_data['timestamp'])) {
    return xmlrpc_error(0, 'timestamp too old or too new');
  }
  if (!d2d_verify_nonce($exploded_data['timestamp'], $exploded_data['nonce'])) {
    return xmlrpc_error(0, 'duplicate nonce detected');
  }
  // Check if method name is properly mapped.
  $hks = module_invoke_all('d2d_secure_rpc');
  if (!array_key_exists($method_name, $hks)) {
    return xmlrpc_error(0, 'method does not exist: ' . $method_name);
  }
  $hk = $hks[$method_name];
  if (!is_array($hk)) {
    return xmlrpc_error(0, 'method exists but is not properly implemented');
  }
  if (!d2d_api_method_permission_exists($friend['id'], $method_name)) {
    return xmlrpc_error(0, 'permission denied for method: ' . $method_name);
  }
  $argument_checks = (array_key_exists('arguments', $hk) ? $hk['arguments'] : array());
  $callback = $hk['callback'];
  $exploded_arguments = d2d_explode($arguments);
  if ($exploded_arguments === FALSE) {
    return xmlrpc_error(0, 'failed explosion of arguments');
  }
  if (!d2d_check_array($exploded_arguments, $argument_checks)) {
    return xmlrpc_error(0, 'arguments failed check');
  }
  $rpc_info = array(
    'instance' => $friend,
    'ip' => ip_address(),
  );
  try {
    $ret = call_user_func($callback, $exploded_arguments, $rpc_info);
  }
  catch (D2DRemoteException $e) {
    return xmlrpc_error(0, $e->getMessage());
  }
  catch (Exception $e) {
    // In case an exception occurs that is not a D2DRemoteException we log the
    // exception message together with name and arguments of the function being
    // called. The string representation of $exploded_arguments can be
    // arbitrarily long and is truncated in case it is too long.
    $arguments_string = var_export($exploded_arguments, TRUE);
    if (!is_string($arguments_string)) {
      $arguments_string = '';
    }
    elseif (strlen($arguments_string) > 1024) {
      $arguments_string = substr($arguments_string, 0, 1024);
    }
    $watchdog_argument = array(
      '%callback' => var_export($callback, TRUE),
      '%exception_message' => $e->getMessage(),
      '%arguments' => $arguments_string,
      '%rpc_info' => var_export($rpc_info, TRUE),
    );
    watchdog('D2D', 'Exception in d2d_secure_rpc: \'%exception_message\', $callback=%callback, $arguments=%arguments, $rpc_info=%rpc_info', $watchdog_argument);
    return xmlrpc_error(0, 'exception in the function called on remote host');
  }
  if (!is_string($ret)) {
    return xmlrpc_error(0, 'called function did not return string');
  }
  $return = array(
    'type' => 'secure_rpc_return',
    'version' => d2d_get_version(),
    'method_name' => $method_name,
    'return' => $ret,
    'callee' => $my_instance['d2d_id'],
    'caller' => $friend['d2d_id'],
    'timestamp' => d2d_get_time(),
    'nonce' => $exploded_data['nonce'],
  );
  $imploded_return = d2d_implode($return);
  if ($imploded_return === FALSE) {
    return xmlrpc_error(0, 'error imploding return');
  }
  // Encrypt return.
  if (!d2d_encrypt($imploded_return, $encrypted_return, $friend['public_key'])) {
    return xmlrpc_error(0, 'failed encryption!');
  }
  // Sign encrypted return.
  $signed_return = d2d_sign($encrypted_return, $my_instance['private_key'], $my_instance['private_key']);
  if ($signed_return === FALSE) {
    return xmlrpc_error(0, 'failed signing');
  }
  return base64_encode($signed_return);
}
