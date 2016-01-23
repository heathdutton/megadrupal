<?php
/**
 * @file
 * Main pages for the Deezer module.
 */

/**
 * Main settings form.
 */
function deezer_form_settings_main() {
  $form = array();

  $form['deezer_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Application ID'),
    '#description' => t('Please insert your Deezer Application ID here.'),
    '#default_value' => DEEZER_APP_ID,
  );
  $form['deezer_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret Key'),
    '#description' => t('Please insert your Deezer Secret Key here.'),
    '#default_value' => DEEZER_APP_SECRET,
  );

  return system_settings_form($form);
}

/**
 * Callback for Deezer tab in user profile.
 */
function deezer_user_profile_form($form, &$form_state, $user = NULL) {
  $form = array();

  if (!empty($user) && $existed = deezer_load_user($user->uid)) {
    // Looking for an existing record.
    // @TODO Maybe cache results to prevent extra HTTP-requests?
    $deezer_user = deezer_remote_user_get($existed['deezer_id']);
    $form_state['#user'] = $user;

    $form['deezer'] = array(
      '#type' => 'fieldset',
      '#title' => t('Linked Account'),
      '#collapsible' => FALSE,
    );

    // Deezer userpic and name.
    $form['deezer']['info'] = array(
      '#type' => 'container',
    );
    $form['deezer']['info']['userpic'] = array(
      '#markup' => theme('image', array(
        'path'   => $deezer_user['picture'],
        'alt'    => t('Deezer user picture'),
        'height' => DEEZER_USERPIC_SIDE,
        'width'  => DEEZER_USERPIC_SIDE,
      )),
    );
    $form['deezer']['info']['name'] = array(
      '#markup' => '<br /><b>' . filter_xss($deezer_user['name']) . '</b>',
    );

    $form['unbind'] = array(
      '#type' => 'submit',
      '#value' => t('Unbind'),
      '#submit' => array('deezer_user_unbind_submit'),
    );
  }
  else {
    // Ask user to login via Deezer.
    $form['message'] = array(
      '#markup' => t('Deezer integration was not yet configured. <br />'),
    );
    $form['login'] = array(
      '#theme' => 'deezer_login_popup',
    );
  }

  return $form;
}

/**
 * That's what happens when user hits the login button.
 */
function deezer_login_callback() {

  // Respect registration settings.
  $user_reg_mode = variable_get('user_register', USER_REGISTER_VISITORS);
  if ($user_reg_mode === USER_REGISTER_ADMINISTRATORS_ONLY) {
    drupal_access_denied();
    return FALSE;
  }

  // Get an access token and perform login.
  if ($access_token = deezer_session_get_token()) {
    $deezer_user = _deezer_active_user_info_get($access_token);

    // Looks like our access token became deprecated.
    if (isset($deezer_user['error'])) {
      $access_token = deezer_session_get_token(TRUE);
      $deezer_user = _deezer_active_user_info_get($access_token);
    }

    $existed = deezer_load_user(NULL, $deezer_user['id']);
    $text    = NULL;
    $type    = 'status';

    // For anonymous.
    if (user_is_anonymous()) {
      if ($existed) {
        // Found an existed record.
        $params = array('uid' => $existed['uid']);
        user_login_submit(array(), $params);
        $text = t('Welcome back!');
      }
      else {
        // The user is new. Redirect him to the register form.
        $_SESSION['deezer_user'] = $deezer_user;
        $_SESSION['deezer_goto'] = 'deezer/register';
        drupal_session_start();
      }
    }
    // For logged in users.
    else {
      if ($existed) {
        // @TODO Need a plan if Deezer account is already attached
        // to another user.
        if ($GLOBALS['user']->uid !== $existed['uid']) {
          $text = t('This Deezer account is already registered.');
          $type = 'warning';
        }
      }
      else {
        deezer_record_save($deezer_user);
        $text = t('Deezeer account successfully attached to your profile.');
      }
    }

    if (!empty($text)) {
      drupal_set_message($text, $type);
    }
  }
  else {
    // Whoops! It's time to call 911.
    watchdog('deezer', 'Error parsing access token.', array(), WATCHDOG_ERROR);
  }
}

/**
 * Generates register form for a newcomer user with required profile field.
 */
function deezer_register_form() {
  global $user;

  $form        = $form_state = array();
  $deezer_user = $_SESSION['deezer_user'];

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#maxlength' => USERNAME_MAX_LENGTH,
    '#default_value' => 'deezer-' . $deezer_user['id'],
    '#description' => t('Spaces are allowed; punctuation is not allowed except for periods, hyphens, apostrophes, and underscores.'),
    '#required' => TRUE,
    '#attributes' => array('class' => array('username')),
  );

  // Attach field widgets, and hide the ones where the 'user_register_form'
  // setting is not on.
  $langcode = entity_language('user', $user);
  field_attach_form('user', $user, $form, $form_state, $langcode);
  foreach (field_info_instances('user', 'user') as $field_name => $instance) {
    if (empty($instance['settings']['user_register_form'])) {
      $form[$field_name]['#access'] = FALSE;
    }
  }

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Register'),
    '#weight' => 99,
  );

  $form['#validate'][] = 'deezer_register_form_validate';
  $form['#validate'][] = 'user_register_validate';

  return $form;
}

/**
 * Validate callback for register form.
 */
function deezer_register_form_validate($form, &$form_state) {
  // Perform core validation of username.
  if (isset($form_state['values']['name'])) {
    if ($error = user_validate_name($form_state['values']['name'])) {
      form_set_error('name', $error);
    }
    else {
      $name_taken = (bool) db_select('users')
        ->fields('users', array('uid'))
        ->condition('name', db_like($form_state['values']['name']), 'LIKE')
        ->range(0, 1)
        ->execute()
        ->fetchField();

      if ($name_taken) {
        form_set_error('name', t('The name %name is already taken.', array('%name' => $form_state['values']['name'])));
      }
    }
  }
}

/**
 * Submit callback for register form.
 */
function deezer_register_form_submit($form, &$form_state) {
  $deezer_user = $_SESSION['deezer_user'];

  // Destroy the Deezer user data, to be sure a user will have to re-login
  // again after he submitted the deezer_register_form() form.
  unset($_SESSION['deezer_user']);

  $uid    = deezer_record_save($deezer_user, $form, $form_state);
  $params = array('uid' => $uid);

  user_login_submit(array(), $params);
  drupal_set_message(t('Thanks for login via Deezer'));
  drupal_goto('<front>');
}

/**
 * Handles the 'Unbind' button on the Deezer form.
 *
 * @see deezer_user_profile_form()
 */
function deezer_user_unbind_submit($form, &$form_state) {
  $user = $form_state['#user'];
  $uri  = 'user/' . $user->uid . '/edit/deezer/delete';

  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }

  $form_state['redirect'] = array($uri, array('query' => $destination));
}

/**
 * Form constructor for the Deezer record deletion confirmation form.
 *
 * @see deezer_user_unbind_submit()
 */
function deezer_user_unbind_confirm($form, &$form_state, $user) {
  if ($existed = deezer_load_user($user->uid)) {
    $form['uid']       = array('#type' => 'value', '#value' => $user->uid);
    $form['dependent'] = array('#type' => 'value', '#value' => $existed['dependent']);

    $path = 'user/' . $user->uid . '/edit/deezer';

    if ($existed['dependent'] == 1) {
      $ques = t('Are you sure you want to delete your account on this site?');
      $desc = t('This action cannot be undone.');
    }
    else {
      $ques = t('Are you sure you want to unbind your Deezer account?');
      $desc = t('To use it again, you will need to re-login.');
    }

    return confirm_form($form, $ques, $path, $desc, t('Unbind'), t('Cancel'));
  }

  // There is no Deezer account record attached to this user.
  drupal_goto('<front>');
}

/**
 * Executes node deletion.
 *
 * @see node_delete_confirm()
 */
function deezer_user_unbind_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $uid = $form_state['values']['uid'];
    $dependent = $form_state['values']['dependent'];

    deezer_delete_user($uid);

    if ($dependent == 1) {
      user_delete($uid);
      $form_state['redirect'] = '<front>';
    }
    else {
      drupal_set_message(t('Your Deezer account was successfully undone!'));
      $form_state['redirect'] = 'user/' . $uid;
    }
  }
}
