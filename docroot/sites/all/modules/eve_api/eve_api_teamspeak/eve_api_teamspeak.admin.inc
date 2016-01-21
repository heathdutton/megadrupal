<?php
/**
 * @file
 * Functions related to allow admin's to alter the settings and options.
 */

/**
 * Form constructor for the teamspeak connection form.
 *
 * @see eve_api_teamspeak_admin_form_validate()
 * @see eve_api_teamspeak_admin_form_submit()
 *
 * @ingroup forms
 */
function eve_api_teamspeak_admin_form($form, &$form_state) {
  if (($library = libraries_load('TeamSpeak3')) && !empty($library['loaded'])) {
    variable_set('eve_api_teamspeak_framework_installed', TRUE);
  }

  $form['teamspeak'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure TeamSpeak 3 Connection'),
    '#description' => t('Configure TeamSpeak 3 connection settings.'),
    '#collapsed' => variable_get('eve_api_teamspeak_enable', FALSE),
    '#collapsible' => TRUE,
    '#weight' => 0,
  );

  $form['teamspeak']['hostname'] = array(
    '#type' => 'textfield',
    '#title' => t('IP Address'),
    '#description' => t('The IP address used to connect to the TeamSpeak 3 Server. This can be 127.0.0.1 if the TeamSpeak server is hosted on the same server as the webhost.'),
    '#required' => TRUE,
    '#size' => 20,
    '#maxlength' => 15,
    '#weight' => 0,
    '#default_value' => variable_get('eve_api_teamspeak_hostname', '127.0.0.1'),
  );

  $form['teamspeak']['server_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Server Address'),
    '#description' => t('The server address users will use to connect to your TeamSpeak Server.'),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 50,
    '#weight' => 0,
    '#default_value' => variable_get('eve_api_teamspeak_server_address', 'teamspeak.com'),
  );

  $form['teamspeak']['server_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Server Port'),
    '#description' => t('The server port users will use to connect to your TeamSpeak 3 Server. (Default: 9987)'),
    '#required' => TRUE,
    '#size' => 10,
    '#maxlength' => 6,
    '#weight' => 10,
    '#default_value' => variable_get('eve_api_teamspeak_server_port', 9987),
  );

  $form['teamspeak']['server_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Server Password'),
    '#description' => t('If you require a password to connect to your TeamSpeak, please input the password, otherwise leave this empty.'),
    '#maxlength' => 50,
    '#size' => 60,
    '#weight' => 20,
    '#default_value' => variable_get('eve_api_teamspeak_server_password', ''),
  );

  $form['teamspeak']['query_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Query Port'),
    '#description' => t('The query port for the TeamSpeak 3 Server. (Default: 10011)'),
    '#required' => TRUE,
    '#size' => 10,
    '#maxlength' => 6,
    '#weight' => 30,
    '#default_value' => variable_get('eve_api_teamspeak_query_port', 10011),
  );

  $form['teamspeak']['query_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Query Username'),
    '#description' => t('The query username for the TeamSpeak 3 Server. (For security and in case anything goes wrong do not use serveradmin.)'),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 50,
    '#weight' => 40,
    '#default_value' => variable_get('eve_api_teamspeak_query_username', 'Username'),
  );

  $form['teamspeak']['query_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Query Password'),
    '#description' => t('The query password for the TeamSpeak 3 server.'),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 50,
    '#weight' => 50,
    '#default_value' => variable_get('eve_api_teamspeak_query_password', 'Password'),
  );

  $form['teamspeak']['query_nickname'] = array(
    '#type' => 'textfield',
    '#title' => t('Query Nickname'),
    '#description' => t('The query nickname to use when connecting to the TeamSpeak 3 server.'),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 25,
    '#weight' => 60,
    '#default_value' => variable_get('eve_api_teamspeak_query_nickname', 'Teamspeak_Bot'),
  );

  $form['teamspeak']['enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable TeamSpeak 3 Connection'),
    '#default_value' => variable_get('eve_api_teamspeak_enable', FALSE),
    '#weight' => 70,
  );

  if (variable_get('eve_api_teamspeak_enable', FALSE)) {
    $form['teamspeak_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('TeamSpeak 3 Settings'),
      '#description' => t('TeamSpeak 3 settings and options.'),
      '#collapsed' => !variable_get('eve_api_teamspeak_enable', FALSE),
      '#collapsible' => TRUE,
      '#weight' => 10,
    );

    $form['teamspeak_settings']['nickname'] = array(
      '#type' => 'textfield',
      '#title' => t('Username Format'),
      '#description' => t('The username format that each user must have when connecting to TeamSpeak 3. You can use the following variables:<br />@corp - Corporation Ticker<br />@alliance - Alliance Ticker<br />@name - EVE Character Name'),
      '#required' => TRUE,
      '#size' => 60,
      '#maxlength' => 50,
      '#weight' => 0,
      '#default_value' => variable_get('eve_api_teamspeak_nickname', '[@corp] @name'),
    );

    try {
      $teamspeakgroups = array();

      $ts3 = eve_api_teamspeak_connect();
      $teamspeak_groups = $ts3->serverGroupList();
      eve_api_teamspeak_disconnect($ts3);

      foreach ($teamspeak_groups as $teamspeak_group) {
        if ((int) $teamspeak_group->type == 1 && (string) $teamspeak_group->name != 'Guest') {
          $teamspeakgroups[(int) $teamspeak_group->sgid] = check_plain((string) $teamspeak_group->name);
        }
      }

      $form['teamspeak_settings']['bypass_group'] = array(
        '#type' => 'select',
        '#title' => t('Select TeamSpeak Bypass Group'),
        '#description' => t('Please select a TeamSpeak Group for users to bypass the username and registration checks.'),
        '#options' => $teamspeakgroups,
        '#default_value' => variable_get('eve_api_teamspeak_bypass_group', 0) == 0 ? '' : variable_get('eve_api_teamspeak_bypass_group', 0),
        '#weight' => 10,
      );
    }
    catch (EveApiException $e) {
      $form['teamspeak_settings']['bypass_group'] = array(
        '#type' => 'markup',
        '#prefix' => '<p>',
        '#markup' => t('TeamSpeak 3 Connection failed, check settings and try again. Error: @error', array('@error' => $e->getMessage())),
        '#suffix' => '</p>',
        '#weight' => 10,
      );
    }

    $selectable_options = array(
      1 => '1 Minute',
      5 => '5 Minutes',
      10 => '10 Minutes',
      15 => '15 Minutes',
      20 => '20 Minutes',
      25 => '25 Minutes',
      30 => '30 Minutes',
      35 => '35 Minutes',
      40 => '40 Minutes',
      45 => '45 Minutes',
      50 => '50 Minutes',
      55 => '55 Minutes',
      60 => '1 Hour',
    );

    $long_selectable_options = array(
      120 => '2 Hours',
      180 => '3 Hours',
      240 => '4 Hours',
      300 => '5 Hours',
      360 => '6 Hours',
      420 => '7 Hours',
      480 => '8 Hours',
      540 => '9 Hours',
      600 => '10 Hours',
      660 => '11 Hours',
      720 => '12 Hours',
      780 => '13 Hours',
      840 => '14 Hours',
      900 => '15 Hours',
      960 => '16 Hours',
      1020 => '17 Hours',
      1080 => '18 Hours',
      1140 => '19 Hours',
      1200 => '20 Hours',
      1260 => '21 Hours',
      1320 => '22 Hours',
      1380 => '23 Hours',
      1440 => '1 Day',
      2880 => '2 Days',
      4320 => '3 Days',
      5760 => '4 Days',
      7200 => '5 Days',
      8640 => '6 Days',
      10080 => '1 Week',
      20160 => '2 Weeks',
      30240 => '3 Weeks',
      43200 => '1 Month',
      525600 => '1 Year',
    );

    $form['registration_notification'] = array(
      '#type' => 'fieldset',
      '#title' => t('Registration Notification'),
      '#description' => t('Options releated to non-registered TeamSpeak 3 users.'),
      '#weight' => 20,
      '#collapsed' => FALSE,
      '#collapsible' => TRUE,
    );

    $form['registration_notification']['reg_time'] = array(
      '#type' => 'select',
      '#title' => t('User Register Time Limit'),
      '#description' => t('How long until the registration options are enforced.'),
      '#weight' => 0,
      '#options' => $selectable_options,
      '#default_value' => variable_get('eve_api_teamspeak_reg_time', 10),
    );

    $form['registration_notification']['reg_ban_time'] = array(
      '#type' => 'select',
      '#title' => t('Ban Time'),
      '#description' => t('How long to ban the user once the Time Limit as expired.'),
      '#weight' => 10,
      '#options' => ($selectable_options + $long_selectable_options),
      '#default_value' => variable_get('eve_api_teamspeak_reg_ban_time', 10),
    );

    $options = $selected = array();

    $options['reg_message_enable'] = t('Message');
    $options['reg_poke_enable'] = t('Poke');
    $options['reg_kick_enable'] = t('Kick - (Only when User Register Time Limit has expired.)');
    $options['reg_ban_enable'] = t('Ban - (Only when User Register Time Limit has expired.)');

    $selected['reg_message_enable'] = variable_get('eve_api_teamspeak_reg_message_enable', FALSE);
    $selected['reg_poke_enable'] = variable_get('eve_api_teamspeak_reg_poke_enable', FALSE);
    $selected['reg_kick_enable'] = variable_get('eve_api_teamspeak_reg_kick_enable', FALSE);
    $selected['reg_ban_enable'] = variable_get('eve_api_teamspeak_reg_ban_enable', FALSE);

    $form['registration_notification']['reg_options_description'] = array(
      '#title' => t('Notification Options'),
      '#description' => t('Notification Options to notify the offending TeamSpeak 3 user.'),
      '#type' => 'item',
      '#weight' => 20,
    );

    $form['registration_notification']['reg_options'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => array_keys(array_filter($selected)),
      '#weight' => 30,
    );

    $form['user_notification'] = array(
      '#type' => 'fieldset',
      '#title' => t('Nickname Notification'),
      '#description' => t('Options releated to registered TeamSpeak 3 users with incorrect nicknames.'),
      '#weight' => 30,
      '#collapsed' => FALSE,
      '#collapsible' => TRUE,
    );

    $form['user_notification']['user_time'] = array(
      '#type' => 'select',
      '#title' => t('Correct Name Time Limit'),
      '#description' => t('How long until the incorrect nickname options are enforced.'),
      '#weight' => 0,
      '#options' => $selectable_options,
      '#default_value' => variable_get('eve_api_teamspeak_user_time', 10),
    );

    $form['user_notification']['user_ban_time'] = array(
      '#type' => 'select',
      '#title' => t('Ban Time'),
      '#description' => t('How long to Ban the user once the Time Limit has expired.'),
      '#weight' => 10,
      '#options' => ($selectable_options + $long_selectable_options),
      '#default_value' => variable_get('eve_api_teamspeak_user_ban_time', 10),
    );

    $options = $selected = array();

    $options['user_message_enable'] = t('Message');
    $options['user_poke_enable'] = t('Poke');
    $options['user_kick_enable'] = t('Kick - (Only when Correct Name Time Limit has expired.)');
    $options['user_ban_enable'] = t('Ban - (Only when Correct Name Time Limit has expired.)');

    $selected['user_message_enable'] = variable_get('eve_api_teamspeak_user_message_enable', FALSE);
    $selected['user_poke_enable'] = variable_get('eve_api_teamspeak_user_poke_enable', FALSE);
    $selected['user_kick_enable'] = variable_get('eve_api_teamspeak_user_kick_enable', FALSE);
    $selected['user_ban_enable'] = variable_get('eve_api_teamspeak_user_ban_enable', FALSE);

    $form['user_notification']['user_options_description'] = array(
      '#title' => t('Notification Options'),
      '#description' => t('Notificaiton Options available after the Time Limit has expired.'),
      '#type' => 'item',
      '#weight' => 20,
    );

    $form['user_notification']['user_options'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => array_keys(array_filter($selected)),
      '#weight' => 30,
    );
  }

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 50,
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  if (!variable_get('eve_api_teamspeak_framework_installed', FALSE)) {
    drupal_set_message(t('TeamSpeak 3 PHP Framework Library is not installed, please read the README.txt for instructions on how to install the library.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_teamspeak_admin_form_validate');
  $form['#submit'] = array('eve_api_teamspeak_admin_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_teamspeak_admin_form().
 *
 * @see eve_api_teamspeak_admin_form_submit()
 */
function eve_api_teamspeak_admin_form_validate($form, &$form_state) {
  variable_set('eve_api_teamspeak_hostname', (string) $form_state['values']['hostname']);
  variable_set('eve_api_teamspeak_server_address', (string) $form_state['values']['server_address']);
  variable_set('eve_api_teamspeak_server_port', (int) $form_state['values']['server_port']);
  variable_set('eve_api_teamspeak_server_password', (string) $form_state['values']['server_password']);
  variable_set('eve_api_teamspeak_query_port', (int) $form_state['values']['query_port']);
  variable_set('eve_api_teamspeak_query_username', (string) $form_state['values']['query_username']);
  variable_set('eve_api_teamspeak_query_password', (string) $form_state['values']['query_password']);

  if (preg_match('/^[a-zA-Z0-9-_]+$/', (string) $form_state['values']['query_nickname']) != 1) {
    form_set_error('query_nickname', t('TeamSpeak 3 Query Nickname only supports numbers, letters and spaces.'));
  }

  if (variable_get('eve_api_teamspeak_framework_installed', FALSE)) {
    if ((bool) $form_state['values']['enable'] && variable_get('eve_api_teamspeak_enable', FALSE) == FALSE) {
      try {
        $ts3 = eve_api_teamspeak_connect(TRUE);
        eve_api_teamspeak_disconnect($ts3, TRUE);
      }
      catch (EveApiException $e) {
        form_set_error('enable', check_plain($e->getMessage()));
        variable_set('eve_api_teamspeak_enable', FALSE);
      }
    }
  }
  else {
    form_set_error('enable', t('TeamSpeak 3 PHP Framework not found, unable to enable TeamSpeak 3 connection.'));
  }
}

/**
 * Form submission handler for eve_api_teamspeak_admin_form().
 *
 * @see eve_api_teamspeak_admin_form_validate()
 */
function eve_api_teamspeak_admin_form_submit($form, &$form_state) {
  variable_set('eve_api_teamspeak_query_nickname', (string) $form_state['values']['query_nickname']);

  if ((bool) $form_state['values']['enable']) {
    if (variable_get('eve_api_teamspeak_enable', FALSE)) {
      variable_set('eve_api_teamspeak_bypass_group', (int) $form_state['values']['bypass_group']);
      variable_set('eve_api_teamspeak_nickname', (string) $form_state['values']['nickname']);
      variable_set('eve_api_teamspeak_reg_time', (int) $form_state['values']['reg_time']);
      variable_set('eve_api_teamspeak_user_time', (int) $form_state['values']['user_time']);
      variable_set('eve_api_teamspeak_reg_ban_time', (int) $form_state['values']['reg_ban_time']);
      variable_set('eve_api_teamspeak_user_ban_time', (int) $form_state['values']['user_ban_time']);

      foreach ((array) $form_state['values']['reg_options'] as $field => $value) {
        $field = 'eve_api_teamspeak_' . (string) $field;
        variable_set($field, (bool) $value);
      }

      foreach ((array) $form_state['values']['user_options'] as $field => $value) {
        $field = 'eve_api_teamspeak_' . (string) $field;
        variable_set($field, (bool) $value);
      }
    }

    variable_set('eve_api_teamspeak_enable', TRUE);

    $queue = DrupalQueue::get('eve_api_teamspeak_cron_role_sync');
    $queue->createItem(1);
  }
  else {
    variable_set('eve_api_teamspeak_enable', FALSE);
  }

  menu_rebuild();

  drupal_set_message(t('TeamSpeak Settings Updated.'));
}
