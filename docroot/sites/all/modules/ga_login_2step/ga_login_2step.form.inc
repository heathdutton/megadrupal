<?php

/**
 * @file
 * Contains the Google Authenticator 2 step login form.
 */

/**
 * Start of GA Login process form.
 *
 * @param object $account
 *   User account.
 */
function ga_login_2step_begin_form($account) {
  return drupal_get_form('ga_login_2step_form', $account);
}

/**
 * Main GA Login process form builder.
 */
function ga_login_2step_form($form, $form_state, $account) {
  $form['gacode'] = array(
    '#type' => 'gacode',
    '#title' => t('GA Code'),
    '#required' => FALSE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['name'] = array('#type' => 'value', '#value' => $account->name);
  $form['uid'] = array('#type' => 'value', '#value' => $account->uid);
  $form['account'] = array('#type' => 'value', '#value' => $account);
  $form['#validate'] = ['ga_login_2step_form_validate', 'ga_login_user_login_validate'];
  $form['#submit'] = ['user_login_submit' , 'ga_login_user_login_submit_code_needed'];

  return $form;
}

/**
 * Validate handler of GA login 2 step form.
 *
 * @see ga_login_2step_form
 */
function ga_login_2step_form_validate($form, &$form_state) {
  $form_state['uid'] = $form_state['values']['uid'];
}
