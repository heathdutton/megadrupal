<?php
/**
 * @file
 * Code related to send pattern to server / receive pattern as server.
 */
module_load_include('inc', 'patterns_server', 'includes/auth');
module_load_include('inc', 'patterns', 'includes/io/io');

/**
 * Implements hook_d2d_secure_rpc().
 */
function patterns_server_d2d_secure_rpc() {
  $methods = array();
  $methods['patterns_server_publish_patterns'] = array(
    'arguments' => array('credentials' => 'is_string', 'patterns' => 'is_string'),
    'callback' => 'patterns_server_incoming_patterns',
    'description' => 'save incoming patterns to the database',
  );
  $methods['patterns_server_register_client'] = array(
    'arguments' => array('user' => 'is_string', 'pwd' => 'is_string', 'email' => 'is_string'),
    'callback' => 'patterns_server_register_client',
    'description' => 'creates programmatically a new user with patterns_server access privileges',
  );
  $methods['patterns_server_link_client'] = array(
    'arguments' => array('user' => 'is_string', 'pwd' => 'is_string'),
    'callback' => 'patterns_server_link_client',
    'description' => 'confirms that a client is the authorized owner of an existing account',
  );
  $methods['patterns_server_get_patterns_list'] = array(
    'arguments' => array('limit' => 'is_string'),
    'callback' => 'patterns_server_get_patterns_list',
    'description' => 'Returns a list of the patterns currently hosted in this server',
  );
  $methods['patterns_server_fetch_pattern'] = array(
      'arguments' => array('pid' => 'is_string'),
      'callback' => 'patterns_server_fetch_pattern',
      'description' => 'Returns a imploded version of the whole pattern',
  );
  $methods['patterns_server_search'] = array(
      'arguments' => array('token' => 'is_string', 'type' => 'is_string', ),
      'callback' => 'patterns_server_search',
      'description' => 'Returns a list of patterns according to the given criteria',
  );
  return $methods;
}

/**
 * On error, returns a string starting with 'Registration-Error.'
 * On success, returns the hashed password.
 */
function patterns_server_register_client($arguments, $rpc_info) {
  $err_str = 'Registration-Error. ';
  if (empty($arguments)) {
    return $err_str . 'Empty request received.';
  }
  $check = array(
    'user' => 'is_string',
    'pwd' => 'is_string',
    'email' => 'is_string',
  );
  if (!d2d_check_array($arguments, $check)) {
    return $err_str . 'Could not extract the credentials from the encrypted message.';
  }
  $user = $arguments['user'];
  $pwd = $arguments['pwd'];
  $email = $arguments['email'];
  if (db_query("SELECT COUNT(*) FROM {users} WHERE name = :name", array(':name' => $user))->fetchField()) {
    return $err_str . 'Username already existing.';
  }
  $account = _patterns_server_register_client($user, $pwd, $email);
  if (!$account) {
    return $err_str . ' Registration did not succeed. Please try again later.';
  }
  else {
    return $account->pass;
  }
}

/**
 * On error, returns a string starting with 'LinkingUser-Error.'
 * On success, returns an imploded array containing the email, and the token
 */
function patterns_server_link_client($arguments, $rpc_info) {
  $err_str = 'LinkingUser-Error. ';
  if (empty($arguments)) {
    return $err_str . 'Empty request received.';
  }
  $check = array(
    'user' => 'is_string',
    'pwd' => 'is_string',
  );
  if (!d2d_check_array($arguments, $check)) {
    return $err_str . 'Could not extract the credentials from the encrypted message.';
  }
  $user = $arguments['user'];
  $pwd = $arguments['pwd'];
  $result = db_query("SELECT email, pass FROM {users} WHERE name = :name AND pass = :pass", array(':name' => $user))->fetchAssoc();
  if (empty($result)) {
    return $err_str . 'Username or password incorrect.';
  }
  $result = d2d_implode($result);
  if ($result === FALSE) {
    return $err_str . 'Server error while compacting the results.';
  }
  else {
    return $result;
  }
}

/**
 * Registers client in the server.
 */
function _patterns_server_register_client($user, $pwd, $email) {
  $role = user_role_load_by_name('patterns_server_user');
  $role_rid = $role->rid;
  // Determine the roles of our new user
  $new_user_roles = array(
    DRUPAL_AUTHENTICATED_RID => 'authenticated user',
    $role_rid => TRUE,
  );
  $new_user = array(
    'name' => $user,
    'pass' => $pwd,
    'mail' => $email,
    'status' => 1,
    'access' => REQUEST_TIME,
    'roles' => $new_user_roles,
  );
  return user_save(null, $new_user);
}

/**
 * Stores the received pattern.
 */
function patterns_server_save_pattern($pattern_name, $pattern, $format, $user) {
  $result = _patterns_io_save_pattern($pattern, $pattern_name, $format, NULL, NULL, $user);
  if (!empty($result['msg'])) {
    $status = ($result['status'] == PATTERNS_SUCCESS) ? WATCHDOG_INFO : WATCHDOG_ERROR;
    watchdog('patterns_server', $result['msg'], array(), $status);
  }
  return $result;
}

/**
 * Processes an incoming request.
 */
function patterns_server_incoming_patterns($arguments, $rpc_info) {
  if (empty($arguments)) {
    return 'Error. Empty request received.';
  }
  $imploded_credentials = $arguments['credentials'];
  $exploded_credentials = d2d_explode($imploded_credentials);
  if ($exploded_credentials === FALSE) {
    return 'Error. Received upload pattern/s request with no credentials.';
  }
  $check = array(
    'user' => 'is_string',
    'pwd' => 'is_string',
  );
  if (!d2d_check_array($exploded_credentials, $check)) {
    return 'Error. Could not extract the credentials from the encrypted message.';
  }
  $user = $exploded_credentials['user'];
  $pwd = $exploded_credentials['pwd'];
  $result = _patterns_server_login_authenticate($user, $pwd);
  if ($result['status'] != PATTERNS_SUCCESS) {
    global $base_url;
    patterns_server_log_failed_login($user, $pwd);
    return 'Error. Authentication failed. Please check your username and password on server ' . $base_url;
  }
  $imploded_patterns = $arguments['patterns'];
  $exploded_patterns = d2d_explode($imploded_patterns);
  if ($exploded_patterns === FALSE) {
    return 'Error. Could not extract the pattern files from the encrypted message.';
  }
  $errors = array();
  $saved = array();
  foreach ($exploded_patterns as $imploded_pattern) {
    $exploded_pattern = d2d_explode($imploded_pattern);
    if ($exploded_pattern !== FALSE) {
      $check = array(
        'name' => 'is_string',
        'format' => 'is_string',
        'content' => 'is_string',
      );
      if (d2d_check_array($exploded_pattern, $check)) {
        $name = $exploded_pattern['name'];
        $pattern = $exploded_pattern['content'];
        $format = $exploded_pattern['format'];
        $result = patterns_server_save_pattern($name, $pattern, $format, $user);
        if ($result['status'] == PATTERNS_SUCCESS) {
          patterns_db_publish_pattern($name);
          $saved[] = $name;
        }
        else {
          $errors[] = $name . ' -> ' . $result['msg'];
        }
      }
    }
  }
  $outcome = count($saved) . ' pattern(s) received and saved.';
  if (count($errors) > 0) {
    $outcome .= ' The following patterns could not be saved: ' . implode(',', $errors);
  }
  return $outcome;
}

/**
 * Provides the list of patterns currently stored in this server.
 * It makes use of patterns_server_get_pattern_timeDESC_from_db()
 * to perform the operation
 * @param array $arguments Set of arguments as defined by D2D
 * @param array $rpc_info  Metadata about the RPC call as defined by D2D
 * @return string Imploded string with the error and another array containing
 * the list of patterns.
 */
function patterns_server_get_patterns_list($arguments, $rpc_info) {
  $error_string = NULL;
  $patterns_list = array();
  // We will perform the error checking first and implode the error string as well if any
  if (!d2d_check_convert_int($arguments['limit'])) {
    $error_string = 'Expected an integer value as limit';
  }
  elseif ($arguments['limit']<=0) {
    $error_string = 'Limit should be a positive number';
  }
  else{
    // Perform the casting manually, applying already d2d_implode
    $pattern_data = array();
    foreach (patterns_server_get_pattern_timeDESC_from_db($arguments['limit']) as $pattern) {
      $pattern_data['pid'] = $pattern->pid;
      $pattern_data['uuuid'] = $pattern->uuuid;
      $pattern_data['format'] = $pattern->format;
      $pattern_data['title'] = $pattern->title;
      $pattern_data['description'] = $pattern->description;
      $pattern_data['author'] = $pattern->author;
      //We will provide the full download url ready to be downloaded. Check if clean urls are enabled
      if ($GLOBALS['conf']['clean_url']) {
        $pattern_data['view_url'] = $GLOBALS['base_url'] . '/' . variable_get('patterns_public_url', '') . '/info/' . $pattern->pid;
      }
      else{
        $pattern_data['view_url'] = $GLOBALS['base_url'] . '/?q=' . variable_get('patterns_public_url', '') . '/info/' . $pattern->pid;
      }
      $patterns_list[] = d2d_implode($pattern_data);
    }
    // Implode the resulting array
    $patterns_list = d2d_implode($patterns_list);
  }
  return d2d_implode(array('return' => $patterns_list, 'error' => $error_string));
}

/**
 * Retrieves a pattern file and implode it using D2D functions.
 * @param array $arguments Set of arguments as defined by D2D
 * @param array $rpc_info  Metadata about the RPC call as defined by D2D
 * @return string Imploded string with the error and the pattern.
 */
function patterns_server_fetch_pattern($arguments, $rpc_info) {
  $error_string = NULL;
  $return = array();
  // We will perform the error checking first and implode the error string as well if any
  if (!d2d_check_convert_int($arguments['pid'])) {
    $error_string = 'Expected an integer value as PID';
  }
  elseif ($arguments['pid']<=0) {
    $error_string = 'PID should be a positive number';
  }
  else{
    // Retrieve the pattern using the function developed for the server itself
    $query = patterns_server_get_pattern_pid_from_db($arguments['pid']);
    if (sizeof($query) == 1) {
      // Obtain the pattern file to be imploded as a string
      $pattern = file_get_contents($query[0]->file);
      // If everyting is correct, increase the counter
      patterns_server_download_recode($arguments['pid']);
      // Prepare the response
      $return['file'] = $pattern;
      $return['format'] = $query[0]->format;
      $return['uuuid'] = $query[0]->uuuid;
    }
    else{
      $error_string = 'Invalid PID';
    }
  }
  return d2d_implode(array('return' => d2d_implode($return), 'error' => $error_string));
}

/**
 * Retrieves a set of patterns that matches the given criteria. It makes
 * use of the DB function patterns_server_search_pattern().
 * @param array $arguments Set of arguments as defined by D2D
 * @param array $rpc_info  Metadata about the RPC call as defined by D2D
 * @return string Imploded string with the error and the list of Patterns.
 */
function patterns_server_search($arguments, $rpc_info) {
  $error_string = NULL;
  $patterns_list = array();
  // We will perform the error checking first and implode the error string as well if any
  if (!d2d_check_convert_int($arguments['type'])) {
    $error_string = 'Expected an integer value as type';
  }
  elseif ($arguments['type']<0 || $arguments['type']>4) {
    $error_string = 'Type should be an integer value in the range 0-4';
  }
  else{
    // Retrieve the patterns using the function developed for the server itself.
    $query = patterns_server_search_pattern($arguments['token'], $arguments['type']);
    if (sizeof($query) >= 0) {
      // Perform the casting manually. No results will return an empty array
      $pattern_data = array();
      foreach ($query as $pattern) {
        $pattern_data['pid'] = $pattern->pid;
        $pattern_data['uuuid'] = $pattern->uuuid;
        $pattern_data['format'] = $pattern->format;
        $pattern_data['title'] = $pattern->title;
        $pattern_data['description'] = $pattern->description;
        $pattern_data['author'] = $pattern->author;
        //We will provide the full download url ready to be downloaded. Check if clean urls are enabled
        if ($GLOBALS['conf']['clean_url']) {
          $pattern_data['view_url'] = $GLOBALS['base_url'] . '/' . variable_get('patterns_public_url', '') . '/info/' . $pattern->pid;
        }
        else{
          $pattern_data['view_url'] = $GLOBALS['base_url'] . '/?q=' . variable_get('patterns_public_url', '') . '/info/' . $pattern->pid;
        }
        $patterns_list[] = d2d_implode($pattern_data);
      }
      // Implode the resulting array
      $patterns_list = d2d_implode($patterns_list);
    }
  }
  return d2d_implode(array('return' => $patterns_list, 'error' => $error_string));
}