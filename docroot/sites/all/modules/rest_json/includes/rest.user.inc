<?php

/**
 * Does user login.
 *
 * Endpoint: /<rest_prefix>/user/login
 */
function page_rest_json_user_login($username=NULL, $password=NULL) {
	global $rest_conf;
	
	// Check params
	if (!$username) $username = request_param('username', PARAM_REQUIRED);
	if (!$password) $password = request_param('password', PARAM_REQUIRED);
	
	// Allow login by mail
	$username = db_select('users')
		->fields('users', array('name'))
		->condition(db_or()
			->condition('name', $username, '=')
			->condition('mail', $username, '='))
		->execute()
		->fetchField();
	
	// Check auth
	if ($uid = user_authenticate($username, $password)) {
		// Drupal login
		global $user;
		$user = user_load($uid);
		$login_array = array ('name' => $username);
		user_login_finalize($login_array);
		
		$account = rest_json_user($user);
		
		// Let other modules attach more content
		drupal_alter('rest_json_user', $account);
		
		rest_json_response(TRUE, 'user', $account);
	}
	else rest_json_response(FALSE, 'user', NULL, 'Wrong password');
} // page_rest_json_user_login

/**
 * Does user logout.
 *
 * Endpoint: /<rest_prefix>/user/logout
 */
function page_rest_json_user_logout() {
	global $user;
	
	// Check if user is logged
	check_user_auth();
	
	// FIXME: Drupal logout redirects to the home page, we don't want this!
#	module_load_include('pages.inc', 'user');
#	user_logout();
	watchdog('user', 'Session closed for %name.', array('%name' => $user->name));
	module_invoke_all('user_logout', $user);
	session_destroy();

	rest_json_response(TRUE, 'user', NULL);
} // page_rest_json_user_logout

/**
 * Get JSON of single user.
 */
function rest_json_user($user_or_uid) {
	global $rest_conf;
	
	$user = is_numeric($user_or_uid) ? user_load($user_or_uid) : $user_or_uid;
	
	$account = new stdClass;
	
	// Base fields
	foreach ($rest_conf['base_user_fields'] as $field => $name) {
		$value = $user->{$field};
		if (is_numeric($value)) $value = $value + 0;
		
		$account->{$name} = $value;
	}
	
	return $account;
} // rest_json_user

/**
 * Test if user is authenticated.
 *
 * Endpoint: /<rest_prefix>/user/auth
 */
function page_rest_json_user_auth() {
	if ($uid = rest_json_uid()) {
		$account = rest_json_user(user_load($uid));
		
		// Let other modules attach more content
		drupal_alter('rest_json_user', $account);
		
		rest_json_response(TRUE, 'user', $account);
	}
	else {
		rest_json_response(FALSE, 'user', NULL, 'User not authenticated');
	}
} // page_rest_json_user_auth

/**
 * Change or reset user password.
 *
 * Endpoint: /<rest_prefix>/user/password/{change|reset}
 */
function page_rest_json_user_password($action) {
	include_once DRUPAL_ROOT . '/includes/password.inc';
	
	// Change password
	if ($action == 'change') {
		// Check params
		$username = request_param('username', PARAM_REQUIRED);
		$old_password = request_param('old', PARAM_REQUIRED);
		$new_password = request_param('new', PARAM_REQUIRED);
		
		if ($uid = user_authenticate($username, $old_password)) {
			// Change password
			$account = user_load($uid);
			$account->pass = user_hash_password($new_password);
			user_save($account);
			
			// Re-login
			page_rest_json_user_login($username, $new_password);
		}
		else rest_json_response(FALSE, 'user', NULL, 'Wrong old password');
	}
	
	// Reset password
	elseif ($action == 'reset') {
		// Check params
		$username = request_param('username', PARAM_REQUIRED);
		
		// Load user
		$uid = db_select('users')
			->fields('users', array('uid'))
			->condition(db_or()
				->condition('name', $username, '=')
				->condition('mail', $username, '='))
			->execute()
			->fetchField();
			
		if ($uid) {
			// Reset password
			$account = user_load($uid);
			_user_mail_notify('password_reset', $account);
			
			rest_json_response(TRUE, 'user', rest_json_user($account));
		}
		else rest_json_response(FALSE, 'user', NULL, 'Username or mail not found');
	}
} // page_rest_json_user_password

/**
 * Register a new user.
 *
 * Endpoint: /<rest_prefix>/user/register
 */
function page_rest_json_user_register() {
	global $language;
	
	include_once DRUPAL_ROOT . '/includes/password.inc';
	
	// Check params
	$username = request_param('username', PARAM_REQUIRED);
	$mail = request_param('mail', PARAM_REQUIRED);
	$password = request_param('password', PARAM_REQUIRED);
	if (!valid_email_address($mail)) rest_json_response(FALSE, 'parameter', NULL, 'Missing or invalid mail');
	$userlang = request_param('language');
	
	// Check if user exists
	$uid = db_select('users')
		->fields('users', array('uid'))
		->condition(db_or()
			->condition('name', $username, '=')
			->condition('mail', $mail, '='))
		->execute()
		->fetchField();
	if ($uid) rest_json_response(FALSE, 'user', NULL, 'Username or mail already registered');
	
	// Create user
	$user = entity_create('user', array());
	$user->name = $username;
	$user->status = 1;
	$user->roles = array(DRUPAL_AUTHENTICATED_RID);
	$user->init = $user->mail = $mail;
	$user->pass = user_hash_password($password);
	$user->language = $userlang ? $userlang : $language->language;
	
	// Save user
	$userw = entity_metadata_wrapper('user', $user);
	$userw->save();
	
	$account = user_load($userw->value()->uid);
	rest_json_response(TRUE, 'user', rest_json_user($account));
} // page_rest_json_user_register

/**
 * Get uid from user session (cookie or request parameter).
 */
function rest_json_uid($session_id='') {
	if (!$session_id) $session_id = request_param('sid');
	if (!$session_id) $session_id = session_id();
	
	$uid = db_query("SELECT uid from {sessions} WHERE sid = :sid LIMIT 1", 
		array(":sid"=>$session_id))->fetchField();
	
	return $uid;
} // rest_json_uid

