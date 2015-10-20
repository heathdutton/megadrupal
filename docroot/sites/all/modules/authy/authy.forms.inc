<?php
/**
 * @file
 * Module that implements two factor authentication using Authy.
 */

/**
 * Form callback: authy_form_verify().
 *
 * Request a Authy token from a user.
 */
function authy_form_verify($form, &$form_state) {

  $form['#attached']['css'] = array(
    'https://www.authy.com/form.authy.min.css' => array(
      'type' => 'external',
    ),
  );

  $form['authy_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Token'),
    '#attributes' => array('id' => array('authy-token')),
    '#description' => t('Enter your Authy token for %application. <a href="!request">Request SMS token</a>.', array(
      '!request' => url('authy/request', array(
      'query' => array('goto' => current_path()),
      )),
      '%application' => variable_get('authy_application', ''),
    )),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Validation function for authy_form_verify().
 */
function authy_form_verify_validate($form, &$form_state) {

  // We need the username that is logging in to load information about
  // Authy settings.
  if (isset($_SESSION['authy']['form_state']['values']['name'])) {
    $form_state['values']['name'] = $_SESSION['authy']['form_state']['values']['name'];
  }

  // We just call the authy_verify() we use for all forms since it's
  // written to work with anything.
  return authy_verify($form, $form_state);
}

/**
 * Submit function for authy_form_verify().
 */
function authy_form_verify_submit($form, &$form_state) {
  global $user;

  // Use the saved form state to pass to user_login_submit().
  // This will log in the user.
  user_login_submit($_SESSION['authy']['form'], $_SESSION['authy']['form_state']);

  module_invoke('rules', 'invoke_event', 'authy_login', $user);

  // And redirect.
  if (isset($_SESSION['authy']['destination'])) {
    $form_state['redirect'] = $_SESSION['authy']['destination'];
  }
  else {
    $form_state['redirect'] = 'user';
  }

  // We don't need this anymore.
  unset($_SESSION['authy']);
}

/**
 * Form callback: authy_form_enable().
 *
 * Enable Authy authentication for a user.
 */
function authy_form_enable($form, &$form_state, $user) {
  $form['#uid'] = $user->uid;

  $form['#attached']['css'] = array(
    'https://www.authy.com/form.authy.min.css' => array(
      'type' => 'external',
    ),
  );

  $form['#attached']['js'] = array(
    'https://www.authy.com/form.authy.min.js' => array(
      'type' => 'external',
    ),
  );

  $form['enable_authy'] = array(
    '#title' => t('Enable Authy'),
    '#type' => 'textfield',
    '#description' => t('To enable Authy enter your %application token from your cellphone.', array('%application' => variable_get('authy_application', ''))),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Enable'),
  );

  return $form;
}

/**
 * Validation function for authy_form_enable().
 */
function authy_form_enable_validate($form, &$form_state) {
  $authy_id = _authy_get_authy_id($form['#uid'], TRUE);

  $authy = _authy_get_api();
  $verification = $authy->verify($authy_id, $form_state['values']['enable_authy']);

  if($verification === FALSE) {
    form_set_error('enable_authy', t('The token you entered could not be verified.'));
  }
}

/**
 * Submit function for authy_form_enable().
 */
function authy_form_enable_submit($form, &$form_state) {
  db_update('authy')
    ->fields(array(
      'active' => 1,
    ))
    ->condition('uid', $form['#uid'], '=')
    ->execute();

  module_invoke('rules', 'invoke_event', 'authy_enabled', $form['#uid']);
  drupal_set_message(t('Authy is now enabled for your account.'));
}

/**
 * Form callback: authy_form_disable().
 *
 * Disable Authy authentication for a user.
 */
function authy_form_disable($form, &$form_state, $user) {
  $form['#uid'] = $user->uid;

  $form['#attached']['css'] = array(
    'https://www.authy.com/form.authy.min.css' => array(
      'type' => 'external',
    ),
  );

  $form['#attached']['js'] = array(
    'https://www.authy.com/form.authy.min.js' => array(
      'type' => 'external',
    ),
  );

  $form['disable_authy'] = array(
    '#title' => t('Disable Authy'),
    '#type' => 'textfield',
    '#description' => t('To disable Authy enter your %application token from your cellphone. <a href="!request">Request SMS token</a>',array(
      '%application' => variable_get('authy_application', ''),
      '!request' => url('authy/request', array(
        'query' => array('goto' => current_path()),
      )),
    )),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Disable'),
  );

  return $form;
}

/**
 * Validation function for authy_form_disable().
 */
function authy_form_disable_validate($form, &$form_state) {
  $authy_id = _authy_get_authy_id($form['#uid'], TRUE);

  $authy = _authy_get_api();
  $verification = $authy->verify($authy_id, $form_state['values']['disable_authy']);

  if($verification === FALSE) {
    form_set_error('disable_authy', t('The token you entered could not be verified.'));
  }
}

/**
 * Submit function for authy_form_disable().
 */
function authy_form_disable_submit($form, &$form_state) {
  db_update('authy')
    ->fields(array(
      'active' => 0,
    ))
    ->condition('uid', $form['#uid'], '=')
    ->execute();

  module_invoke('rules', 'invoke_event', 'authy_disabled', $form['#uid']);
  drupal_set_message(t('Authy is now disabled for your account.'), 'warning');
}

/**
 * Form callback: authy_form_unregister().
 *
 * Unregister Authy ID from user.
 */
function authy_form_unregister($form, &$form_state, $user) {
  $form['#uid'] = $user->uid;
  return confirm_form($form,
    t('Are you sure you want to unregister your Authy ID?'),
    'user/'.$user->uid.'/authy',
    t('This will clear all the Authy information connected to your user on this site, however this will not delete your Authy.com account.'),
    t('Unregister ID'),
    t('Cancel')
  );
}

/**
 * Submit function for authy_form_unregister().
 */
function authy_form_unregister_submit($form, &$form_state) {
  db_delete('authy')
    ->condition('uid', $form['#uid'], '=')
    ->execute();

  module_invoke('rules', 'invoke_event', 'authy_disabled', $form['#uid']);
  $form_state['redirect'] = 'user/' . $form['#uid'] . '/authy';
}

/**
 * Form callback: authy_form_register().
 *
 * Register a Authy ID to user.
 */
function authy_form_register($form, &$form_state, $user) {
  $form['#uid'] = $user->uid;

  $form['#attached']['css'] = array(
    'https://www.authy.com/form.authy.min.css' => array(
      'type' => 'external',
    ),
  );

  $form['#attached']['js'] = array(
    'https://www.authy.com/form.authy.min.js' => array(
      'type' => 'external',
    ),
  );

  $form['newid'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register your phone'),
    '#description' => t('Enter your mobile number and your country to use your cellphone as your secure token. All the information in this form will be submitted to Authy. See !link for more details. ',
    array(
      '!link' => '<a href="http://authy.com">Authy.com</a>',
    )),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['newid']['email'] = array(
    '#title' => t('E-mail'),
    '#type' => 'textfield',
    '#default_value' => $user->mail,
    '#description' => '',
    '#required' => TRUE,
    '#disabled' => TRUE,
  );

  $form['newid']['cellphone'] = array(
    '#title' => t('Cellphone'),
    '#type' => 'textfield',
    '#attributes' => array('id' => array('authy-cellphone')),
    '#description' => '',
    '#required' => FALSE,
  );

  $form['newid']['countrycode'] = array(
    '#title' => t('Country'),
    '#type' => 'textfield',
    '#attributes' => array('id' => array('authy-countries')),
    '#description' => '',
    '#required' => FALSE,
  );

  $form['newid']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Register phone'),
  );

  return $form;
}

/**
 * Validation function for authy_form_register().
 */
function authy_form_register_validate($form, &$form_state) {
  if (!empty($form_state['values']['cellphone']) && !empty($form_state['values']['countrycode'])) {

    $authy = _authy_get_api();
    $authy_user = $authy->userNew($form_state['values']['email'], $form_state['values']['cellphone'], $form_state['values']['countrycode']);

    if ($authy_user !== FALSE) {
      $form_state['values']['authyId'] = $authy_user;
    }
    else {
      watchdog('Authy', 'Registration failed. %error', array('%error' => $authy->lastError), WATCHDOG_DEBUG);
      form_set_error('cellphone', t('Unable to verify your phonenumber. Please try again later.'));
    }
  }
}

/**
 * Submit function for authy_form_register().
 */
function authy_form_register_submit($form, &$form_state) {
  if ($form_state['values']['authyId'] > 0) {
    db_merge('authy')
      ->key(array('uid' => $form['#uid']))
      ->fields(array(
        'uid' => $form['#uid'],
        'authy_id' => $form_state['values']['authyId'],
      ))
      ->execute();

    drupal_set_message(t('You have now registerd your Authy account to this site. However Authy still needs to be activated before your account is protected.'));
  }
}

/**
 * Form callback: authy_form_request().
 *
 * Request one time token.
 */
function authy_form_request($form, &$form_state) {
  if(isset($_GET['goto']) && drupal_valid_path($_GET['goto'])) {
    $form['#goto'] = $_GET['goto'];
  }

  $form['type'] = array(
    '#title' => t('Token type'),
    '#type' => 'select',
    '#description' => t('Select how you want to receive your token'),
    '#options' => array(
      'sms' => t('SMS Message'),
    ),
    '#default_value' => 'sms',
    '#required' => TRUE,
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Request token'));

  return $form;
}

/**
 * Submit function for authy_form_request().
 */
function authy_form_request_submit($form, &$form_state) {
  global $user;

  $authy_api = _authy_get_api();

  // If we have a session variable, this means that we are in a login process.
  // Load the user by the almost logged in username.
  if (isset($_SESSION['authy']['form_state']['values']['name'])) {
    $authy_user = user_load_by_name($_SESSION['authy']['form_state']['values']['name']);
    $authy_id = _authy_get_authy_id($authy_user->uid);
  }
  else {
    $authy_id = _authy_get_authy_id($user->uid);
  }

  switch ($form_state['values']['type']) {
    case 'sms':
      $sms = $authy_api->requestSms($authy_id);
      if ($sms === TRUE) {
        drupal_set_message(t('SMS message code sent.'));
      }
      else {
        watchdog('Authy', 'SMS request failed. %error', array('%error' => $authy->lastError), WATCHDOG_ERROR);
        form_set_error('type', t('Unable to send SMS, please try again later.'));
      }
      break;
  }

  if (isset($_SESSION['authy']['destination'])) {
    $form_state['redirect'] = $_SESSION['authy']['destination'];
  }
  else {
    $form_state['redirect'] = 'user';
  }

  if(isset($form['#goto'])) {
    $form_state['redirect'] = $form['#goto'];
  }

}
