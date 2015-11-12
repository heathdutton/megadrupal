<?php
/**
 * @file
 * Functions related to allowing users to alter the user settings and options.
 */

/**
 * Form constructor for the user teamspeak registration form.
 *
 * @param string $account
 *   The account information for the user page being viewed.
 *
 * @see eve_api_teamspeak_user_form_validate()
 * @see eve_api_teamspeak_user_form_submit()
 *
 * @ingroup forms
 */
function eve_api_teamspeak_user_form($form, &$form_state, $account) {
  if (!isset($form_state['user'])) {
    $form_state['user'] = $account;
  }
  else {
    $account = $form_state['user'];
  }

  $uid = $account->uid;

  $teamspeak_port_check = variable_get('eve_api_teamspeak_server_port', 9987);
  $teamspeak_password_check = variable_get('eve_api_teamspeak_server_password', '');

  $form['teamspeak']['info'] = array(
    '#type' => 'item',
    '#title' => t('Register your TeamSpeak Identity to your User Account'),
    '#description' => t('In order to register with TeamSpeak, you must first connect to our TeamSpeak.<br />If you do not have TeamSpeak already installed, please download and install TeamSpeak 3 from <a href="@teamspeak_url" target="_blank">here</a>.<br />Please use the following settings to connect:<br /><br />Address - @teamspeak_address<br />@teamspeak_password', array(
      '@teamspeak_url' => 'http://www.teamspeak.com/?page=downloads',
      '@teamspeak_address' => empty($teamspeak_port_check) ? variable_get('eve_api_teamspeak_server_address', 'teamspeak.com') : variable_get('eve_api_teamspeak_server_address', 'teamspeak.com') . ':' . variable_get('eve_api_teamspeak_server_port', 9987),
      '@teamspeak_password' => empty($teamspeak_password_check) ? '' : t('Password') . ' - ' . variable_get('eve_api_teamspeak_server_password', ''),
    )),
    '#weight' => 0,
  );

  $form['teamspeak']['teamspeak_name'] = array(
    '#type' => 'item',
    '#title' => t('Username:'),
    '#description' => ($account->characterID != 0) ? eve_api_teamspeak_format_name($account->name) : t('<a href="@url">Please select a main character.</a>', array('@url' => url('user/' . $uid . '/edit'))),
    '#weight' => 10,
  );

  $form['teamspeak']['teamspeak_uid'] = array(
    '#type' => 'item',
    '#title' => t('Your TeamSpeak Unique ID:'),
    '#description' => !empty($account->teamspeakUID) ? $account->teamspeakUID : t('You are not registered.'),
    '#weight' => 20,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Register'),
  );

  if ($account->characterID == 0) {
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_teamspeak_user_form_validate');
  $form['#submit'] = array('eve_api_teamspeak_user_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_teamspeak_user_form().
 *
 * @see eve_api_teamspeak_user_form_submit()
 */
function eve_api_teamspeak_user_form_validate($form, &$form_state) {
  $account = $form_state['user'];
  $uid = $account->uid;

  if ($account->characterID == 0) {
    drupal_goto('user/' . $uid . '/edit');
  }

  try {
    $ts3 = eve_api_teamspeak_connect();
    $teamspeak_name = eve_api_teamspeak_format_name($account->name);
    $teamspeak_data = eve_api_teamspeak_user_by_name($ts3, $teamspeak_name);
    unset($teamspeak_data);
    eve_api_teamspeak_disconnect($ts3);
  }
  catch (EveApiException $e) {
    form_set_error('teamspeak', check_plain($e->getMessage()));
  }
}

/**
 * Form submission handler for eve_api_teamspeak_user_form().
 *
 * @see eve_api_teamspeak_user_form_validate()
 */
function eve_api_teamspeak_user_form_submit($form, &$form_state) {
  $account = $form_state['user'];
  $uid = (int) $account->uid;

  try {
    $ts3 = eve_api_teamspeak_connect();
    $teamspeak_name = eve_api_teamspeak_format_name($account->name);
    $teamspeak_data = eve_api_teamspeak_user_by_name($ts3, $teamspeak_name);
    $teamspeak_uid = (string) $teamspeak_data->client_unique_identifier;
    $teamspeak_dbid = (int) $teamspeak_data->client_database_id;

    db_update('users')->fields(array(
        'teamspeakUID' => $teamspeak_uid,
        'teamspeakDBID' => $teamspeak_dbid,
      ))->condition('uid', $uid, '=')->execute();

    // Need to assign roles.
    drupal_set_message(t('TeamSpeak access granted.'));
    eve_api_teamspeak_disconnect($ts3);
  }
  catch (EveApiException $e) {
    drupal_set_message(t('TeamSpeak connection failed, please try again later. Error: @error', array('@error' => $e->getMessage())), 'error');
  }
}
