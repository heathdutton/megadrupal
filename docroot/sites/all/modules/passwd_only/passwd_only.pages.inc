<?php

/**
 * @file
 * Form functions for the admin area.
 */

/**
 * Settings form to set the PassWd only user.
 *
 * @see passwd_only_admin_submit()
 */
function passwd_only_admin() {

  $form = array();

  $form['passwd_only_user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Select user'),
    '#description' => t('Select user to use to login in PassWd only login forms.'),
    '#size' => 60,
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => variable_get('passwd_only_user_name', ''),
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['passwd_only_block_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('This description text is shown in the Passwd only login block.'),
    '#default_value' => variable_get('passwd_only_block_description', ''),
  );

  $form['#validate'][] = 'passwd_only_admin_validate';
  $form['#submit'][] = 'passwd_only_admin_submit';

  return system_settings_form($form);
}

/**
 * Validate callback for passwd_only_admin().
 *
 * @see passwd_only_admin()
 */
function passwd_only_admin_validate($form, &$form_state) {
  $user_name = $form_state['values']['passwd_only_user_name'];
  $account = user_load_by_name($user_name);
  if ($account->uid == 1) {
   form_set_error('passwd_only_user_name', t('"@name" is the root user account (User-ID 1). It is not secure to use this account with PassWd only. Please select another user account.', array('@name' => format_username($account))));
  }
}

/**
 * Submit callback for passwd_only_admin().
 *
 * @see passwd_only_admin()
 */
function passwd_only_admin_submit($form, &$form_state) {
  $user_name = $form_state['values']['passwd_only_user_name'];

  if (!empty($user_name)) {
    if ($account = user_load_by_name($user_name)) {
      variable_set('passwd_only_user', $account->uid);
    }
  }
  else {
    variable_set('passwd_only_user', '');
  }
}
