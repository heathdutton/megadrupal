<?php
/**
 * @file
 * Driver API documentation for ilsauthen.module.
 */

/**
 * Stores basic metadata about this driver.
 * 
 * @return array
 *   An array that contains the driver name and may contain information
 *   about a required php class library file.
 */
function ilsauthen_driver_meta() {
  return array(
    // Don't wrap any of this in t(); the values are for paths & filenames.
    // The driver filename without the file extension or preceding dot.
    'driver_name' => 'sample',
    // This section is optional and only for required php libraries that can't
    // be included with the driver file.
    // Begin optional section.
    'driver_requirements' => array(
      // The library filename without the file extension or preceding dot.
      // Eg. for file sample.class.inc, the value is sample.class
      'php_class_filename' => 'sample.class',
      // The extension without the preceding dot.
      'php_class_filetype' => 'inc',
      // The URL where users can download the library.
      'php_class_download_url' => 'http://example.com/download/sample.class.inc',
    ),
    // End optional section.
  );
}

/**
 * Allows driver to add its own hook_form_alter() code.
 *
 * @param array &$form
 *   Nested array of form elements that comprise the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form. The arguments that
 *   drupal_get_form() was originally called with are available in the array
 *   $form_state['build_info']['args'].
 * @param string $form_id
 *   String representing the name of the form itself. Typically this is the name
 *   of the function that generated the form.
 * See hook_form_alter for details.
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
      '#default_value' => variable_get($reset_password_message_element, "Please go to https://passwords.example.com/myaccount/changepassword to change your password."),
      '#required' => TRUE,
      '#weight' => 0,
    );

    // This setting is just an example.
    $form['ilsauthen_example_setting_from_sample_driver'] = array(
      '#type' => 'textfield',
      '#title' => t('Test setting from sample driver'),
      '#default_value' => variable_get('ilsauthen_example_setting_from_sample_driver',
        'You should choose another driver (like Evergreen) because someone could log in with usersample/usersamplepass'),
      '#size' => 90,
      '#required' => FALSE,
      '#weight' => 0,
    );
  }
}

/**
 * Validate elements in the login form.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form. The arguments that
 *   drupal_get_form() was originally called with are available in the array
 *   $form_state['build_info']['args'].
 * The module is already handling all of the voodoo of sorting out which users
 * are handled internally by Drupal and which should be handled by your driver.
 * This function is just for ensuring that the input values are sane for your
 * ILS.  Don't do anything here if you aren't performing extra validation.
 */
function ilsauthen_driver_login_validation($form, &$form_state) {
  // Begin optional section.
  if ($form_state['values']['name'] == 'usersample1') {
    form_set_error('name', t('Hey, do not add a 1 to the end of the username'));
  }
  // End optional section.
}

/**
 * Gets email address from external authentication source.
 *
 * @return string
 *   $_SESSION['ilsauthen_driver_mail_address']
 *   The email address, if given. Not required, but very desirable.
 */
function ilsauthen_get_email_address() {
  // Create a session variable that is defined in ils_authen_driver_connect().
  // to avoid a second call to the external authen source.
  if (!empty($_SESSION['ilsauthen_driver_mail_address'])) {
    return $_SESSION['ilsauthen_driver_mail_address'];
  }
}

/**
 * Connects to external authentication source.
 *
 * @param array $user_data
 *   A copy of the login form's $form_values array.
 *
 * @return bool
 *   TRUE if the credentials authenticate in the external source,
 *   FALSE if they don't.
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
