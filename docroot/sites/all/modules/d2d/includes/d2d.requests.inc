<?php
/**
 * @file
 * Managing and sending requests, handling incoming requests.
 */

/**
 * Implements hook_d2d_handle_outgoing_request().
 */
function d2d_d2d_handle_outgoing_request() {
  $types['d2d_friendship'] = array(
    'arguments_save' => array(),
    'arguments_send' => array('state' => 'is_string'),
    'arguments_return' => array('value' => 'is_string'),
    'callback_process' => 'd2d_friendship_outgoing_process',
    'callback_describe' => 'd2d_friendship_outgoing_describe',
    'callback_return' => 'd2d_friendship_outgoing_return',
  );
  return $types;
}

/**
 * Implements hook_d2d_handle_incoming_request().
 */
function d2d_d2d_handle_incoming_request() {
  $types['d2d_friendship'] = array(
    'arguments' => array('state' => 'is_string'),
    'arguments_return' => array('value' => 'is_string'),
    'callback' => 'd2d_friendship_incoming_process',
  );
  return $types;
}

/**
 * Implements hook_d2d_handle_incoming_preprocess().
 */
function d2d_d2d_handle_incoming_request_preprocess() {
  $types['d2d_friendship'] = array(
    'callback' => 'd2d_friendship_incoming_preprocess',
  );
  return $types;
}

/**
 * Adds an outgoing request to the database.
 *
 * @param int $receiver_id
 *   Instance id of the receiver
 *
 * @param string $type
 *   The type of the request, for example 'd2d_friendship'
 *
 * @param string $data
 *   The data to be sent
 *
 * @param int $max_resend_trials
 *   The maximum number of attempts to send the request
 *
 * @param bool $verbose
 *   If TRUE, results will be displayed using drupal_set_message
 *
 * @return int|NULL
 *   The id of the new request, or NULL if an error occurs
 */
function d2d_add_request($receiver_id, $type, $data, $max_resend_trials, $verbose = FALSE) {
  $hooks = module_invoke_all('d2d_handle_outgoing_request');
  if (!array_key_exists($type, $hooks)) {
    if ($verbose) {
      drupal_set_message(t('hook to handle outgoing request not implemented'), 'error');
    }
    return NULL;
  }
  $hook = $hooks[$type];
  $argument_checks = (array_key_exists('arguments_save', $hook) ? $hook['arguments_save'] : array());
  if (d2d_check_array($data, $argument_checks) === FALSE) {
    if ($verbose) {
      drupal_set_message(t('internal error processing request - data failed check'), 'error');
    }
    return NULL;
  }
  $data = d2d_implode($data);
  if ($data === NULL) {
    if ($verbose) {
      drupal_set_message(t('internal error processing request - data failed implosion'), 'error');
    }
    return NULL;
  }
  $t = d2d_get_time();
  $id = db_insert('d2d_outgoing_requests')->fields(array(
    'receiver_id' => $receiver_id,
    'type' => $type,
    'data' => $data,
    'time_insert' => $t,
    'time_next_send' => $t,
    'time_invalid' => $t + D2D_REQUEST_VALIDITY,
    'max_send_trials' => D2D_REQUEST_MAX_NUMBER_OF_RESENDS,
  ))->execute();
  return $id;
}

/**
 * Removes outdated outgoing requests from the database.
 */
function d2d_remove_outdated_outgoing_requests() {
  $t = d2d_get_time();
  // Remove requests that are invalid (time_invalid <= $t).
  db_delete('d2d_outgoing_requests')
    ->condition('time_invalid', $t, '<=')
    ->execute();
  // Remove requests that have been sent too often.
  db_delete('d2d_outgoing_requests')
    ->condition('max_send_trials', 0, '<=')
    ->execute();
  // TODO: a possible improvement would be
  // - get hooks that handle outgoing requests
  // - remove requests that cannot be handled
  // - if specified as an attributes of the hooks, remove certain requests
  // - if there is another request of the same type that is newer.
}

/**
 * Sends a single outgoing request.
 *
 * It optionally notifies a D2D Server as specified by the module d2d_passport.
 *
 * @param string $type
 *   The type of the request, for example 'd2d_friendship'
 *
 * @param array $arguments
 *   The array of arguments of the request
 *
 * @param array $instance
 *   The array of the instance
 *
 * @param array $my_instance
 *   Array describing the own instance
 *
 * @param string $my_private_key
 *   The private_key
 *
 * @param array $return_check
 *   An array of checks to be performed on the return array
 *
 * @param string &$error_string
 *   A string containing an error message, in case of failure
 *
 * @param bool $notify
 *   If TRUE, and d2d_passport module is enabled, and D2D_PASSPORT_NOTIFY
 *   is TRUE, then the specified D2D server is notified of the request.
 *   Defaults, TRUE
 *
 * @return NULL|bool|array
 *   NULL if the other instance received the request, but could
 *   not send a reply. FALSE, on failure, array containing
 *   the checked returned values on success.
 *
 * @see d2d_check_array()
 */
function d2d_send_single_outgoing_request($type, $arguments, $instance, $my_instance, $my_private_key, $return_check, &$error_string, $notify = TRUE) {
  $t = d2d_get_time();
  $imploded_arguments = d2d_implode($arguments);
  $nonce = d2d_random_nonce();
  $request = array(
    'type' => 'secure_request',
    'version' => d2d_get_version(),
    'request_type' => $type,
    'arguments' => $imploded_arguments,
    'receiver' => $instance['d2d_id'],
    'sender' => $my_instance['d2d_id'],
    'url_sender' => $my_instance['url'],
    'timestamp' => $t,
    'nonce' => $nonce,
  );
  $imploded_request = d2d_implode($request);
  if ($imploded_request === FALSE) {
    $error_string = t('Error imploding request.');
    return FALSE;
  }
  // Encrypt request.
  if (!d2d_encrypt($imploded_request, $encrypted_request, $instance['public_key'])) {
    $error_string = t('Failed encryption.');
    return FALSE;
  }
  // Sign encrypted request.
  $signed_request = d2d_sign($encrypted_request, $my_private_key, $my_instance['d2d_id']);
  if ($signed_request === FALSE) {
    $error_string = t('Failed signing.');
    return FALSE;
  }

  // If d2d_passport is enabled notify it.
  if ($notify && function_exists('d2d_passport_notify')
    && variable_get('D2D_PASSPORT_NOTIFY', FALSE)) {
    d2d_passport_notify($my_instance, $instance, $type, strlen($imploded_arguments));
  }

  $result64 = xmlrpc($instance['url'], array('d2d.secureRequest' => array(base64_encode($signed_request), d2d_get_version())), d2d_xmlrpc_options());
  if ($result64 === FALSE) {
    $error_string = t('Remote error on !instance:', array('!instance' => $instance['url'])) . ' ' . xmlrpc_error_msg();
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
  if (d2d_check_array(
    $exploded_result,
    array(
      'data' => 'is_string',
      'signature' => 'is_string',
      'signer' => 'is_string',
    )
  )) {
    $signer = $exploded_result['signer'];
    if ($signer !== $instance['d2d_id']) {
      $error_string = t('signer of return result mismatch');
      return FALSE;
    }
    $encrypted_data = $exploded_result['data'];
    $signature = $exploded_result['signature'];
    if (openssl_verify($encrypted_data, $signature, $instance['public_key']) !== 1) {
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
    if (!openssl_open($cipher_envkey['cipher'], $data, $cipher_envkey['env_key'], $my_private_key)) {
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
      'request_type' => 'is_string',
      'return' => 'is_string',
      'receiver' => 'd2d_check_d2d_id',
      'sender' => 'd2d_check_d2d_id',
      'timestamp' => 'd2d_check_convert_int',
      'nonce' => 'd2d_check_nonce',
    );
    if (!d2d_check_array($exploded_data, $check)) {
      $error_string = t('exploded data not of desired structure');
      return FALSE;
    }
    if ($exploded_data['type'] !== 'secure_request_return') {
      $error_string = t('type != secure_rpc_return');
      return FALSE;
    }
    if ($exploded_data['version'] !== d2d_get_version()) {
      $error_string = t('Version in exploded data mismatch.');
      return FALSE;
    }
    if ($exploded_data['request_type'] !== $type) {
      $error_string = t('Request_type mismatch.');
    }
    if ($exploded_data['receiver'] !== $instance['d2d_id']) {
      $error_string = t('Reiver in exploded data mismatch.');
      return FALSE;
    }
    if ($exploded_data['sender'] !== $my_instance['d2d_id']) {
      $error_string = t('Sender in exploded data mismatch.');
      return FALSE;
    }
    if (!d2d_verify_timestamp(d2d_get_time(), $exploded_data['timestamp'])) {
      $error_string = t('Timestamp too old or too new.');
      return FALSE;
    }
    if ($exploded_data['nonce'] !== $nonce) {
      $error_string = t('Returned nonce not equal to nonce sent.');
      return FALSE;
    }
    $exploded_return = d2d_explode($exploded_data['return']);
    if ($exploded_return === FALSE) {
      $error_string = t('Failed exploding return.');
      return FALSE;
    }
    if (!d2d_check_array($exploded_return, $return_check)) {
      $error_string = t('Exploded return not of desired structure.');
      return FALSE;
    }
    return $exploded_return;
  }
  elseif (d2d_check_array($exploded_result, array('signature' => 'is_string', 'signer' => 'is_string'))) {
    $signer = $exploded_result['signer'];
    if ($signer !== $instance['d2d_id']) {
      $error_string = t('Signer of return result mismatch.');
      return FALSE;
    }
    $signed_array = array(
      'type' => 'secure_request_return',
      'version' => d2d_get_version(),
      'version' => d2d_get_version(),
      'request_type' => $type,
      'arguments' => $imploded_arguments,
      'receiver' => $instance['d2d_id'],
      'sender' => $my_instance['d2d_id'],
      'url_sender' => $my_instance['url'],
      'timestamp' => $t,
      'nonce' => $nonce,
    );
    $signed_string = d2d_implode($signed_array);
    if ($signed_string === FALSE || openssl_verify($signed_string, $exploded_result['signature'], $instance['public_key']) !== 1) {
      $error_string = t('Wrong signature.');
      return FALSE;
    }
    return NULL;
  }
  else {
    $error_string = t('Exploded result is not of desired structure.');
    return FALSE;
  }
}

/**
 * Re-evaluates the signatures of the incoming request of specified instance.
 *
 * @param array $instance
 *   The associative array of the instance
 */
function d2d_incoming_requests_reevaluate_signatures($instance) {
  $ids = array();
  $result = db_query('SELECT id FROM {d2d_incoming_requests}' .
            ' WHERE sender_d2d_id <= :d2d_id ORDER BY id ASC',
            array(':d2d_id' => $instance['d2d_id'])
  );
  foreach ($result as $record) {
    $ids[] = $record->id;
  }
  $hks = module_invoke_all('d2d_handle_incoming_request');
  foreach ($ids as $request_id) {
    $result = db_query('SELECT sender_d2d_id, type, arguments, time_insert, timestamp, url, ip, signed_data, signature, signature_valid FROM {d2d_incoming_requests} WHERE id=:id', array(':id' => $request_id));
    $request = $result->fetchAssoc();
    if (!$request) {
      continue;
    }
    $type = $request['type'];
    $delete = FALSE;
    if (!array_key_exists($type, $hks)) {
      $delete = TRUE;
    }
    if (!$delete) {
      $hk = $hks[$type];
      // TODO: check if we need variable: $argument_checks.
      //$argument_checks = (array_key_exists('arguments', $hk) ? $hk['arguments'] : array());
      $callback = $hk['callback'];
      $exploded_arguments = d2d_explode($request['arguments']);
      $signature_valid_old = ($request['signature_valid'] != 0);
      if (is_null($instance['public_key']) || openssl_verify($request['signed_data'], $request['signature'], $instance['public_key']) !== 1) {
        $signature_valid_new = FALSE;
      }
      else {
        $signature_valid_new = TRUE;
      }
      if ($signature_valid_new != $signature_valid_old) {
        db_update('d2d_incoming_requests')
          ->fields(array('signature_valid' => ($signature_valid_new ? 1 : 0)))
          ->condition('id', $request_id)
          ->execute();
      }
      $rpc_info = array(
        'd2d_id' => $request['sender_d2d_id'],
        'signature_valid' => $signature_valid_new,
        'url' => $request['url'],
        'instance' => $instance,
        'timestamp' => $request['timestamp'],
        'ip' => $request['ip'],
      );
      try {
        $request_return = NULL;
        $keep = call_user_func_array(
          $callback,
          array(
            $exploded_arguments,
            $rpc_info,
            FALSE,
            &$request_return,
          )
        );
      }
      catch (D2DRemoteException $e) {
        $keep = FALSE;
      }
      $instance = d2d_api_instance_get($instance['d2d_id']);
      $delete = !$keep;
    }
    if ($delete) {
      db_delete('d2d_incoming_requests')
        ->condition('id', $request_id)
        ->execute();
    }
  }
}

/**
 * D2D RPC callback; handles secure request.
 *
 * Requests are special rpc, requests can be done by any instance,
 * even without permissions, friendship, or public key.
 *
 * Requests are put in the database, then processed.
 *
 * If the public key is initially missing, and then added, the
 * requests are later re-evaluated.
 *
 * @param string $request64
 *   Specially MIME base64 encoded string
 *
 * @param string $version
 *   The D2D version of the caller
 *
 * @return StdClass|string
 *   An xmlrpc_error on error, an encrypted and signed string on success
 *
 * @see d2d_secure_rpc()
 * @see _d2d_execute_preprocess_request_cb()
 */
function d2d_secure_request($request64, $version) {
  $t = d2d_get_time();
  if (!d2d_is_online()) {
    return xmlrpc_error(0, 'instance is not online');
  }
  // Check version.
  if ($version !== d2d_get_version()) {
    return xmlrpc_error(0, 'wrong version');
  }
  // Convert $request64 if in MIME base64.
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
  $instance = d2d_api_instance_get($signer);
  if ($instance === FALSE || is_null($instance['public_key'])) {
    $signature_valid = FALSE;
  }
  else {
    if (openssl_verify($encrypted_data, $signature, $instance['public_key']) !== 1) {
      return xmlrpc_error(0, 'wrong signature');
    }
    $signature_valid = TRUE;
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
    'request_type' => 'is_string',
    'arguments' => 'is_string',
    'receiver' => 'd2d_check_d2d_id',
    'sender' => 'd2d_check_d2d_id',
    'url_sender' => 'd2d_check_url',
    'timestamp' => 'd2d_check_convert_int',
    'nonce' => 'd2d_check_nonce',
  );
  if (!d2d_check_array($exploded_data, $check)) {
    return xmlrpc_error(0, 'exploded data not of desired structure');
  }
  if ($exploded_data['type'] !== 'secure_request') {
    return xmlrpc_error(0, 'type != secure_request');
  }
  if ($exploded_data['version'] !== $version) {
    return xmlrpc_error(0, 'version in exploded data mismatch');
  }
  if ($exploded_data['receiver'] !== $my_instance['d2d_id']) {
    return xmlrpc_error(0, 'receiver in exploded data mismatch');
  }
  if ($exploded_data['sender'] !== $signer) {
    return xmlrpc_error(0, 'sender exploded data mismatch');
  }
  if (!d2d_verify_timestamp($t, $exploded_data['timestamp'])) {
    return xmlrpc_error(0, 'timestamp too old or too new');
  }
  if (!d2d_verify_nonce($exploded_data['timestamp'], $exploded_data['nonce'])) {
    return xmlrpc_error(0, 'duplicate nonce detected');
  }
  // Check if request type is properly mapped.
  $hks = module_invoke_all('d2d_handle_incoming_request');
  if (!array_key_exists($exploded_data['request_type'], $hks)) {
    return xmlrpc_error(0, 'handler for request type does not exist');
  }
  $hk = $hks[$exploded_data['request_type']];
  if (!is_array($hk)) {
    return xmlrpc_error(0, 'handler for request type exists but is not properly implemented');
  }
  $argument_checks = (array_key_exists('arguments', $hk) ? $hk['arguments'] : array());
  $callback = $hk['callback'];
  $exploded_arguments = d2d_explode($exploded_data['arguments']);
  if ($exploded_arguments === FALSE) {
    return xmlrpc_error(0, 'failed explosion of arguments');
  }
  if (!d2d_check_array($exploded_arguments, $argument_checks)) {
    return xmlrpc_error(0, 'arguments failed check');
  }

  // Create request info array.
  $ip = ip_address();
  $rpc_info = array(
    'd2d_id' => $signer,
    'signature_valid' => $signature_valid,
    'url' => $exploded_data['url_sender'],
    'instance' => $instance,
    'timestamp' => $exploded_data['timestamp'],
    'ip' => $ip,
  );

  // Pre-process requests (e.g. if AUTO_ACCEPT flag is on, add instance and
  // set friendship status accordingly.
  $hks_preprocess = module_invoke_all('d2d_handle_incoming_request_preprocess');
  if (array_key_exists($exploded_data['request_type'], $hks_preprocess)) {
    $hk_preprocess = $hks_preprocess[$exploded_data['request_type']];
    $preprocess_cbs = $hk_preprocess['callback'];

    // In case the callback returns FALSE, return. No need to do more,
    // because everything is handled inside _d2d_execute_preprocess_request_cb.
    if (!is_array($preprocess_cbs)) {
      $preprocess_cbs = array($preprocess_cbs);
    }

    foreach ($preprocess_cbs as $preprocess_cb) {
      $success = FALSE;
      if (function_exists($preprocess_cb)) {
        $error_string = '';
        try {
          // Cannot use call_user_func_array because it does not pass
          // args by reference.
          $success = $preprocess_cb($exploded_arguments,
                     $rpc_info,
                     $encrypted_data,
                     $signature,
                     $error_string);
        }
        catch (D2DRemoteException $e) {
          return xmlrpc_error(0, 'Error on pre-processing: ' . $e->getMessage());
        }
        if ($success !== TRUE) {
          if ($error_string !== '') {
            return xmlrpc_error(0, 'Error on pre-processing: ' . $error_string);
          }
          else {
            return xmlrpc_error(0, 'Error on pre-processing.');
          }
        }
      }
      else {
        watchdog('d2d', 'Preprocess callback !cb for request %r does not exist.',
          array(
            '%r' => $exploded_data['request_type'],
            '!cb' => $preprocess_cb,
          ));
      }
    }
    // Some infor could have been modified by the pre-process hook.
    $instance = $rpc_info['instance'];
    $signature_valid = $rpc_info['signature_valid'];
  }

  // Finally process the request.
  try {
    $request_return = NULL;
    $keep = call_user_func_array(
      $callback,
      array(
        $exploded_arguments,
        $rpc_info,
        TRUE,
        &$request_return,
      )
    );
  }
  catch (D2DRemoteException $e) {
    return xmlrpc_error(0, $e->getMessage());
  }
  if ($instance === FALSE || is_null($instance['public_key'])) {
    if (!is_null($request_return)) {
      return xmlrpc_error(0, 'callback did not return proper return value');
    }
  }
  elseif (!is_null($request_return)) {
    $argument_checks = (array_key_exists('arguments_return', $hk) ? $hk['arguments_return'] : array());
    if (!d2d_check_array($request_return, $argument_checks)) {
      return xmlrpc_error(0, 'callback did not return proper return value');
    }
    $imploded_request_return = d2d_implode($request_return);
    if ($imploded_request_return === FALSE) {
      return xmlrpc_error(0, 'error imploding return');
    }
  }
  if ($keep) {
    db_insert('d2d_incoming_requests')->fields(array(
      'sender_d2d_id' => $signer,
      'type' => $exploded_data['request_type'],
      'arguments' => $exploded_data['arguments'],
      'time_insert' => d2d_get_time(),
      'timestamp' => $exploded_data['timestamp'],
      'url' => $exploded_data['url_sender'],
      'ip' => $ip,
      'signed_data' => $encrypted_data,
      'signature' => $signature,
      'signature_valid' => ($signature_valid ? 1 : 0),
    ))->execute();
  }
  if (is_null($request_return)) {
    // No public key available for sender, return can only be
    // signed but not encrypted.
    $return = array(
      'type' => 'secure_request_return',
      'version' => d2d_get_version(),
      'request_type' => $exploded_data['request_type'],
      'arguments' => $exploded_data['arguments'],
      'receiver' => $exploded_data['receiver'],
      'sender' => $exploded_data['sender'],
      'url_sender' => $exploded_data['url_sender'],
      'timestamp' => $exploded_data['timestamp'],
      'nonce' => $exploded_data['nonce'],
    );
    $imploded_return = d2d_implode($return);
    if ($imploded_return === FALSE) {
      return xmlrpc_error(0, 'error imploding return');
    }
    $signed_return = d2d_sign2($imploded_return, $my_instance['private_key'], $my_instance['d2d_id']);
    if ($signed_return === FALSE) {
      return xmlrpc_error(0, 'failed signing');
    }
    return base64_encode($signed_return);
  }
  else {
    $return = array(
      'type' => 'secure_request_return',
      'version' => d2d_get_version(),
      'request_type' => $exploded_data['request_type'],
      'return' => $imploded_request_return,
      'receiver' => $exploded_data['receiver'],
      'sender' => $exploded_data['sender'],
      'timestamp' => d2d_get_time(),
      'nonce' => $exploded_data['nonce'],
    );
    $imploded_return = d2d_implode($return);
    if ($imploded_return === FALSE) {
      return xmlrpc_error(0, 'error imploding return');
    }
    // Encrypt return.
    if (!d2d_encrypt($imploded_return, $encrypted_return, $instance['public_key'])) {
      return xmlrpc_error(0, 'failed encryption!');
    }
    // Sign encrypted return.
    $signed_return = d2d_sign($encrypted_return, $my_instance['private_key'], $my_instance['d2d_id']);
    if ($signed_return === FALSE) {
      return xmlrpc_error(0, 'failed signing');
    }
    return base64_encode($signed_return);
  }
}

/**
 * Send outgoing requests specified by their ids.
 *
 * @param array $ids
 *   The ids of the requests
 *
 * @param bool $verbose
 *   If TRUE, uses drupal_set_message to display the results on screen
 *
 * @return int
 *   The number of requests successfully sent
 */
function d2d_send_outgoing_requests($ids, $verbose = FALSE) {
  // Check if instance is online.
  if (!d2d_is_online()) {
    return 0;
  }
  // Number of successfully sent requests.
  $n_sucessfully_sent = 0;
  // Get current time and keys, url, d2d id of own instance.
  $t = d2d_get_time();
  $my_instance = d2d_api_own_instance_get();
  $my_private_key = d2d_api_own_private_key_get();
  // Get hooks that handle outgoing requests.
  $hooks = module_invoke_all('d2d_handle_outgoing_request');
  // Get information about all instances.
  $instances = d2d_api_instance_get_by_id();
  // Get ids of requests to be sent now.
  $send_now = TRUE;
  if (is_null($ids)) {
    $send_now = FALSE;
    $ids = array();
    // Select all outgoing requests, invalid ones have been removed earlier.
    $result = db_query('SELECT id FROM {d2d_outgoing_requests}' .
              ' ORDER BY id DESC'
    );
    foreach ($result as $record) {
      $ids[] = $record->id;
    }
  }
  $persistent_state = array();
  foreach ($ids as $request_id) {
    // Receive database entry for that request.
    $result = db_query('SELECT receiver_id, type, data, time_insert, time_next_send, time_invalid, max_send_trials FROM {d2d_outgoing_requests} WHERE id=:id', array(':id' => $request_id));
    $request = $result->fetchAssoc();
    if (!$request) {
      continue;
    }
    // Check if instance is in database,
    // if not: delete, continue.
    $delete = FALSE;
    if (!array_key_exists($request['receiver_id'], $instances)) {
      $delete = TRUE;
    }
    // Check if hook to handle the request exists,
    // if not: delete, continue.
    if (!$delete && !array_key_exists($request['type'], $hooks)) {
      $delete = TRUE;
    }
    else {
      $hook = $hooks[$request['type']];
    }
    // Check and convert data.
    if (!$delete) {
      $argument_checks = (array_key_exists('arguments_save', $hook) ? $hook['arguments_save'] : array());
      $arguments = d2d_explode($request['data']);
      if (d2d_check_array($arguments, $argument_checks) === FALSE) {
        $delete = TRUE;
      }
    }
    // Use function to handle the request to determine if request shall be
    // send now, later, or be deleted. Function also prepares data to be sent.
    if (!$delete) {
      if (!array_key_exists($request['type'], $persistent_state)) {
        $persistent_state[$request['type']] = array();
      }
      if (!array_key_exists($request['receiver_id'], $persistent_state[$request['type']])) {
        $persistent_state[$request['type']][$request['receiver_id']] = NULL;
      }
      $arguments_send = NULL;
      $return = call_user_func_array(
        $hook['callback_process'],
        array(
          $arguments,
          $request['time_insert'],
          $request['time_next_send'],
          $request['time_invalid'],
          $request['max_send_trials'],
          $instances[$request['receiver_id']],
          &$persistent_state[$request['type']][$request['receiver_id']],
          &$arguments_send,
        )
      );
      if ($return === FALSE) {
        continue;
      }
      elseif (is_null($return)) {
        $delete = TRUE;
      }
      else {
        $argument_checks = (array_key_exists('arguments_send', $hook) ? $hook['arguments_send'] : array());
        if (d2d_check_array($arguments_send, $argument_checks) === FALSE) {
          $delete = TRUE;
        }
      }
    }
    // Check if public key is available, if not: continue.
    if (!$delete && is_null($instances[$request['receiver_id']]['public_key'])) {
      continue;
    }
    // Ready for sending.
    // If max_send_trials is 0 then delete entry, set flag that
    // entry has been deleted,
    // otherwise update time_next_send and max_send_trials.
    $delete_before_send = FALSE;
    if (!$delete && ($send_now || $request['time_next_send'] <= $t)) {
      $request['max_send_trials']--;
      $request['time_next_send'] = $t + D2D_REQUEST_TIME_UNTIL_RESEND;
      if ($request['max_send_trials'] <= 0 || $request['time_next_send'] > $request['time_invalid']) {
        $delete_before_send = TRUE;
      }
      else {
        db_update('d2d_outgoing_requests')
          ->fields(array(
            'time_next_send' => $request['time_next_send'],
            'max_send_trials' => $request['max_send_trials'],
          ))
          ->condition('id', $request_id)
          ->execute();
      }
    }
    if ($delete || $delete_before_send) {
      db_delete('d2d_outgoing_requests')
        ->condition('id', $request_id)
        ->execute();
    }
    if ($delete) {
      continue;
    }
    // Call function that sends the request.
    $return_checks = (array_key_exists('arguments_return', $hook) ? $hook['arguments_return'] : array());
    $error_string = NULL;
    $return = d2d_send_single_outgoing_request($request['type'], $arguments_send, $instances[$request['receiver_id']], $my_instance, $my_private_key, $return_checks, $error_string);
    if ($return !== FALSE) {
      ++$n_sucessfully_sent;
      db_delete('d2d_outgoing_requests')
        ->condition('id', $request_id)
        ->execute();
    }
    if ($return === FALSE && $verbose) {
      drupal_set_message(check_plain($error_string), 'error');
    }
    call_user_func_array(
      $hook['callback_return'],
      array(
        $return,
        $arguments,
        $arguments_send,
        $request['time_insert'],
        $request['time_next_send'],
        $request['time_invalid'],
        $request['max_send_trials'],
        $error_string,
        &$instances[$request['receiver_id']],
        &$persistent_state[$request['type']][$request['receiver_id']],
      )
    );
  }
  return $n_sucessfully_sent;
}