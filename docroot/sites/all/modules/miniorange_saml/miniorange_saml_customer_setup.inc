<?php
/**
 * @file
 * Contains form for customer setup.
 */

/**
 * Customer setup form().
 */
function miniorange_saml_customer_setup($form, &$form_state) {

  $current_status = variable_get('miniorange_saml_status', '');
  if ($current_status == 'VALIDATE_OTP') {
    $form['miniorange_saml_customer_otp_token'] = array(
      '#type' => 'textfield',
      '#title' => t('OTP'),
    );

    $form['miniorange_saml_customer_validate_otp_button'] = array(
      '#type' => 'submit',
      '#value' => t('Validate OTP'),
      '#submit' => array('miniorange_saml_validate_otp_submit'),
    );

    $form['miniorange_saml_customer_setup_resendotp'] = array(
      '#type' => 'submit',
      '#value' => t('Resend OTP'),
      '#submit' => array('miniorange_saml_resend_otp'),
    );

    $form['miniorange_saml_customer_setup_back'] = array(
      '#type' => 'submit',
      '#value' => t('Back'),
      '#submit' => array('miniorange_saml_back'),
    );

    return $form;
  }
  elseif ($current_status == 'PLUGIN_CONFIGURATION') {
    // Show customer configuration here.
    $form['markup_top'] = array(
      '#markup' => '<div>Thank you for registering with miniOrange</div>'
      . '<h4>Your Profile: </h4>',
    );

    $header = array(
      'email' => array('data' => t('Customer Email')),
      'customerid' => array('data' => t('Customer ID')),
      'token' => array('data' => t('Token Key')),
      'apikey' => array('data' => t('API Key')),
    );

    $options = array();

    $options[0] = array(
      'email' => variable_get('miniorange_saml_customer_admin_email', ''),
      'customerid' => variable_get('miniorange_saml_customer_id', ''),
      'token' => variable_get('miniorange_saml_customer_admin_token', ''),
      'apikey' => variable_get('miniorange_saml_customer_api_key', ''),
    );

    $form['fieldset']['customerinfo'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
    );

    return $form;
  }

  $form['markup_14'] = array(
    '#markup' => '<h3>Register with miniOrange</h3>',
  );

  $form['markup_15'] = array(
    '#markup' => 'Just complete the short registration below to configure'
    . ' the SAML Plugin. Please enter a valid email id <br>that you have'
    . ' access to. You will be able to move forward after verifying an OTP'
    . ' that we will send to this email.',
  );

  $form['miniorange_saml_customer_setup_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
  );

  $form['miniorange_saml_customer_setup_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
  );

  $form['markup_16'] = array(
    '#markup' => '<b>NOTE:</b> We will only call if you need support.',
  );

  $form['miniorange_saml_customer_setup_password'] = array(
    '#type' => 'password_confirm',
    '#required' => TRUE,
  );

  $form['miniorange_saml_customer_setup_button'] = array(
    '#type' => 'submit',
    '#value' => t('Register'),
    '#submit' => array('miniorange_saml_customer_setup_submit'),
  );

  return $form;

}

/**
 * Validate OTP.
 */
function miniorange_saml_validate_otp_submit(&$form, $form_state) {

  $otp_token = $form['miniorange_saml_customer_otp_token']['#value'];
  $username = variable_get('miniorange_saml_customer_admin_email', NULL);
  $phone = variable_get('miniorange_saml_customer_admin_phone', NULL);
  $tx_id = variable_get('miniorange_saml_tx_id', NULL);
  $customer_config = new MiniorangeSAMLCustomer($username, $phone, NULL, $otp_token);

  // Validate OTP.
  $validate_otp_response = json_decode($customer_config->validateOtp($tx_id));
  if ($validate_otp_response->status == 'SUCCESS') {
    // OTP Validated. Show Configuration page.
    $current_status = 'PLUGIN_CONFIGURATION';
    variable_set('miniorange_saml_status', $current_status);
    variable_del('miniorange_saml_tx_id');

    // OTP Validated. Create customer.
    $password = variable_get('miniorange_saml_customer_admin_password', '');
    $customer_config = new MiniorangeSAMLCustomer($username, $phone, $password, NULL);
    $create_customer_response = json_decode($customer_config->createCustomer());
    if ($create_customer_response->status == 'SUCCESS') {
      // Customer created.
      $current_status = 'PLUGIN_CONFIGURATION';
      variable_set('miniorange_saml_status', $current_status);
      variable_set('miniorange_saml_customer_admin_email', $username);
      variable_set('miniorange_saml_customer_admin_phone', $phone);
      variable_set('miniorange_saml_customer_admin_token', $create_customer_response->token);
      variable_set('miniorange_saml_customer_id', $create_customer_response->id);
      variable_set('miniorange_saml_customer_api_key', $create_customer_response->apiKey);

      drupal_set_message(t('Customer account created. Please read the instructions to setup your SAML Plugin in the next section'));
      drupal_goto('admin/config/people/miniorange_saml/licensing');
    }
    else {
      drupal_set_message(t('Error creating customer'), 'error');
    }
  }
  else {
    drupal_set_message(t('Error validating OTP'), 'error');
  }

}

/**
 * Resend OTP.
 */
function miniorange_saml_resend_otp(&$form, $form_state) {

  variable_del('miniorange_saml_tx_id');
  $username = variable_get('miniorange_saml_customer_admin_email', NULL);
  $phone = variable_get('miniorange_saml_customer_admin_phone', NULL);
  $customer_config = new MiniorangeSAMLCustomer($username, $phone, NULL, NULL);
  $send_otp_response = json_decode($customer_config->sendOtp());
  if ($send_otp_response->status == 'SUCCESS') {
    // Store txID.
    variable_set('miniorange_saml_tx_id', $send_otp_response->txId);
    $current_status = 'VALIDATE_OTP';
    variable_set('miniorange_saml_status', $current_status);
    drupal_set_message(t('Verify email address by entering the passcode sent to @username', array('@username' => $username)));
  }
}

/**
 * Handle submit for customer setup.
 */
function miniorange_saml_customer_setup_submit(&$form, $form_state) {

  $username = $form['miniorange_saml_customer_setup_username']['#value'];
  $phone = $form['miniorange_saml_customer_setup_phone']['#value'];
  $password = $form['miniorange_saml_customer_setup_password']['#value']['pass1'];

  $customer_config = new MiniorangeSAMLCustomer($username, $phone, $password, NULL);
  $check_customer_response = json_decode($customer_config->checkCustomer());
  if ($check_customer_response->status == 'CUSTOMER_NOT_FOUND') {
    // Create customer.
    // Store email and phone.
    variable_set('miniorange_saml_customer_admin_email', $username);
    variable_set('miniorange_saml_customer_admin_phone', $phone);
    variable_set('miniorange_saml_customer_admin_password', $password);

    $send_otp_response = json_decode($customer_config->sendOtp());
    if ($send_otp_response->status == 'SUCCESS') {
      // Store txID.
      variable_set('miniorange_saml_tx_id', $send_otp_response->txId);
      $current_status = 'VALIDATE_OTP';
      variable_set('miniorange_saml_status', $current_status);
      drupal_set_message(t('Verify email address by entering the passcode sent to @username', array('@username' => $username)));
    }
  }
  elseif ($check_customer_response->status == 'CURL_ERROR') {
    drupal_set_message(t('cURL is not enabled. Please enable cURL'), 'error');
  }
  else {
    // Customer exists. Retrieve keys.
    $customer_keys_response = json_decode($customer_config->getCustomerKeys());
    if (json_last_error() == JSON_ERROR_NONE) {
      variable_set('miniorange_saml_customer_id', $customer_keys_response->id);
      variable_set('miniorange_saml_customer_admin_token', $customer_keys_response->token);
      variable_set('miniorange_saml_customer_admin_email', $username);
      variable_set('miniorange_saml_customer_admin_phone', $phone);
      variable_set('miniorange_saml_customer_api_key', $customer_keys_response->apiKey);
      $current_status = 'PLUGIN_CONFIGURATION';
      variable_set('miniorange_saml_status', $current_status);
      drupal_set_message(t('Successfully retrieved your account.'));
      drupal_goto('admin/config/people/miniorange_saml/licensing');
    }
    else {
      drupal_set_message(t('Invalid credentials'), 'error');
    }
  }
}

/**
 * Handle back button submit for customer setup.
 */
function miniorange_saml_back(&$form, $form_state) {
  $current_status = 'CUSTOMER_SETUP';
  variable_set('miniorange_saml_status', $current_status);
  variable_del('miniorange_saml_customer_admin_email');
  variable_del('miniorange_saml_customer_admin_phone');
  variable_del('miniorange_saml_tx_id');
  drupal_set_message(t('Register/Login with your miniOrange Account'));
}
