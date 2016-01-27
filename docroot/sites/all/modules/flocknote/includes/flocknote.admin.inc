<?php

/**
 * @file
 * Administration pages for Flocknote module.
 */

/**
 * Flocknote API settings form.
 */
function flocknote_admin_form($form, &$form_state) {
  $auth_info = flocknote_get_authentication_info();

  // If auth info is not correct or doesn't exist, just show warning.
  if (!$auth_info['verified']) {
    $form['description']['#markup'] = t('Your Flocknote API Authentication information is either not entered or incorrect. Please !link.', array(
      '!link' => l(t('enter your API authentication information'), 'admin/config/services/flocknote/authentication'),
    ));
    return $form;
  }

  // Add a helpful description.
  $form['description']['#markup'] = t('Configure your Flocknote integration settings below.');

  // Registration Option
  $form['registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Registration'),
    '#collapsible' => TRUE,
    '#tree' => FALSE,
  );
  $lists = _flocknote_registration_list_options();
  if (!empty($lists)) {
    $form['registration']['flocknote_registration_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t("Subscribe new site users to Flocknote list"),
      '#description' => t("Enable this setting to automatically subscribe new site users to a Flocknote list you administer. Users may opt-out of their subscription during their registration, or at any time through Flocknote."),
      '#default_value' => variable_get('flocknote_registration_enabled', 0),
    );
    $form['registration']['flocknote_registration_list_id'] = array(
      '#type' => 'select',
      '#title' => t("List to which new users will be subscribed"),
      '#description' => t("This list only includes lists for which your Flocknote account has permission to add members."),
      '#options' => $lists,
      '#default_value' => variable_get('flocknote_registration_list_id', 0),
      // Only show this element when registration enabled checkbox is checked.
      '#states' => array(
        'invisible' => array(
          ':input[name="flocknote_registration_enabled"]' => array('checked' => FALSE),
        ),
      ),
    );
  }
  else {
    $form['registration']['warning']['#markup'] = t("Your Flocknote account does not have permission to add members for any Lists.");
  }

  // Allow choice of list to which new nodes of type will be posted.
  // TODO

  return system_settings_form($form);
}

/**
 * Flocknote API authentication form.
 */
function flocknote_authentication_form($form, &$form_state) {
  $auth_info = flocknote_get_authentication_info();
  $verified = $auth_info['verified'];

  // Set description for form.
  if ($verified) {
    $description = t("Your authentication information has been verified.");
  }
  else {
    $auth_info = _flocknote_default_authentication_info();
    $description = t("Please enter your authentication information below.");
  }
  $form['description']['#markup'] = '<strong>' . $description . '</strong>';

  // Disable autocomplete for the entire form.
  $form['#attributes']['autocomplete'] = 'off';

  // App ID and Key.
  $form['app'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flocknote API App Information'),
    '#tree' => FALSE,
  );
  $form['app']['app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App ID'),
    '#description' => t("Your Flocknote API Application ID."),
    '#size' => 60,
    '#required' => TRUE,
    '#default_value' => $auth_info['app_id'],
    '#disabled' => $verified,
  );
  $form['app']['app_key'] = array(
    '#type' => 'textfield',
    '#title' => t('App Key'),
    '#description' => t("Your Flocknote API Application Key (secret)."),
    '#size' => 60,
    '#required' => TRUE,
    '#default_value' => $auth_info['app_key'],
    '#disabled' => $verified,
  );

  // User account login.
  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flocknote User Account'),
    '#tree' => FALSE,
  );
  $form['user']['user'] = array(
    '#type' => 'textfield',
    '#title' => t('Email or Phone Number'),
    '#description' => t('Your Email address or Phone number, used to login to Flocknote.'),
    '#size' => 24,
    '#required' => TRUE,
    '#default_value' => $auth_info['user'],
    '#disabled' => $verified,
  );
  $form['user']['pass'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('Your Flocknote password.'),
    '#size' => 24,
    '#required' => TRUE,
    '#default_value' => $auth_info['pass'],
    '#disabled' => $verified,
  );
  if ($verified) {
    $form['user']['pass']['#description'] .= ' <em>' . t("Your password is not shown for better security.") . '</em>';
  }

  // Submit button.
  $form['actions']['#type'] = 'container';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save app and login info'),
    '#disabled' => $verified,
  );
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset app and login info'),
    '#disabled' => !$verified,
  );

  return $form;
}

function flocknote_authentication_form_validate($form, &$form_state) {
  // If reset button is clicked, return.
  if (!empty($form_state['clicked_button']['#id']) && $form_state['clicked_button']['#id'] == 'edit-reset') {
    return;
  }

  // Get auth info from form values.
  $auth_info = array(
    'app_id' => $form_state['values']['app_id'],
    'app_key' => $form_state['values']['app_key'],
    'user' => $form_state['values']['user'],
    'pass' => $form_state['values']['pass'],
  );
  
  // If auth info is valid, save it in the form state.
  if (flocknote_verify_authentication_info($auth_info)) {
    $form_state['storage']['auth_info'] = $auth_info;
  }
  // Otherwise, show an error.
  else {
    form_set_error('', t("There was a problem verifying your information. Please try again."));
  }
}

function flocknote_authentication_form_submit($form, &$form_state) {
  // If reset button is clicked, delete flocknote_auth_info variable.
  if (!empty($form_state['clicked_button']['#id']) && $form_state['clicked_button']['#id'] == 'edit-reset') {
    variable_del('flocknote_auth_info');
    drupal_set_message(t("Your Flocknote authentication information has been reset."));
    return;
  }

  // Save the auth info.
  drupal_set_message(t("Your Flocknote authentication information has been verified and saved."));
  _flocknote_auth_info_password_encrypt($form_state['storage']['auth_info']);
  variable_set('flocknote_auth_info', $form_state['storage']['auth_info']);
}

/**
 * Helper function to retrieve lists for which current user has 'add_member'
 * permission.
 *
 * @return (array)
 *   List names keyed by List IDs for which member has 'add_member' permission.
 */
function _flocknote_registration_list_options() {
  $lists = array();
  $auth_info = flocknote_get_authentication_info();

  // If the authentication information couldn't be verified, don't attempt call.
  if (!$auth_info['verified']) {
    return $lists;
  }

  // Get all lists for the auth info.
  $flocknote = new Flocknote($auth_info['app_id'], $auth_info['app_key']);
  $flocknote->setUserCredentials($auth_info['user'], $auth_info['pass']);
  $all_lists = $flocknote->getListsForCurrentMember();

  // Loop through lists and only add ones with proper permissions to array.
  foreach ($all_lists as $list) {
    if ($list['permissions']['add_member'] === TRUE) {
      $lists[$list['id']] = $list['title'];
    }
  }

  return $lists;
}
