<?php
/**
 * @file
 * Innovative Interfaces driver for ilsauthen.module. Will only if the IP
 * address of your Drupal server is registered with your III server. Currently
 * this driver only supports the use of name and barcode, not PINs.
 */

/**
 * Function that stores some basic metadata about this driver.
 */
function ilsauthen_driver_meta() {
  return array(
    'driver_name' => 'iii',
  );
}

/**
 * Allows driver to add its own hook_form_alter() code.
 *
 * The two forms that the driver will most likely want to modify are
 * the login form and the module's admin settings form, both identified below.
 */
function ilsauthen_form_alter_driver(&$form, $form_state, $form_id) {
  // Modify the login form so it reflects III wording.
  if ($form_id == 'user_login' || $form_id == 'user_login_block') {
    $form['name']['#title'] = t('Library barcode');
    $form['pass']['#title'] = t('Your name exactly as it appears on your library card');
  }

  // Add field to admin settings form store III patron API URL.
  if ($form_id == 'ilsauthen_admin_settings') {
    $form['ilsauthen_iii_patron_api'] = array(
      '#type' => 'textfield',
      '#title' => t('III patron API URL'),
      '#default_value' => variable_get('ilsauthen_iii_patron_api', 'http://my.iii.lib.ca:3000/API/'),
      '#description' => t("The full URL, including http://, port number, path to the API, and trailing slash. You will need to register your Drupal server's IP address with your III server in order for authentication to work."),
      '#size' => 60,
      '#required' => TRUE,
      '#weight' => -9,
    );
    $form['ilsauthen_iii_min_name_length'] = array(
      '#type' => 'textfield',
      '#title' => t('Minimum length of patron name'),
      '#default_value' => variable_get('ilsauthen_iii_min_name_length', '3'),
      '#description' => t("Minimum length of the patron's name. Used as a security precaution."),
      '#size' => 5,
      '#required' => TRUE,
      '#weight' => -8,
    );

    $driver_meta = ilsauthen_driver_meta();
    $reset_password_message_element = 'ilsauthen_' . $driver_meta['driver_name'] . '_reset_password_message';
    $form[$reset_password_message_element] = array(
      '#type' => 'textarea',
      '#title' => t('Message sent to users when they reset their password'),
      '#default_value' => variable_get($reset_password_message_element, "Please go to http://passwords.myexample.ca to change your password.\n\nThank you."),
      '#description' => t('You may include HTML markup.'),
      '#required' => TRUE,
      '#weight' => -5,
    );
  }
}

/**
 * Allows drivers to validate elements in the login form.
 */
function ilsauthen_driver_login_validation($form, &$form_state) {
  // Check to make sure that the name field is as long as the
  // 'ilsauthen_iii_min_name_length' setting value.
  $min_len = variable_get('ilsauthen_iii_min_name_length', '3');
  if (drupal_strlen($form_state['values']['pass']) < $min_len) {
    form_set_error('name', t('Your name must be at least !min_len characters long', array('!min_len' => $min_len)));
  }
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
  // Query the III patron API via GET URL.
  $iii_patron_api_url = variable_get('ilsauthen_iii_patron_api', '');
  $iii_patron_api_url = $iii_patron_api_url . $user_data['name'] . '/dump';
  $iii_get = drupal_http_request($iii_patron_api_url);

  // Basic error checking on drupal_http_request()
  if ($iii_get->error) {
    ilsauthen_remote_connection_error('iii', $iii_patron_api_url);
    return FALSE;
  }

  // Parse the returned record and create an array of fields.
  $iii_rec = $iii_get->data;
  $iii_rec = preg_replace('/<HTML><BODY>/', '', $iii_rec);
  $iii_rec = preg_replace('/<\/BODY><\/HTML>/', '', $iii_rec);
  $iii_rec = preg_replace('/^\s+$/', '', $iii_rec);
  $iii_rec_attributes = array();
  $iii_rec_lines = explode('<BR>', $iii_rec);
  foreach ($iii_rec_lines as $iii_rec_line) {
    $iii_rec_line = trim($iii_rec_line);
    $iii_rec_line = explode('=', $iii_rec_line);
    $iii_attribute_key = $iii_rec_line[0];
    if (!empty($iii_attribute_key)) {
      $iii_rec_attributes[$iii_attribute_key] = $iii_rec_line[1];
    }
  }

  // We need an exact match on the barcode but a substring match on name is OK.
  $pass_pattern = '/' . $user_data['pass'] . '/';
  if (($iii_rec_attributes['P BARCODE[pb]'] == $user_data['name']) && preg_match($pass_pattern, $iii_rec_attributes['PATRN NAME[pn]'])) {
    $_SESSION['ilsauthen_driver_mail_address'] = $iii_rec_attributes['EMAIL ADDR[pz]'];
    return TRUE;
  }
  else {
    return FALSE;
  }
}
