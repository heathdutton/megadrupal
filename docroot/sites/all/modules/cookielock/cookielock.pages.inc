<?php

/**
 * @file
 * Page callbacks for the Cookie Lock module
 */

/**
 * Page callback: Cookie Lock settings
 *
 * @see cookielock_menu()
 */
function cookielock_main_page() {
  $form = drupal_get_form('cookielock_authenticate_form');
  print theme('cookielock_page', array('content' => render($form)));
  module_invoke_all('exit');
  exit();
}

/**
 * Authentication form
 */
function cookielock_authenticate_form() {
  $form = array();

  $form['header'] = array(
    '#markup' => t('<p>This site is currently locked.</p><p>Please enter valid credentials to continue.</p>'),
  );

  // Trick chrome into disabling autofill on the legit username/password fields below.
  // See http://stackoverflow.com/a/22694173/1023773
  $form['noautocomplete_user'] = array(
    '#type' => 'textfield',
    // Setting a bogus default prevents user's autofilled username (and its password) from being POSTed
    '#default_value' => 'S0m3th!NGaUSERwillNEVERENTER!?(*#',
    '#attributes' => array('style' => 'display: none;'),
  );

  $form['noautocomplete_pass'] = array(
    '#type' => 'password',
    '#attributes' => array('style' => 'display: none;'),
  );

  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#size' => 25,
    '#maxlength' => 25,
    '#required' => TRUE,
  );

  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#size' => 25,
    '#maxlength' => 25,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Form submit callback for the authentication form
 */
function cookielock_authenticate_form_submit($form, &$form_state) {
  // If their password matches the one stored in the DB, set the cookie allowing access
  $stored_user = variable_get('cookielock_username', 'S0m3th!NGaUSERwillNEVERENTER!?(*#');
  $stored_pass = variable_get('cookielock_password', 'md5willnevergeneratethisstring');
  $md5pass = md5($form_state['values']['password']);
  if ($form_state['values']['username'] == $stored_user && $md5pass == $stored_pass) {
    user_cookie_save(array(
      'cookielock_username' => $form_state['values']['username'],
      'cookielock_password' => $md5pass,
    ));
  }
  else {
    drupal_set_message(t('Access denied'), 'error');
  }
}