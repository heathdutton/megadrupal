<?php
/**
 * @file
 * Page callbacks for access_code module.
 */

/**
 * Form callback for access code login form.
 */
function access_code_login($form, &$form_state, $page = FALSE) {
  global $user;

  if ($page && $user->uid) {
    drupal_goto('user/' . $user->uid);
  }

  $form['access_code'] = array(
    '#type' => 'password',
    '#title' => t('Access code'),
    '#description' => t('Enter your access code to log in.'),
    '#size' => 20,
    '#maxlength' => 20,
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Log in'));

  return $form;
}

/**
 * Validation function for the access code login form.
 */
function access_code_login_validate($form, &$form_state) {
  $uid = access_code_validate_code($form_state['values']['access_code']);

  if (!$uid) {
    $form_state['#uid'] = 0;
    form_set_error('access_code', t('Invalid access code.'));
  }
  else {
    $form_state['#uid'] = $uid;
  }
}

/**
 * Submit function for the access code login form.
 */
function access_code_login_submit($form, &$form_state) {
  $login_form_state['uid'] = $form_state['#uid'];
  user_login_submit(array(), $login_form_state);
}

/**
 * Page callback to handle instant login when the access code is supplied in the url.
 *
 * @param $code
 *   Access code.
 */
function access_code_authenticate($code) {
  global $user;

  $uid = access_code_validate_code($code);

  if ($uid) {
    $login_form_state['uid'] = $uid;
    user_login_submit(array(), $login_form_state);

    drupal_set_message(t('You have been logged in as %name.', array('%name' => $user->name)));
    drupal_goto('user/' . $uid);
  }
  else {
    return MENU_NOT_FOUND;
  }
}
