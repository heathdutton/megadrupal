<?php
/**
 * @file
 * Evergreen driver for ilsauthen.module.
 */

/**
 * Function that stores some basic metadata about this driver.
 */
function ilsauthen_driver_meta() {
  return array(
    'driver_name' => 'evergreen',
  );
}

/**
 * Allows driver to add its own hook_form_alter() code.
 *
 * The two forms that the driver will most likely want to modify are
 * the login form and the module's admin settings form, both identified below.
 */
function ilsauthen_form_alter_driver(&$form, $form_state, $form_id) {
  if ($form_id == 'ilsauthen_admin_settings') {
    $form['ilsauthen_evergreen_server_url'] = array(
      '#type' => 'textfield',
      '#title' => t("Evergreen server's URL"),
      '#default_value' => variable_get('ilsauthen_evergreen_server_url', 'http://my.evergreen.lib.ca'),
      '#description' => t('Include the http:// but not the trailing slash.'),
      '#size' => 60,
      '#required' => TRUE,
      '#weight' => -9,
    );

    // Build list of authentication methods
    // (Evergreen supports XML-RPC and JSON),
    // depending on what extensions are loaded in PHP.
    $auth_methods = array();
    if (extension_loaded('xmlrpc')) {
      $auth_methods['XML-RPC'] = 'XML-RPC';
    }
    if (extension_loaded('curl') && extension_loaded('json')) {
      $auth_methods['JSON'] = 'JSON';
    }

    $form['ilsauthen_evergreen_authen_method'] = array(
      '#type' => 'select',
      '#title' => t('Authentication method'),
      '#default_value' => variable_get('ilsauthen_evergreen_authen_method', 'XML-RPC'),
      '#options' => $auth_methods,
      '#description' => t("Select XML-RPC if given the choice."),
      '#weight' => -8,
    );

    $driver_meta = ilsauthen_driver_meta();
    $reset_password_message_element = 'ilsauthen_' . $driver_meta['driver_name'] . '_reset_password_message';
    $form[$reset_password_message_element] = array(
      '#type' => 'textarea',
      '#title' => t('Message sent to users when they reset their password'),
      '#default_value' => variable_get($reset_password_message_element, "Please go to http://passwords.myexample.ca to change your password.\n\nThank you."),
      '#description' => t('You my include HTML markup.'),
      '#required' => TRUE,
      '#weight' => -7,
    );
  }
}

/**
 * Allows drivers to validate elements in the login form.
 *
 * Don't do anything here if no validation is being performed.
 */
function ilsauthen_driver_login_validation($form, &$form_state) {
  // No login form validation in this driver.
}

/**
 * Gets email address from external authentication source.
 *
 * Not required, but very desirable.
 */
function ilsauthen_get_email_address() {
  // Create a session variable that is defined in ils_authen_driver_connect()
  // to avoid a second call to the external authen source.
  return $_SESSION['ilsauthen_driver_mail_address'];
}

/**
 * Connects to external authentication source.
 * 
 * Returns TRUE or FALSE to the main module.
 * $user_data is a copy of the login form's $form_values array.
 */
function ilsauthen_driver_connect($user_data) {
  // Test whether the user entered a username or a barcode and define the
  // login identity accordingly.
  // $nametype = 'barcode' if ($username =~ /^\d+$/o);
  // is used in perlmods/OpenILS/Reporter/Proxy.pm and
  // perlmods/OpenILS/WWW/Proxy.pm.
  if (preg_match("/^\d+$/", $user_data['name'])) {
    $identity = 'barcode';
  }
  else {
    $identity = 'username';
  }
  $ret_value = FALSE;
  $evergreen_server = variable_get('ilsauthen_evergreen_server_url', 'http://my.evergreen.lib.ca');
  $auth_method = variable_get('ilsauthen_evergreen_authen_method', 'XML-RPC');
  switch ($auth_method) {
    case 'XML-RPC':
      $url = $evergreen_server . '/xml-rpc/open-ils.auth';
      // First, get the seed.
      $method = 'open-ils.auth.authenticate.init';
      $seed = xmlrpc($url, $method, $user_data['name']);
      // Call general error handler if can't connect.
      if ($error = xmlrpc_error()) {
        ilsauthen_remote_connection_error('evergreen', $evergreen_server, $error->code, $error->message);
        return FALSE;
      }
      // Then, log in.
      $md5_password = md5($seed . md5($user_data['pass']));
      $method = 'open-ils.auth.authenticate.complete';
      $login_params = array(
        'password' => $md5_password, 'type' => 'opac', 'org' => NULL,
        $identity => $user_data['name'],
      );
      $result = xmlrpc($url, $method, $login_params);
      // Call general error handler if can't connect.
      if ($error = xmlrpc_error()) {
        ilsauthen_remote_connection_error('evergreen', $evergreen_server, $error->code, $error->message);
        return FALSE;
      }
      if ($result['textcode'] == 'SUCCESS') {
        $ret_value = TRUE;
      }
      break;

    case 'JSON':
      $url = $evergreen_server . '/gateway';
      // First, get the seed.
      $post_data = 'method=open-ils.auth.authenticate.init&service=open-ils.auth&param="' . $user_data['name'] . '"';
      $curl = curl_init($url);
      curl_setopt($curl, CURLOPT_POST, TRUE);
      curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
      curl_setopt($curl, CURLOPT_POSTFIELDS, $post_data);
      $seed = curl_exec($curl);
      // Call general error handler if can't connect.
      if (curl_errno($curl)) {
        ilsauthen_remote_connection_error('evergreen', $evergreen_server, curl_errno($curl), curl_error($curl));
        return FALSE;
      }
      $seed = json_decode($seed);
      // Then, log in.
      $md5_password = md5($seed->payload[0] . md5($user_data['pass']));
      $login_params = json_encode(array(
        'password' => $md5_password,
        'type' => 'opac',
        'org' => NULL,
        $identity => $user_data['name'],
      ));
      $post_data = 'method=open-ils.auth.authenticate.complete&service=open-ils.auth&param=' . $login_params;
      curl_setopt($curl, CURLOPT_POSTFIELDS, $post_data);
      $result = curl_exec($curl);
      // Call general error handler if can't connect.
      if (curl_errno($curl)) {
        ilsauthen_remote_connection_error('evergreen', $evergreen_server, curl_errno($curl), curl_error($curl));
        return FALSE;
      }
      curl_close($curl);
      $result = json_decode($result);
      if ($result->payload[0]->textcode == 'SUCCESS') {
        $ret_value = TRUE;
      }
      break;
  }
  return $ret_value;
}
