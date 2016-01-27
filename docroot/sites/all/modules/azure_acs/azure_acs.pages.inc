<?php
/**
 * @file
 * Pages for azure_acs
 *
 */

/**
 * Endpoint for login flows through identity providers. Handles Drupal-side login.
 */
function azure_acs_return() {
  if (isset($_POST['wresult'])) {
    // Currently only SWT 1.0 tokens are supported
    $token_type = 'http://schemas.xmlsoap.org/ws/2009/11/swt-token-profile-1.0';

    module_load_include('php', 'azure_acs', 'lib/wsfederation');
    module_load_include('php', 'azure_acs', 'lib/swt');

    $realm = _azure_acs_get_realm();
    $namespace = variable_get('azure_acs_namespace', '');
    $token_signing_key = variable_get('azure_acs_token_signing_key', '');

    $login_error_message = t('Login failed. Please contact the site administrator.');

    if (!$namespace || !$token_signing_key) {
      watchdog('azure_acs', 'Windows Azure ACS namespace and/or token signing key are not set', array(), WATCHDOG_WARNING);
      drupal_set_message($login_error_message, 'error');
      drupal_goto('<front>');
    }

    //Parse token and extract information ("claims") about the user
    try {
      $handler = new TokenReponseHandler();
      $token = $handler->HandleResponse($_POST['wresult'], $realm, $token_type);
      $validator = new TokenValidator();
      $validator->Validate($token, $realm, $namespace, $token_signing_key, '');
      $claims = $validator->GetClaims($token);
    }
    catch (Exception $e) {
      watchdog('azure_acs', 'Error while validating tokens. Error message: @error', array('@error' => $e->getMessage()), WATCHDOG_ERROR);
      drupal_set_message($login_error_message, 'error');
      drupal_goto('<front>');
    }

    //Prettify the claims' keys a bit
    $user_data = array();
    foreach ($claims as $claim_key => $content) {
      $claim = explode('/', $claim_key);
      $key = array_pop($claim);
      $user_data[$key] = $content;
    }

    if (empty($user_data['identityprovider']) || empty($user_data['nameidentifier'])) {
      watchdog('azure_acs', 'Identity provider or name identifier missing from return data: @data', array('@data' => print_r($user_data, TRUE)), WATCHDOG_ERROR);
      drupal_set_message($login_error_message, 'error');
      drupal_goto('<front>');
    }

    //Prettify identity provider string a bit (for example "Facebook-192452900881513" -> "Facebook")
    $ip_parts = explode('-', $user_data['identityprovider']);
    $user_data['identityprovider'] = $ip_parts[0];

    $mail = isset($user_data['emailaddress']) ? $user_data['emailaddress'] : NULL;

    if ($uid = _azure_acs_match_user($user_data)) {
      // We've seen this guy before, log 'em in
      _azure_acs_log_user_in($uid);
    }
    else {
      //Unregognized user, either create automatically or take the user to registration
      //But first, let's check that a user by the same email address doesn't already exist
      if ($mail && user_load_multiple(array(), array('mail' => $mail))) {
        drupal_set_message(t('Email address %mail is already in use.', array('%mail' => $mail)), 'error');
        drupal_goto('<front>');
      }

      if (variable_get('azure_acs_create_users', TRUE)) {
        // We're allowed to create accounts on the fly
        // Select an unique username
        $name = $user_data['name'];
        $i = 0;
        while (user_load_by_name($name)) {
          $name = $user_data['name'] . '_' . $i;
          $i++;
        }

        $edit = array(
          'name' => $name,
          'mail' => $mail,
          'pass' => user_password(),
          'azure_acs_data' => $user_data,
          'status' => 1,
        );

        $account = user_save(NULL, $edit);

        _azure_acs_log_user_in($account->uid);
      }
      else {
        // We need to take the user to the registration form.
        drupal_set_message(t('You need to create a local user account to proceed.'));
        if (isset($_GET['destination'])) {
          unset($_GET['destination']);
        }

        // Store user data so that we can populate it in the register form
        $_SESSION['azure_acs_data'] = $user_data;
        drupal_goto('user/register');
      }
    }
  }
  else {
    drupal_goto('<front>');
  }

  // There's nothing to see on this page.
  drupal_goto('<front>');
}

/**
 * Endpoint for login flows through identity providers when a login error occurred.
 */
function azure_acs_error() {
  if (isset($_POST['ErrorDetails'])) {
    $errors = json_decode($_POST['ErrorDetails'], TRUE);

    //Log the errors and show the last error on the stack to the user
    watchdog('azure_acs', 'Login flow ended in error: @data', array('@data' => print_r($errors, TRUE)), WATCHDOG_NOTICE);
    if (!empty($errors['errors'])) {
      $user_error = array_pop($errors['errors']);
      drupal_set_message($user_error['errorMessage'], 'error');
    }
    else {
      drupal_set_message(t('Login failed'), 'error');
    }
  }

  // There's nothing to see on this page.
  drupal_goto('<front>');
}
