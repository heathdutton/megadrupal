<?php

/**
 * @file
 * Private helper functions
 *
 * @author Mathew Winstone <mwinstone@coldfrontlabs.ca>
 * @copyright 2013 Coldfront Labs Inc.
 * @license Copyright (c) 2013 All rights reserved
 */

/**
 * Users API callback
 *
 * @param string $method
 *  Method type. Valid values are 'index', 'read', 'create', 'update' and 'delete'.
 * @param string $id
 *  User's gitlab id
 * @param string $user_data [optional]
 *  User data to be updated
 *  Only required when creating or updating the user
 * @param string $private_token
 *  The user's private token for connecting to the api endpoint
 * @param array $options [optional]
 *  List of extra options
 * @return array
 *  Returns the gitlab user object, FALSE otherwise.
 */
function _gitlab_users_users($method, $id = NULL, $user_data = NULL, $private_token, $options) {
  if (('create' == $method or 'update' == $method) and !_gitlab_users_validate_user_data($method, $user_data)) {
    return FALSE;
  }

  // ID is required for all but create and index calls
  if (is_null($id) and 'create' != $method and 'index' != $method) {
    return FALSE;
  }

  // Load the wsconfig
  $wsconfig = wsconfig_load_by_name('gitlab_users');

  // Apply options
  gitlab_set_wsconfig_options($wsconfig, $options);

  $processor = new gitlabWsdataProcessor();

  // Check for a valid connector
  if ($wsconfig->connector) {
    $data = $wsconfig->call(
      $method,
      array('%id' => $id),
      drupal_json_encode($user_data),
      gitlab_set_wsconfig_call_options($private_token, $options)
    );
    if (FALSE !== $data) {
      return _gitlab_users_process_result($data, $processor, 'gitlab_users_current');
    }
    else {
      watchdog('gitlab_users_current', 'Users @method failed', array('@method' => $method), WATCHDOG_ERROR);
    }
  }
}

/**
 * Current User API callback
 *
 * @param string $method
 *  Method type. Valid values are 'read' and 'update'.
 * @param string $user_data [optional]
 *  User data to be updated
 *  Only required when making an update to the user
 * @param string $private_token
 *  The user's private token for connecting to the api endpoint
 * @param array $options [optional]
 *  List of extra options
 * @return array
 *  Returns the gitlab user object, FALSE otherwise.
 */
function _gitlab_users_user_current($method, $user_data = NULL, $private_token, $options) {
  if ('update' == $method and !_gitlab_users_validate_user_data($method, $user_data)) {
    return FALSE;
  }

  // Load the wsconfig
  $wsconfig = wsconfig_load_by_name('gitlab_users_current');

  // Apply options
  gitlab_set_wsconfig_options($wsconfig, $options);

  $processor = new gitlabWsdataProcessor();

  // Check for a valid connector
  if ($wsconfig->connector) {
    $data = $wsconfig->call(
      $method,
      array(),
      drupal_json_encode($user_data),
      gitlab_set_wsconfig_call_options($private_token, $options)
    );
    if (FALSE !== $data) {
      return _gitlab_users_process_result($data, $processor, 'gitlab_users');
    }
    else {
      watchdog('gitlab_users_current', 'Current user @method failed', array('@method' => $method), WATCHDOG_ERROR);
    }
  }
}

/**
 * Current User SSH Key API callback
 *
 * @param string $method
 *  Method type. Valid values are 'index', 'create', 'read' and 'delete'.
 * @param string $key_data [optional]
 *  User data to be updated
 *  Only required when creating a key
 * @param string $private_token
 *  The user's private token for connecting to the api endpoint
 * @param array $options [optional]
 *  List of extra options
 * @return array
 *  Returns the gitlab user object, FALSE otherwise.
 * @see gitlab_users_create_current_sshkey().
 */
function _gitlab_users_user_current_sshkey($method, $id = NULL, $key_data = NULL, $private_token, $options) {
  if ('create' == $method and !_gitlab_users_validate_sshkey_data($key_data)) {
    return FALSE;
  }

  // Load the wsconfig
  $wsconfig = wsconfig_load_by_name('gitlab_users_current_sshkey');

  // Apply options
  gitlab_set_wsconfig_options($wsconfig, $options);

  $processor = new gitlabWsdataProcessor();

  // Check for a valid connector
  if ($wsconfig->connector) {
    $data = $wsconfig->call(
      $method,
      array('%id' => $id),
      drupal_json_encode($key_data),
      gitlab_set_wsconfig_call_options($private_token, $options)
    );
    if (FALSE !== $data) {
      return _gitlab_users_process_result($data, $processor, 'gitlab_users');
    }
    else {
      watchdog('gitlab_users_current_sshkey', 'Current user SSH key @method failed', array('@method' => $method), WATCHDOG_ERROR);
    }
  }
}

/**
 * User SSH Key API callback
 *
 * @param string $method
 *  Method type. Valid values is 'create'.
 * @param string $key_data [optional]
 *  User data to be updated
 *  Only required when creating a key
 * @param string $private_token
 *  The user's private token for connecting to the api endpoint
 * @param array $options [optional]
 *  List of extra options
 * @return array
 *  Returns the gitlab user object, FALSE otherwise.
 * @see gitlab_users_create_current_sshkey().
 */
function _gitlab_users_users_sshkey($method, $id = NULL, $key_data = NULL, $private_token, $options) {
  if ('create' == $method and !_gitlab_users_validate_sshkey_data($key_data)) {
    return FALSE;
  }

  // Load the wsconfig
  $wsconfig = wsconfig_load_by_name('gitlab_users_sshkey');

  // Apply options
  gitlab_set_wsconfig_options($wsconfig, $options);

  $processor = new gitlabWsdataProcessor();

  // Check for a valid connector
  if ($wsconfig->connector) {
    $data = $wsconfig->call(
      $method,
      array('%id' => $id),
      drupal_json_encode($key_data),
      gitlab_set_wsconfig_call_options($private_token, $options)
    );
    if (FALSE !== $data) {
      return _gitlab_users_process_result($data, $processor, 'gitlab_users');
    }
    else {
      watchdog('gitlab_users_current_sshkey', 'User SSH key @method failed', array('@method' => $method), WATCHDOG_ERROR);
    }
  }
}

/**
 * Validate the user data.
 *
 * @param string $method
 *  Operation being performed. Valid values are 'create' and 'update'.
 * @param array $user_data
 *  User data to check
 * @return boolean
 *  Returns TRUE if data is valid, FALSE otherwise.
 * @see gitlab_users_create_user().
 */
function _gitlab_users_validate_user_data($method = 'create', $user_data) {

  if ('read' != $method and 'update' != $method) {
    // Invalid method
    return FALSE;
  }

  // Check all required values for creating a user
  if ($method == 'create') {
    if (empty($user_data['email'])
      or empty($user_data['username'])
      or empty($user_data['name'])
      or empty($user_data['password'])
    ) {
      watchdog('gitlab_users', 'Missing required field during user create', array(), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // Validate the email
  if (!empty($user_data['email']) and !filter_var($user_data['email'], FILTER_VALIDATE_EMAIL)) {
    watchdog('gitlab_users', 'Invalid email value passed during @method', array('@string' => $string, '@method' => $method), WATCHDOG_ERROR);
    return FALSE;
  }

  // Validate the username
  if (!empty($user_data['username']) and !ctype_alnum($user_data['username'])) {
      watchdog('gitlab_users', 'Non-alphanumeric string passed for username during @method', array('@method' => $method), WATCHDOG_ERROR);
    return FALSE;
  }

  // Validate the set of strings who's values cannot be empty
  $strings = array('name', 'password');
  foreach ($strings as $string) {
    if (isset($user_data[$string]) and empty($user_data[$string])) {
      watchdog('gitlab_users', 'Empty string value passed for @string during @method', array('@string' => $string, '@method' => $method), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // Validate URL
  if (!empty($user_data['website_url']) and !filter_var($user_data['website_url'], FILTER_VALIDATE_URL)) {
    watchdog('gitlab_users', 'Invalid URL value passed for website_url during @method', array('@method' => $method), WATCHDOG_ERROR);
    return FALSE;
  }

  // Validate boolean flags
  $bools = array('admin', 'can_create_group');
  foreach ($bools as $bool) {
    if (isset($user_data[$bool]) and !is_bool($user_data[$bool])) {
      watchdog('gitlab_users', 'Non-boolean value passed to @string during @method', array('@string' => $bool, '@method' => $method), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // All else fails, it's valid.
  return TRUE;
}

/**
 * Validate the SSH key data
 * @param array $key_data
 *  User data to check
 * @return boolean
 *  Returns TRUE if data is valid, FALSE otherwise.
 */
function _gitlab_users_validate_sshkey_data($key_data) {
  if (empty($key_data['title']) or empty($key_data['key'])) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Process request result
 */
function _gitlab_users_process_result($data, $processor, $wsconfig_name = 'gitlab_users') {
  $ret = array();
  $processor->addData($data);
  $error = $processor->getError();
  if ($error) {
    $error_keys = array_keys($error);
    watchdog($wsconfig_name, gitlab_request_error(array_pop($error_keys)), array(), WATCHDOG_NOTICE);
    $ret['error'] = $error;
  } else {
    $data = $processor->getData();
    if (!empty($data)) {
      $ret['data'] = $data;
    }
  }
  return $ret;
}