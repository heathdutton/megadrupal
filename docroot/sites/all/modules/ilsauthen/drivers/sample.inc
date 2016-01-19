<?php
/**
 * @file
 * Sample driver for ilsauthen.module. DO NOT DELETE this driver since it
 * contains defaults that might cause White Screen of Death if they are not
 * available at install (and reinstall) time. For security purposes, you should
 * choose another as soon as possible, or change the values of $sample_account
 * in ilsauthen_driver_connect() so people can't log in using the sample
 * account. See evergreen.inc for examples of how to use
 * ilsauthen_remote_connection_error() to check for problems connecting to a
 * remote authentication source.
 */

/**
 * Function that stores some basic metadata about this driver.
 */
function ilsauthen_driver_meta() {
  return array(
    // Do not wrap in t() since this should be the same as the filename.
    'driver_name' => 'sample',
  );
}

/**
 * Allows driver to add its own hook_form_alter() code.
 *
 * The two forms that the driver will most likely want to modify are
 * the login form and the module's admin settings form, both identified below.
 */
function ilsauthen_form_alter_driver(&$form, $form_state, $form_id) {
  // Changes wording on the login form.
  if ($form_id == 'user_login' || $form_id == 'user_login_block') {
    $form['name']['#title'] = 'Your custom username field title';
    $form['pass']['#title'] = '... and a custom password field title.';
  }

  if ($form_id == 'ilsauthen_admin_settings') {
    // This admin setting is required, since it tells users of this module
    // where to go to change their password -- Drupal doesn't manage passwords
    // for accounts created with external authentication services.
    $driver_meta = ilsauthen_driver_meta();
    $reset_password_message_element = 'ilsauthen_' . $driver_meta['driver_name'] . '_reset_password_message';
    $form[$reset_password_message_element] = array(
      '#type' => 'textarea',
      '#title' => t('Message sent to users when they reset their password'),
      '#default_value' => variable_get($reset_password_message_element, "Please go to http://passwords.myexample.ca to change your password."),
      '#required' => TRUE,
      '#weight' => 0,
    );

    // This setting is just an example.
    $form['ilsauthen_example_setting_from_sample_driver'] = array(
      '#type' => 'textfield',
      '#title' => t('Test setting from sample driver'),
      '#default_value' => variable_get('ilsauthen_example_setting_from_sample_driver',
        'You should choose another driver (like Evergreen) becuase someone could log in with usersample/usersamplepass'),
      '#size' => 90,
      '#required' => FALSE,
      '#weight' => 0,
    );
  }
}

/**
 * Allows drivers to validate elements in the login form.
 *
 * Don't do anything here if no validation is being performed.
 */
function ilsauthen_driver_login_validation($form, &$form_state) {
  // Sample code:
  // if ($form_state['values']['name'] == 'usersample1') {
  // form_set_error('name', 'Hey, do not add a 1 to the end of the username');
  // }
  // .
}

/**
 * Gets email address from external authentication source.
 *
 * Not required, but very desirable.
 */
function ilsauthen_get_email_address() {
  // Create a session variable that is defined in ils_authen_driver_connect().
  // to avoid a second call to the external authen source.
  return $_SESSION['ilsauthen_driver_mail_address'];
}

/**
 * Connects to external authentication source.
 *
 * Returns TRUE or FALSE to the main module.
 * $user_data is a copy of the login form's $form_values array.
 *
 * This sample function will return TRUE if the credentials
 * usersample/usersamplepass are entered in the login form, FALSE otherwise.
 * It doesn't actually connect to an external authentication source,
 * but you get the idea.
 */
function ilsauthen_driver_connect($user_data) {
  // This function would normally connect to an external authentication source
  // instead of using hard-coded credentials like $sample_account.
  $sample_account = array('sampleusername' => 'usersample', 'sampleuserpass' => 'usersamplepass');
  if (($user_data['name'] == $sample_account['sampleusername']) && ($user_data['pass'] == $sample_account['sampleuserpass'])) {
    $_SESSION['ilsauthen_driver_mail_address'] = 'usersample@sample.com';
    return TRUE;
  }
  else {
    return FALSE;
  }
}
