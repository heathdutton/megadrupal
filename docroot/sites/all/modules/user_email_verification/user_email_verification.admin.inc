<?php

/**
 * Menu callback; process one time login link and redirects to the user page on success.
 */
function user_email_verification_verify($form, &$form_state, $uid, $timestamp, $hashed_pass) {
  // Time out, in seconds, until login URL expires. Defaults to 24 hours =
  // 86400 seconds.
  $timeout = variable_get('user_email_verification_timeout', 86400);
  $current = REQUEST_TIME;
  // Some redundant checks for extra security ?
  $users = user_load_multiple(array($uid), array('status' => '1'));
  if ($timestamp <= $current && $account = reset($users)) {
    // No time out for first time login.
    if ($current - $timestamp > $timeout) {
      drupal_set_message(t('You have tried to use a one-time verify link that has expired. Please request a new one using the form below.'));
      drupal_goto('user/password');
    }
    elseif ($account->uid) {
      $verified = user_email_verification_load_verify_flag($account->uid);
      if (!$verified && ($hashed_pass == user_email_verification_hmac($account->uid, $timestamp))) {
        db_update('user_email_verification')
        ->fields(array(
        'verified' => 1,
        ))
        ->condition('uid', $account->uid, '=')
        ->execute();

        drupal_set_message(t('Thank you for verifing your e-mail address.'));

        if ($account->status == 0) {
          global $language;
          drupal_mail('user_email_verification', 'verify_blocked', variable_get('site_mail', ''), $language, array('account' => $account));
          drupal_set_message(t('Your account has been blocked before the verification of the e-mail. ' .
            'An administrator will make an audit and unblock your account if the reason for the blocking was the e-mail verification.'));
        }

        drupal_goto('user');
      }
      else {
        drupal_set_message(t('You have tried to use a one-time verify link that has either been used or is no longer valid. Please request a new one using the form below.'));
        drupal_goto('user/verify');
      }
    }
    else {
      drupal_set_message(t('You have tried to use a one-time verify link that has either been used or is no longer valid. Please request a new one using the form below.'));
      drupal_goto('user/verify');
    }
  }
  else {
    // Deny access, no more clues.
    // Everything will be in the watchdog's URL for the administrator to check.
    drupal_access_denied();
    drupal_exit();
  }
}

/**
 * Form builder; Request a email verification.
 *
 * @ingroup forms
 * @see user_email_verification_request_validate()
 * @see user_email_verification_request_submit()
 */
function user_email_verification_request() {
  global $user;

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username or e-mail address'),
    '#size' => 60,
    '#maxlength' => max(USERNAME_MAX_LENGTH, EMAIL_MAX_LENGTH),
    '#required' => TRUE,
    '#default_value' => isset($_GET['name']) ? $_GET['name'] : '',
  );
  // Allow logged in users to request this also.
  if ($user->uid > 0) {
    $form['name']['#type'] = 'value';
    $form['name']['#value'] = $user->mail;
    $form['mail'] = array(
      '#prefix' => '<p>',
      '#markup' =>  t('Verify email will be send to %email.', array('%email' => $user->mail)),
      '#suffix' => '</p>',
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Send verify mail'));

  return $form;
}

/**
 *
 * @param unknown $form
 * @param unknown $form_state
 */
function user_email_verification_request_validate($form, &$form_state) {
  $name = trim($form_state['values']['name']);
  // Try to load by email.
  $users = user_load_multiple(array(), array('mail' => $name, 'status' => '1'));
  $account = reset($users);
  if (!$account) {
    // No success, try to load by name.
    $users = user_load_multiple(array(), array('name' => $name, 'status' => '1'));
    $account = reset($users);
  }
  if (isset($account->uid)) {
    form_set_value(array('#parents' => array('account')), $account, $form_state);
  }
  else {
    form_set_error('name', t('Sorry, %name is not recognized as a user name or an e-mail address.', array('%name' => $name)));
  }
}

/**
 *
 * @param unknown $form
 * @param unknown $form_state
 */
function user_email_verification_request_submit($form, &$form_state) {
  global $language;

  $account = $form_state['values']['account'];
  // Mail one time login URL and instructions using current language.
  $mail = user_email_verification_mail_notify('verify', $account, $language);
  if (!empty($mail)) {
    watchdog('user', 'Verification instructions mailed to %name at %email.', array('%name' => $account->name, '%email' => $account->mail));
    drupal_set_message(t('Further instructions have been sent to your e-mail address.'));
  }

  $form_state['redirect'] = 'user';
  return;
}