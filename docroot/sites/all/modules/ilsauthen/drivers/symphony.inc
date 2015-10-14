<?php
/**
 * @file
 * SirsiDynix Symphony ILS driver for ilsauthen.module.
 *
 * The driver was written against Symphony Web Services (SymWS) 3.2,
 * which customers of SirsiDynix may acquire at
 * <http://support.sirsidynix.com/kA57000000000b8>, after first logging in
 * to the SirsiDynix Support Center at <http://support.sirsidynix.com>.
 */

/**
 * Implements ilsauthen_driver_meta(). @see sample.inc.
 */
function ilsauthen_driver_meta() {
  return array(
    'driver_name' => 'symphony',
  );
}

/**
 * Implements ilsauthen_form_alter_driver(). @see sample.inc.
 */
function ilsauthen_form_alter_driver(&$form, $form_state, $form_id) {
  if ($form_id == 'user_login' || $form_id == 'user_login_block') {
    $form['name']['#title'] = 'Library ID';
    $form['pass']['#title'] = 'Library PIN';
  }

  if ($form_id == 'ilsauthen_admin_settings') {
    $form['ilsauthen_symphony_reset_password_message'] = array(
      '#type' => 'textarea',
      '#title' => t('Message sent to users when they reset their password'),
      '#default_value' => variable_get('ilsauthen_symphony_reset_password_message', "Please go to http://passwords.myexample.ca to change your password."),
      '#required' => TRUE,
      '#weight' => 0,
    );

    $form['ilsauthen_symphony_base_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Base URL'),
      '#default_value' => variable_get(
        'ilsauthen_symphony_base_url',
        'http://localhost:8080/symws'
      ),
      '#required' => TRUE,
    );

    $form['ilsauthen_symphony_client_id'] = array(
      '#type' => 'textfield',
      '#title' => t('clientID'),
      '#default_value' => variable_get(
        'ilsauthen_symphony_client_id',
        'Drupal'
      ),
      '#required' => TRUE,
    );
  }
}

/**
 * Implements ilsauthen_driver_login_validation(). @see sample.inc.
 */
function ilsauthen_driver_login_validation($form, &$form_state) {
}

/**
 * Implements ilsauthen_get_email_address(). @see sample.inc.
 *
 * @todo Check for availability of the Patron service,
 * and if available, pull the user's email address,
 * which is not provided by the authenticateUser operation.
 */
function ilsauthen_get_email_address() {
  return FALSE;
}

/**
 * Implements ilsauthen_driver_connect(). @see sample.inc.
 */
function ilsauthen_driver_connect($user_data) {
  $symws_base_url = variable_get(
    'ilsauthen_symphony_base_url',
    'http://localhost:8080/symws'
  );

  $symws_client_id = variable_get(
    'ilsauthen_symphony_client_id',
    'Drupal'
  );

  $query = http_build_query(array(
    'login' => $user_data['name'],
    'password' => $user_data['pass'],
    'clientID' => $symws_client_id,
  ));

  $url = "$symws_base_url/rest/security/authenticateUser?$query";

  if (($response = @file_get_contents($url)) === FALSE) {
    ilsauthen_remote_connection_error('symphony', $symws_base_url);
    return FALSE;
  }
  else {
    return trim($response) == 'true';
  }
}
