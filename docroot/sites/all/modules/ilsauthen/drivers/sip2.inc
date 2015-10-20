<?php
/**
 * @file
 * Generic SIP2 driver for the ILS Authentication module.
 * Author: Jason Sherman jsherman@usao.edu
 * Based on the excellent php-sip2 class library by John Wohler.
 */

/**
 * Function that stores some basic metadata about this driver.
 */
function ilsauthen_driver_meta() {
  return array(
    // Do not wrap in t() since this should be the same as the filename.
    'driver_name' => 'sip2',
    'driver_requirements' => array(
      'php_class_filename' => 'sip2.class',
      'php_class_filetype' => 'inc',
      'php_class_download_url' => 'http://raw.github.com/jsnshrmn/php-sip2-drupal/ils-authen-7.x-1.x/sip2.class.inc',
    ),
  );
}

// First things first, make sure we have the required php class library.
$pathtosip2 = realpath(".") . '/'  . drupal_get_path('module', 'ilsauthen') . '/' . 'drivers/';
$sip2filename = 'sip2.class.inc';
$sip2absolute = $pathtosip2 . $sip2filename;
if (!file_exists($sip2absolute)) {
  drupal_set_message(t('Please install the php-sip2 class.  You can download it directly from <a href="@url">github</a>. Save %sip2filename in %pathtosip2', array(
    '@url' => url("http://raw.github.com/jsnshrmn/php-sip2-drupal/ils-authen-7.x-1.x/sip2.class.inc"),
    '%sip2filename' => $sip2filename,
    '%pathtosip2' => $pathtosip2,
  )), 'warning', TRUE);
}
else {
  module_load_include('inc', 'ilsauthen', '/drivers/sip2.class');
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
    $form['name']['#title'] = variable_get('ilsauthen_sip_username_title', t('Cardnumber'));
    $form['name']['#description'] = variable_get('ilsauthen_sip_username_description', t('Enter your Cardnumber'));
    $form['pass']['#title'] = variable_get('ilsauthen_sip_userpass_title', t('Password'));
    $form['pass']['#description'] = variable_get('ilsauthen_sip_userpass_description', t('Enter your Password'));
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
      '#default_value' => variable_get($reset_password_message_element, t("Please go to https://koha.example.org/cgi-bin/koha/opac-user.pl to change your password.")),
      '#required' => TRUE,
      '#weight' => 0,
    );

    // Network Settings.
    $form['network_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t("Network Settings"),
      '#weight' => -10,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    // Host.
    $form['network_settings']['ilsauthen_sip_server_address'] = array(
      '#type' => 'textfield',
      '#title' => t("SIP Host (For testing only)"),
      '#default_value' => variable_get('ilsauthen_sip_server_address', t('localhost')),
      '#description' => t("Include the address or hostname but not the port. Production systems should use a secure tunnel and leave this value at localhost."),
      '#size' => 25,
      '#required' => TRUE,
      '#disabled' => FALSE,
      '#weight' => -9,
    );

    // Port.
    $form['network_settings']['ilsauthen_sip_server_port'] = array(
      '#type' => 'textfield',
      '#title' => t("SIP server's listening port"),
      '#default_value' => variable_get('ilsauthen_sip_server_port', t('6001')),
      '#description' => t("Koha default: 6001, SirsiDynix Default: 6002."),
      '#size' => 25,
      '#required' => TRUE,
      '#weight' => -8,
    );

    // SIP Settings.
    $form['sip_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t("SIP Settings"),
      '#weight' => -7,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    // SIP Login User.
    $form['sip_settings']['ilsauthen_sip_server_user'] = array(
      '#type' => 'textfield',
      '#title' => t("SIP username"),
      '#default_value' => variable_get('ilsauthen_sip_server_user', t('sipuser')),
      '#description' => t("Include this if your SIP server expects a login, this is NOT a telnet credential."),
      '#size' => 25,
      '#required' => FALSE,
      '#weight' => -7,
    );

    // SIP Login Password.
    $form['sip_settings']['ilsauthen_sip_server_pass'] = array(
      '#type' => 'textfield',
      '#title' => t("SIP password"),
      '#default_value' => variable_get('ilsauthen_sip_server_pass', t('sippass')),
      '#description' => t("Include this if your SIP server expects a login, this is NOT a telnet credential."),
      '#size' => 25,
      '#required' => FALSE,
      '#weight' => -6,
    );

    // SIP Login Location.
    $form['sip_settings']['ilsauthen_sip_server_scloc'] = array(
      '#type' => 'textfield',
      '#title' => t("SIP 'self check' location code"),
      '#default_value' => variable_get('ilsauthen_sip_server_scloc', NULL),
      '#description' => t("Check your ILS's documentation & configuration."),
      '#size' => 25,
      '#required' => FALSE,
      '#weight' => -5,
    );

    // Advanced Settings.
    $form['sip_settings']['advanced_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t("Advanced Settings"),
      '#weight' => -3,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    // SIP Debugging.
    $form['sip_settings']['advanced_settings']['ilsauthen_sip_server_debug'] = array(
      '#type' => 'checkbox',
      '#title' => t("Enable SIP debugging"),
      '#default_value' => variable_get('ilsauthen_sip_server_debug', NULL),
      '#description' => t("Displays SIP debug messages when enabled."),
      '#required' => FALSE,
      '#weight' => -5,
    );

    // Patron Group Field.
    $form['sip_settings']['advanced_settings']['ilsauthen_sip_server_group_field'] = array(
      '#type' => 'textfield',
      '#title' => t("SIP field for patron group/category"),
      '#default_value' => variable_get('ilsauthen_sip_server_group_field', NULL),
      '#description' => t("Check your ILS's documentation & configuration.  This is outside of the standard."),
      '#size' => 25,
      '#required' => FALSE,
      '#weight' => -4,
    );

    // Allowed Patron Group Codes.
    $form['sip_settings']['advanced_settings']['ilsauthen_sip_server_allowed_group_codes'] = array(
      '#type' => 'textfield',
      '#title' => t("Comma separated list of patron group/category codes allowed to login"),
      '#default_value' => variable_get('ilsauthen_sip_server_allowed_group_codes', NULL),
      '#description' => t("Check your ILS's documentation & configuration.  This is outside of the standard."),
      '#size' => 25,
      '#required' => FALSE,
      '#weight' => -3,
    );

    // Mapped Patron Group Codes.
    $form['sip_settings']['advanced_settings']['ils_sip_server_map_group_codes_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Map SIP server koha codes'),
      '#description' => t('Map SIP server patron codes to drupal user roles. When a ILS user logs into drupal their patron code is recorded and they will be assigned to the role specified. Patron codes are CASE SENSITIVE'),
      '#weight' => -2,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $user_roles = user_roles(TRUE);
    $drupal_roles = str_replace(' ', '_', $user_roles);

    foreach ($drupal_roles as $drupal_role) {
      // Map Patron codes to drupal roles (to be assigned on user creation).
      $form['sip_settings']['advanced_settings']['ils_sip_server_map_group_codes_fieldset']['ilsauthen_sip_server_map_group_codes_' . $drupal_role] = array(
        '#type' => 'textfield',
        '#title' => t("Map koha patron codes to: @drupal_role", array('@drupal_role' => $drupal_role)),
        '#default_value' => variable_get('ilsauthen_sip_server_map_group_codes_' . $drupal_role, NULL),
        '#description' => t("List comma seperated koha patron group codes to be associated with @drupal_role (case sensitive, no spaces)", array('@drupal_role' => $drupal_role)),
        '#size' => 25,
        '#required' => FALSE,
      );
    }

    // Changes wording on the login form.
    $form['login_wording'] = array(
      '#type' => 'fieldset',
      '#title' => t("Login Form"),
      '#description' => t("Changes wording on the login form."),
      '#weight' => -4,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    // Drupal username title.
    $form['login_wording']['ilsauthen_sip_username_title'] = array(
      '#type' => 'textfield',
      '#title' => t("Login username field title"),
      '#default_value' => variable_get('ilsauthen_sip_username_title', t('Cardnumber')),
      '#size' => 25,
      '#required' => TRUE,
      '#weight' => -8,
    );

    // Drupal username description.
    $form['login_wording']['ilsauthen_sip_username_description'] = array(
      '#type' => 'textfield',
      '#title' => t("Login username field description"),
      '#default_value' => variable_get('ilsauthen_sip_username_description', t('Enter your Cardnumber')),
      '#size' => 25,
      '#required' => TRUE,
      '#weight' => -7,
    );

    // Drupal password title.
    $form['login_wording']['ilsauthen_sip_userpass_title'] = array(
      '#type' => 'textfield',
      '#title' => t("Login password field title"),
      '#default_value' => variable_get('ilsauthen_sip_userpass_title', t('Password')),
      '#size' => 25,
      '#required' => TRUE,
      '#weight' => -6,
    );

    // Drupal password description.
    $form['login_wording']['ilsauthen_sip_userpass_description'] = array(
      '#type' => 'textfield',
      '#title' => t("Login password field description"),
      '#default_value' => variable_get('ilsauthen_sip_userpass_description', t('Enter your Password')),
      '#size' => 25,
      '#required' => TRUE,
      '#weight' => -5,
    );

  }
}

/**
 * Allows drivers to validate elements in the login form.
 *
 * Don't do anything here if no validation is being performed.
 */
function ilsauthen_driver_login_validation($form, &$form_state) {

}

/**
 * Gets email address from external authentication source.
 *
 * Not required, but very desirable.
 */
function ilsauthen_get_email_address() {
  // Create a session variable that is defined in ils_authen_driver_connect()
  // to avoid a second call to the external authen source.
  if (!empty($_SESSION['ilsauthen_driver_mail_address'])) {
    return $_SESSION['ilsauthen_driver_mail_address'];
  }
}

/**
 * Connects to external authentication source.
 * 
 * Returns TRUE or FALSE to the main module.
 * $user_data is a copy of the login form's $form_values array.
 */
function ilsauthen_driver_connect($user_data) {
  $sip_server_address = variable_get('ilsauthen_sip_server_address', t('localhost'));
  $sip_server_port = variable_get('ilsauthen_sip_server_port', t('6002'));
  $sip_server_debug = variable_get('ilsauthen_sip_server_debug', NULL);
  $sip_server_user = variable_get('ilsauthen_sip_server_user', t('sipuser'));
  $sip_server_pass = variable_get('ilsauthen_sip_server_pass', t('sippass'));
  $sip_server_scloc = variable_get('ilsauthen_sip_server_scloc', NULL);
  $sip_server_group_field = variable_get('ilsauthen_sip_server_group_field', NULL);
  $sip_server_allowed_group_codes = variable_get('ilsauthen_sip_server_allowed_group_codes', NULL);
  $sip_server_mapped_group_codes = ilsauthen_map_user_role();

  // Create Sip2 object.
  $mysip = new Sip2();

  // Populate the object with form settings.
  if ($sip_server_debug == 1) {
    $mysip->debug = TRUE;
  }
  else {
    $mysip->debug = FALSE;
  }
  $mysip->hostname = $sip_server_address;
  $mysip->port = $sip_server_port;
  $mysip->sip_login = $sip_server_user;
  $mysip->sip_password = $sip_server_pass;
  $mysip->scLocation = $sip_server_scloc;

  // Connect to SIP server.
  $result = $mysip->sip2Connect();

  // SC Login.
  $login = $mysip->sip2MsgLogin($mysip->sip_login, $mysip->sip_password);
  $result = $mysip->sip2ParseLoginResponse($mysip->sip2GetMessage($login));

  // Send selfcheck status message.
  $in = $mysip->sip2MsgScStatus();
  $result = $mysip->sip2ParseAcsStatusResponse($mysip->sip2GetMessage($in));

  // Use result to populate SIP2 setings.
  if (isset($result['variable']['AO'][0])) {
    $mysip->AO = $result['variable']['AO'][0]; /* set AO to value returned */
  }
  else {
    $mysip->AO = "NOTINACSSTATUS";
  }
  if (isset($result['variable']['AN'][0])) {
    $mysip->AN = $result['variable']['AN'][0]; /* set AN to value returned */
  }

  // Patron login.
  $mysip->patron = $user_data['name'];
  $mysip->patronpwd = $user_data['pass'];

  // Load patron status into array.
  $in = $mysip->sip2MsgPatronInformation('none');
  $result = $mysip->sip2ParsePatronInfoResponse($mysip->sip2GetMessage($in));

  // Were the name an pass correct?
  if ((!empty($result['variable']['CQ'][0])) && (!empty($result['variable']['BL'][0]))) {
    if (($result['variable']['CQ'][0] == 'Y') && ($result['variable']['BL'][0] == 'Y')) {
      if (!empty($result['variable']['BE'][0])) {
        $_SESSION['ilsauthen_driver_mail_address'] = $result['variable']['BE'][0];
      }
      // Has the administrator selected a field for user groups/categories,
      // as well as selected allowed groups?
      // and if so, does this user fall into one of those groups?
      if ((!empty($sip_server_group_field)) && (!empty($sip_server_allowed_group_codes))) {
        if (!empty($result['variable'][$sip_server_group_field][0])) {
          $group = $result['variable'][$sip_server_group_field][0];
          $allowed_groups = explode(",", $sip_server_allowed_group_codes);
          foreach ($allowed_groups as $allowed_group) {
            if ($group == $allowed_group) {
              // If patron codes to roles also mapped.
              if (!empty($sip_server_mapped_group_codes)) {
                $patron_code = $result['variable']['PC'][0];
                return ilsauthen_check_user_role($patron_code);
              }
            }
          }
        }
      }
      // If user has not selected user groups/categories but has
      // mapped patron codes to drupal roles.
      if (!empty($sip_server_mapped_group_codes)) {
        $patron_code = $result['variable']['PC'][0];
        return ilsauthen_check_user_role($patron_code);
      }
      else {
        return TRUE;
      }
    }
  }
  else {
    return FALSE;
  }
}

/**
 * Maps patron codes from ILS to user roles in Drupal.
 *
 * Takes patron roles mapped to roles in admin config and writes them into an
 * multi-dimensional array where the key is mapped role, value is patron codes.
 */
function ilsauthen_map_user_role() {
  $drupal_roles = user_roles(TRUE);
  $patron_codes_to_user = array();
  foreach ($drupal_roles as $drupal_role) {
    // Clean out the underscores as drupal variables don't like spaces.
    $cleaned_drupal_role = str_replace(' ', '_', $drupal_role);
    $patron_group_codes = variable_get('ilsauthen_sip_server_map_group_codes_' . $cleaned_drupal_role, NULL);
    // Explodes each patron code into a separate value.
    if (!empty($patron_group_codes)) {
      $split_codes = explode(',', $patron_group_codes);
      foreach ($split_codes as $split_code) {
        $patron_codes_to_user[$drupal_role][] = $split_code;
      }
    }
  }
  return $patron_codes_to_user;
}

/**
 * Matches ILS patron codes to Drupal roles defined in previous function.
 *
 * Takes patron_code return in authenticate success message and checks
 * it against mapped roles. If a match found the associated role is returned.
 */
function ilsauthen_check_user_role($patron_code) {
  $sip_server_mapped_group_codes = ilsauthen_map_user_role();
  $drupal_roles = user_roles(TRUE);
  // Finicky logic here searches for patron codes and sets variable to role
  // name if found.
  foreach ($drupal_roles as $drupal_role) {
    if (isset($sip_server_mapped_group_codes[$drupal_role])) {
      $search_roles = array_search($patron_code, $sip_server_mapped_group_codes[$drupal_role]);
      if ($search_roles !== FALSE) {
        $found_role = $drupal_role;
      }
    }
  }
  if (!empty($found_role)) {
    $rid = user_role_load_by_name($found_role);
    $role[$rid->rid] = $found_role;
    return $role;

  }
  // Else if role not mapped still return true as username/password check should
  // already have been performed before this function is called.
  else {
    return TRUE;
  }
}
