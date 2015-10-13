<?php

/**
 * @file
 * Code related to send pattern to server / receive pattern as server.
 */

module_load_include('module', 'd2d');

/**
 * Provides the full credentials.
 */
function patterns_client_get_full_credentials($server, $user = NULL) {
  $conf = variable_get(PATTERNS_CLIENT_CREDENTIALS, FALSE);
  if (!$conf || empty($conf[$server])) return FALSE;
  if (!is_null($user) && $conf[$server]['user'] != $user) return FALSE;
  return $conf[$server];
}

/**
 * Return user credentials.
 */
function patterns_client_get_credentials($server, $user = NULL) {
  $conf = patterns_client_get_full_credentials($server, $user);
  if (!$conf) return FALSE;
  return array(
    'user' => $conf['user'],
    'pwd' => $conf['token'],
  );
}

/**
 * Updates the full credentials.
 */
function patterns_client_update_credentials($server, $user, $token, $email) {
  $current = variable_get(PATTERNS_CLIENT_CREDENTIALS, array());
  $current[$server] = array(
    'user' => $user,
    'token' => $token,
    'email' => $email,
  );
  variable_set(PATTERNS_CLIENT_CREDENTIALS, $current);
}

/**
 * Checks if the user is already registerd in the server.
 */
function patterns_client_is_user_registered_on_server($server, $user = NULL) {
  if (empty($server)) return FALSE;
  $credentials = patterns_client_get_credentials($server, $user);
  return $credentials ? TRUE : FALSE;
}

/**
 * Registers the user in the server.
 */
function patterns_client_register_user_on_server($url, $user, $pwd, $email) {
  $friend = d2d_api_friend_get_by_url($url);
  $post = array(
    'user' => $user,
    'pwd' => $pwd,
    'email' => $email,
  );
  // Send the request
  $error_string = '';
  $token = d2d_call_secure_rpc($friend, 'patterns_server_register_client', $post, $error_string);
  // Unknown Error
  if (is_null($token)) {
    drupal_set_message(check_plain($url) . ' - ' . t('Unknown error'), 'error');
  }
  // Communication Error
  elseif ($token === FALSE) {
    drupal_set_message(check_plain($url) . ' - ' . check_plain($error_string), 'error');
  }
  // Error while saving the user (e.g., already existing)
  elseif (_patterns_client_is_registration_error($token)) {
    drupal_set_message(check_plain($url) . ' - ' . t('%err', array('%err' => $token)), 'error');
  }
  // Save the results
  else {
    drupal_set_message(check_plain(t('Registration executed successfully.')) . ' ' . check_plain($url));
    patterns_client_update_credentials($url, $user, check_plain($token), $email);
    drupal_set_message(t('Credentials stored in local database'));
  }
}

/**
 * Links the user account in the server.
 */
function patterns_client_link_user_on_server($url, $user, $pwd) {
  $friend = d2d_api_friend_get_by_url($url);
  $post = array(
    'user' => $user,
    'pwd' => $pwd,
  );
  if ($post === FALSE) {
    drupal_set_message(t('An error occurred while compacting your account data.'), 'error');
    return FALSE;
  }
  // Send the request
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'patterns_server_link_client', $post, $error_string);
  // Unknown Error
  if (is_null($result)) {
    drupal_set_message(check_plain($url) . ' - ' . t('Unknown error'), 'error');
  }
  // Communication Error
  elseif ($result === FALSE) {
    drupal_set_message(check_plain($url) . ' -A ' . check_plain($error_string), 'error');
  }
  // Trying to parse returned string
  elseif (_patterns_client_is_linking_error($result)) {
    drupal_set_message(check_plain($url) . ' - ' . t('%err', array(
      '%err' => $result,
    )), 'error');
  }
  else {
    $values = d2d_explode($result);

    if ($values === FALSE) {
      drupal_set_message(check_plain($url) . ' - ' . t('Could not parse return value. Control server/client versions.'), 'error');
    }
    else {
      $check = array(
        'mail' => 'is_string',
        'pass' => 'is_string',
      );
      if (!d2d_check_array($values, $check)) {
        drupal_set_message(check_plain($url) . ' - ' . t('Could not extract the data from the encrypted message.'), 'error');
      }

      // Save the results
      else {
        $email = $values['mail'];
        $token = $values['pass'];
        drupal_set_message(check_plain(t('Existing username on server succesfully linked.')) . ' ' . check_plain($url));
        patterns_client_update_credentials($url, $user, check_plain($token), check_plain($email));
        drupal_set_message(t('Credentials stored in local database'));
      }
    }
  }
}

/**
 * Returns the registration error.
 */
function _patterns_client_is_registration_error($str) {
  $needle = 'Registration-Error.';
  return !strncmp($str, $needle, strlen($needle));
}

/**
 * Returns the linking error.
 */
function _patterns_client_is_linking_error($str) {
  $needle = 'LinkingUser-Error.';
  return !strncmp($str, $needle, strlen($needle));
}

/**
 * Push patterns to the server.
 */
function patterns_client_push_patterns($patterns) {
  $url = variable_get('patterns_client_server');
  if (!patterns_client_is_user_registered_on_server($url)) {
    drupal_set_message(t('No registered Patterns server found.'), 'error');
    return FALSE;
  }
  $credentials = patterns_client_get_credentials($url);
  $imploded_credentials = d2d_implode($credentials);
  if ($imploded_credentials === FALSE) {
    drupal_set_message(t('An error occurred while parsing your account data.'), 'error');
    return FALSE;
  }
  $upload_patterns = array();
  foreach ($patterns as $pattern) {
    $name = $pattern->name;
    $format = $pattern->format;
    $content = file_get_contents($pattern->file);
    if ($content === FALSE) {
      drupal_set_message(check_plain(t('Pattern')) . ' ' . check_plain($name) . ' ' . check_plain(t('could not be processed')), 'error');
      continue;
    }
    $imploded_pattern = d2d_implode(
      array(
        'name' => $name,
        'format' => $format,
        'content' => $content,
      )
    );
    if ($imploded_pattern == FALSE) {
      drupal_set_message(check_plain(t('An error occurred while processing pattern')) . ' ' . check_plain($name) . ' ' . check_plain(t('Skipped.')), 'error');
      continue;
    }
    $upload_patterns[] = $imploded_pattern;
  }
  $imploded_patterns = d2d_implode($upload_patterns);
  if ($imploded_patterns === FALSE) {
    drupal_set_message(t('Internal error while compacting the patterns to be sent.'), 'error');
    return FALSE;
  }
  $post = array(
    'credentials' => $imploded_credentials,
    'patterns' => $imploded_patterns,
  );
  $error_string = '';
  $friend = d2d_api_friend_get_by_url($url);
  $res = d2d_call_secure_rpc($friend, 'patterns_server_publish_patterns', $post, $error_string);
  if ($res === FALSE) {
    drupal_set_message(check_plain($url) . ' - ' . check_plain($error_string), 'error');
  }
  else{
    drupal_set_message(check_plain(t('Upload to server successful.')) . ' ' . check_plain($url) . ' - ' . check_plain($res));
  }
}

/**
 * Performs a remote call to the Patterns Server using D2D in order
 * to retrieve the list of all the Patterns stored in the server sorted by its
 * timestamp descendingly.
 * @param int $limit
 * @return array List of patterns currently stored in the server
 */
function _patterns_client_get_patterns_server_list($limit) {

  $info = patterns_client_check_credentials_and_display_warns();

  if ($info['ready'] === FALSE) {
    return;
  }

  $url = $info['url'];
  $credentials = $info['credentials'];

  $friend = d2d_api_friend_get_by_url($url);


// Prepare arguments for the call. In this case just the maximum number of patterns expected
  $post = array(
    'limit' => $limit,
  );


  // Send the request. We explode the result in two different steps
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'patterns_server_get_patterns_list', $post, $error_string);
  $result = d2d_explode($result);

  // Display communication errors if any
  if (!$result) {
    drupal_set_message(t('No response from the server. Are you friends with it?'), 'warning');
  }
  elseif (!is_null($result['error'])) {
    drupal_set_message(t('The Patterns server returned this response: @response', array('@response' => $result['error'])), 'error');
  }
  else{
    //If everything is OK, return the results completely exploded
    $patterns_list = d2d_explode($result['return']);
    $exploded_result = array();
    foreach ($patterns_list as $p) {
      $exploded_result[] = d2d_explode($p);
    }
    return $exploded_result;
  }
}

/**
 * Performs a remote call to the Patterns Server using D2D in order
 * to retrieve the pattern file and its type, and returns a Pattern object.
 * @param int $limit
 * @return array List of patterns currently stored in the server
 */
function _patterns_client_fetch_server_pattern($pid)  {
  global $user;
  // Get the current server.
  $url = variable_get('patterns_client_server', PATTERNS_CLIENT_OFFICIAL_SERVER);
  $friend = d2d_api_friend_get_by_url($url);
  // Prepare arguments for the call. In this case just the maximum number
  // of patterns expected.
  $post = array(
    'pid' => $pid,
  );
  // Send the request. We explode the result in two different steps.
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'patterns_server_fetch_pattern', $post, $error_string);
  $result = d2d_explode($result);
  // Display communication errors if any.
  if (!$result) {
    // Unknown error.
    drupal_set_message(check_plain($url) . ' - ' . t('Unknown error. Please make sure that the server has provided you permisssions to perform this operation.'), 'error');
  }
  elseif (!is_null($result['error'])) {
    drupal_set_message(t('The Patterns server returned this response: @response', array('@response' => $result['error'])), 'error');
    return $result['error'];
  }
  else {
    // Exploded the value and parse it to work with an object.
    $pattern_info = d2d_explode($result['return']);
    $pattern = patterns_parser_parse($pattern_info['file'], $pattern_info['format']);
    //debug(print_r($pattern, TRUE));
    // Add parent header including all the pulled pattern as parent
    $parents = array();
    $parents[] = array(
      'action' => 'pull',
      'host' => $url,
      'd2d_id' => $friend['d2d_id'],
      'uuuid' => $pattern_info['uuuid'],
      'title' => @$pattern['info']['title'],
      'category' => @$pattern['info']['category'],
      'author' => @$pattern['info']['author'],
    );
    patterns_history_set_parent($pattern, $parents);

    return _patterns_io_save_pattern($pattern, 'clone_' . $pattern_info['uuuid'], $pattern_info['format'], NULL, NULL, $user->name, $pattern_info['uuuid']);
  }
}

/**
 * Performs a remote call to the Patterns Server using D2D in order
 * to retrieve a set of patterns that matches the given criteria.
 * @param string $token  Token to perform the search.
 * @param string $type  Field in which the search will be performed.
 * @return array  List of strings matching the giving criteria.
 */
function _patterns_client_search_server_pattern($token, $type)  {
  // Get the current server
  $url = variable_get('patterns_client_server', PATTERNS_CLIENT_OFFICIAL_SERVER);
  $friend = d2d_api_friend_get_by_url($url);
  // Prepare arguments for the call. In this case just the maximum number of patterns expected
  $post = array(
    'token' => $token,
    'type' => $type,
  );
  // Send the request. We explode the result in two different steps
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'patterns_server_search', $post, $error_string);
  $result = d2d_explode($result);
  // Display communication errors if any
  if (!$result) {
    // Unknown error
    drupal_set_message(check_plain($url) . ' - ' . t('Unknown error. Please ensure your Peer instance has provided you permisssions to perform this operation.'), 'error');
  }
  elseif (!is_null($result['error'])) {
    drupal_set_message(t('The Patterns server returned this response: @response', array('@response' => $result['error'])), 'error');
    return $result['error'];
  }
  else{
    //If everything is OK, return the results completely exploded
    $patterns_list = d2d_explode($result['return']);
    $exploded_result = array();
    foreach ($patterns_list as $p) {
      $exploded_result[] = d2d_explode($p);
    }
    return $exploded_result;
  }
}

/**
 * Return the number of Patterns in the client already stored with the current UUUID.
 * Although the UUUID is generated randomly and is unique, there could be cases in which
 * it could be manipulated manually and the DB currently does not hold any UNIQUE constraint
 * for this parameter, therefore we return the total number instead of a boolean value for the moment.
 * @param string $uuuid  ID to be checked
 * @return Number of patterns with the given UUUID
 */
function _patterns_client_row_count_uuuid($uuuid) {
  $return = db_query("SELECT p.uuuid FROM {patterns} p WHERE p.uuuid = :uuuid", array(':uuuid' => $uuuid));
  return $return->rowCount();
}
