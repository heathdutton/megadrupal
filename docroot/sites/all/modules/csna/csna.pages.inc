<?php

/**
 * @file
 * Menu callbacks for the CSNA module.
 */

/**
 * Menu callback: Set email address and password after successful user login
 */
function csna_complete_register_form($form, &$form_state, $account) {
  global $user;

  if ($account->mail) {
    drupal_goto('user');
  }

  $form_state['user'] = $account;

  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#maxlength' => EMAIL_MAX_LENGTH,
    '#description' => t('A valid e-mail address. All e-mails from the system will be sent to this address. The e-mail address is not made public and will only be used if you wish to receive a new password or wish to receive certain news or notifications by e-mail.'),
    '#required' => TRUE,
  );

  $form['pass'] = array(
    '#type' => 'password_confirm',
    '#size' => 25,
    '#description' => t('Provide a password for the new account in both fields.'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function csna_complete_register_form_validate($form, &$form_state) {
  $account = $form_state['user'];

  // Trim whitespace from mail, to prevent confusing 'e-mail not valid'
  // warnings often caused by cutting and pasting.
  $mail = trim($form_state['values']['mail']);
  form_set_value($form['mail'], $mail, $form_state);

  // Validate the e-mail address, and check if it is taken by an existing user.
  if ($error = user_validate_mail($form_state['values']['mail'])) {
    form_set_error('mail', $error);
  }
  elseif ((bool) db_select('users')->fields('users', array('uid'))->condition('uid', $account->uid, '<>')->condition('mail', db_like($form_state['values']['mail']), 'LIKE')->range(0, 1)->execute()->fetchField()) {
    form_set_error('mail', t('The e-mail address %email is already taken.', array('%email' => $form_state['values']['mail'])));
  }
}

function csna_complete_register_form_submit($form, &$form_state) {
  $account = $form_state['user'];

  $edit = array(
    'mail' => $form_state['values']['mail'],
    'pass' => $form_state['values']['pass'],
  );

  user_save($account, $edit);
}

