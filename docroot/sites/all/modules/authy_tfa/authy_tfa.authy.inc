<?php
/**
 * @file
 * Module file for Authy TFA
 */

/**
 * Validate the authy token through the API
 *
 * @param int $uid
 *   Drupal user id
 * @param string $token
 *   Token entered by the user
 *
 * @return bool
 *   True if valid else false
 */
function _authy_tfa_validate_token($uid, $token) {

  $authy_api = _authy_tfa_get_api();

  if (is_object($authy_api)) {

    $authy = _authy_tfa_authy_get($uid);
    $verification = $authy_api->verifyToken($authy->authy_id, $token);

    if ($verification->ok()) {
      return TRUE;
    }

    $error = $verification->errors()->message;
    watchdog('authy', "Authy responded with an error for uid @uid using token @token: %error",
      array('@uid' => $uid, '@token' => $token, '%error' => $error), WATCHDOG_ERROR);
  }

  return FALSE;
}

/**
 * Send an alternate code.
 *
 * @param int $uid
 *   Drupal user id
 * @param string $type
 *   sms or phonecall
 */
function authy_tfa_alternate_code($uid, $type) {

  if (isset($_SESSION['authy_uid'])) {

    $authy_user = _authy_tfa_authy_get($_SESSION['authy_uid']);
    $authy_api = _authy_tfa_get_api();
    if (is_object($authy_api)) {

      switch ($type) {
        case 'sms':
          $verification = $authy_api->requestSms($authy_user->authy_id, array('force' => 'true'));
          break;
        case 'phonecall':
          $verification = $authy_api->phoneCall($authy_user->authy_id, array('force' => 'true'));
          break;
      }

      if ($verification->ok()) {
        return TRUE;
      }
      else {
        $errors = $verification->errors();
        watchdog('authy', 'Authy Errors: %errors', array('%errors' => $errors->message), WATCHDOG_ERROR);
      }
    }
  }

  return FALSE;
}


/**
 * Implements hook_form().
 *
 * Builds the authy form for submitting the token.
 */
function authy_tfa_authy_form($form, &$form_state) {

  global $user;

  // during this process we reset the user as anonymous but save the session.
  // Here we re-load the user to make sure they have proper access permissions
  // to this form.
  if ($user->uid === 0 && isset($_SESSION['authy_uid'])) {
    $user = user_load($_SESSION['authy_uid']);
  }

  if (($authy_user = _authy_tfa_authy_get($user->uid)) == FALSE) {
    $_SESSION['authy_uid'] = $user->uid;
    drupal_goto('authy_tfa/cellphone');
  }

  // The user should not be here if they already have completed the
  // authentication process.
  if (isset($_SESSION['authy_tfa'])) {
    drupal_goto('<front>');
  }

  // reset the users session as anonymous until they pass this last
  // authentication test.
  $user = drupal_anonymous_user();

  $_SESSION['authy_uid'] = $authy_user->uid;

  $form['#attached']['js'][] = array(
    'data' => '//cdnjs.cloudflare.com/ajax/libs/authy-forms.js/2.0/form.authy.min.js',
    'type' => 'external',
  );

  $form['#attached']['css'][] = array(
    'data' => '//cdnjs.cloudflare.com/ajax/libs/authy-forms.css/2.0/form.authy.min.css',
    'type' => 'external',
  );

  $form['authy-token'] = array(
    '#type' => 'textfield',
    '#title' => t('Authy Token'),
    '#id' => 'authy-token',
    '#attributes' => array('autocomplete' => 'off'),
  );

  $form['response'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="response">',
    '#suffix' => '</div>',
  );

  $display_sms = variable_get('authy_display_sms', 0);
  $display_phonecall = variable_get('authy_display_phonecall', 0);

  if ($display_sms || $display_phonecall) {
    $form['alternate']['action'] = array(
      '#type' => 'fieldset',
      '#title' => t('Alternate method'),
      '#description' => t('If you do not have the Authy app installed on your phone, you can request an alternate method to get your code.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
  }

  if ($display_sms) {
    $form['alternate']['action']['sms'] = array(
      '#value' => t('Send SMS'),
      '#type' => 'button',
        '#ajax' => array(
        'callback' => 'authy_tfa_ajax_submit_alternate_code',
        'wrapper' => 'response',
      ),
    );
  }

  if ($display_phonecall) {
    $form['alternate']['action']['phonecall'] = array(
      '#value' => t('Request phonecall'),
      '#type' => 'button',
      '#ajax' => array(
        'callback' => 'authy_tfa_ajax_submit_alternate_code',
        'wrapper' => 'response',
      ),
    );
  }

  $form['action']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Sign in'),
  );

  $form['action']['cancel'] = array(
    '#markup' => l(t('Cancel'), '<front>'),
    '#attributes' => array('class' => array('cancel-button')),
  );

  return $form;
}

/**
 * Implements hook_form().
 *
 * Add a form to allow someone to enter their cellphone information.
 */
function authy_tfa_cellphone_entry_form($form, &$form_state) {

  global $user;
  $authy_user = _authy_tfa_authy_get($user->uid);

  $form['#attached']['js'][] = array(
    'data' => '//cdnjs.cloudflare.com/ajax/libs/authy-forms.js/2.0/form.authy.min.js',
    'type' => 'external',
  );

  $form['#attached']['css'][] = array(
    'data' => '//cdnjs.cloudflare.com/ajax/libs/authy-forms.css/2.0/form.authy.min.css',
    'type' => 'external',
  );

  $form['authy_tfa']['authy_country'] = array(
    '#type' => 'textfield',
    '#title' => t('Country Code'),
    '#description' => t('For the US or Canada this is +1'),
    '#default_value' => isset($authy_user->country_code) ? $authy_user->country_code : '',
    '#id' => 'authy-countries',
  );

  $form['authy_tfa']['authy_cellnumber'] = array(
    '#type' => 'textfield',
    '#title' => t('Cellphone number'),
    '#default_value' => isset($authy_user->cellphone) ? $authy_user->cellphone : '',
    '#id' => 'authy-cellphone',
  );

  $form['authy_tfa']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save authy details'),
  );

  // handle this form.
  $form['#submit'][] = 'authy_tfa_form_user_profile_submit';
  $form['#validate'][] = 'authy_tfa_form_user_profile_validate';

  return $form;
}

/**
 * authy_tfa_ajax_submit_alternate_code().
 *
 * Implements hook_form().
 *
 * This is the ajax callback for sending an SMS message.
 */
function authy_tfa_ajax_submit_alternate_code($form, $form_state) {

  if (isset($_SESSION['authy_uid'])) {
    $type = $form_state['clicked_button']['#parents'][0];

    if (authy_tfa_alternate_code($_SESSION['authy_uid'], $type)) {
      return t('Your request for a %type has been sent.', array('%type' => $type));
    }
    else {
      return t('There was an error. Please contact an administrator.');
    }
  }

}


/**
 * Implements hook_submit().
 *
 * Test to see if the given token is valid.
 */
function authy_tfa_authy_form_submit($form, &$form_state) {
  global $user;

  // Check to see if the users uid was saved in the session table. If so we
  // need to reload that user so we can ensure this user has access to submit
  // the form.
  if (isset($_SESSION['authy_uid'])) {
    $user = user_load($_SESSION['authy_uid']);
    unset($_SESSION['authy_uid']);
  }

  // set the session value and move on.
  if (_authy_tfa_validate_token($user->uid, $form_state['values']['authy-token'])) {
    $_SESSION['authy_tfa'] = time();
    $form_state['redirect'] = '<front>';
  }
  else {
    drupal_set_message(t('You entered an invalid Authy token.'), 'error');
  }
}

/**
 * Fetch the authy object.
 *
 * Include the authy library files and instanciate the authy object for use in
 * calls to the authy service.
 *
 * We do some checking to see if the api key has been setup proplerly and
 * return FALSE if it hasn't. Return FALSE if the libraries aren't loaded.
 *
 * @return Authy_API object or FALSE
 */
function _authy_tfa_get_api() {

  if (($library = libraries_load('authy-php')) && !empty($library['loaded'])) {

    // Do some simple verification of the API key.
    if (!_authy_check_config()) {
      watchdog('authy', "Invalid Authy API key provided in module configuration.", array(), WATCHDOG_ERROR);
      return FALSE;
    }

    $key = variable_get('authy_api_key', '');

    $host = variable_get('authy_host_uri', 'https://api.authy.com');
    $authy_api =  new Authy_Api($key, $host);
    return $authy_api;
  }
  else {
    drupal_set_message(t("There was a system error. Please contact the administrator."));
    watchdog('authy', "Unable to load authy-php library.", array(), WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Add the user to Authy and require two factor.
 *
 * @param array $account
 *   Keyed array containing the following fields to add a user to the authy
 *   service.
 *   array(
 *     'uid' => Drupal user id
 *     'country_code' => int Country code
 *     'cellphone' => int Cellphone number
 *     'use_authy' => int 1 = Yes, 0 = No
 *   );
 *
 * @return int $authy_id or FALSE on failure.
 */
function authy_tfa_authy_user_add($account) {

  $authy_api = _authy_tfa_get_api();

  if (is_object($authy_api)) {

    $account2 = user_load($account['uid']);

    $authy_user = $authy_api->registerUser($account2->mail, $account['cellphone'], $account['country_code']);

    if ($authy_user->ok()) {
      return $authy_user->id();
    }
    else {
      $errors = $authy_user->errors();
      watchdog('authy', 'Authy Errors: %errors', array('%errors' => $errors->message), WATCHDOG_ERROR);
    }
  }

  return FALSE;
}

/**
 * Remove the user from Authy and from the local db.
 *
 * @param $account
 *   The account that is being deleted.
 *
 * @return bool
 *   TRUE if success else FALSE.
 */
function authy_tfa_authy_user_remove($account) {

  $authy_api = _authy_tfa_get_api();
  if (is_object($authy_api)) {

    $authy_user = _authy_tfa_authy_get($account['uid']);

    $result = $authy_api->deleteUser($authy_user->authy_id);

    if ($result->ok()) {
      // clean up the database
      db_delete('authy_user', 'a')
        ->condition('authy_id', $authy_user->authy_id)
        ->execute();

      return TRUE;
    }
    else {
      $errors = $result->errors();
      watchdog('authy', 'Authy Errors: %errors', array('%errors' => $errors->message), WATCHDOG_ERROR);
    }
  }

  return FALSE;
}

/**
 * Save the users authy information locally to the database.
 * If the user id is not in the local database already, the information is
 * sent using the API to Authy and the resulting data is added to the local
 * db.
 *
 * @param int $uid
 *   Drupal user id.
 * @param bool $use_authy
 *   Use authy or not.
 * @param int $country_code
 *   Users country code (US = 1)
 * @param int $cellphone
 */
function _authy_tfa_authy_save($uid, $use_authy, $country_code, $cellphone) {

  // check to see if the user is already in the db
  $authy_user = _authy_tfa_authy_get($uid);
  if (is_object($authy_user) && isset($authy_user->use_authy)) {
    // the user is currently allowed to update whether they are using it or not.
    if ($authy_user->use_authy != $use_authy) {
      db_update('authy_user')
        ->fields(array('use_authy' => $use_authy))
        ->condition('uid', $uid)
        ->execute();
    }
  }
  else {
    $account = array(
      'uid' => $uid,
      'country_code' => $country_code,
      'cellphone' => $cellphone,
      'use_authy' => (int) $use_authy,
    );

    if ($authy_id = authy_tfa_authy_user_add($account)) {
      $account['authy_id'] = $authy_id;

      db_insert('authy_user')
        ->fields( $account )
        ->execute();
    }
  }

  $_SESSION['authy_tfa'] = time();
}

/**
 * Retreive the users information from the authy table.
 *
 * @param $uid
 *   Users id.
 *
 * @return $authy_user
 *   Authy user object.
 */
function _authy_tfa_authy_get($uid) {
  $authy_user = db_query('SELECT * FROM {authy_user} WHERE uid = :uid', array(':uid' => $uid))->fetchObject();
  return $authy_user;
}

/**
 * Check if this user requires the use of Authy to log in.
 *
 * @param object $account
 *   Drupal user object.
 *
 * @return bool
 */
function authy_tfa_user_require_tfa($account) {
  $role_required = FALSE;

  // Allow the administrator to not have Authy required.
  if ($account->uid == 1) {
    $perms = user_role_permissions($account->roles);
    foreach ($perms as $perm) {
      if (in_array('require authy_tfa', array_keys($perm))) {
        $role_required = TRUE;
        break;
      }
    }
  }
  else {
    $role_required = user_access('require authy_tfa', $account);
  }

  if ($role_required) {
    return TRUE;
  }

  $authy_user = _authy_tfa_authy_get($account->uid);
  if (isset($authy_user->use_authy) && $authy_user->use_authy) {
    return TRUE;
  }

  return FALSE;
}
