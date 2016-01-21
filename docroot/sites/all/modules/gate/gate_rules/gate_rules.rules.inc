<?php
/**
 * @file
 * Rules API integration for gate_rules.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function gate_rules_rules_event_info() {
  $events = array();

  $events['gate_rules_mail_init'] = array(
    'label' => t('Gate is blocking "direct-mail" (hook_mail_alter).'),
    'group' => t('Gate'),
    'variables' => array(
      'gate_mail_data' => array(
        'type' => 'gate_mail_data',
        'label' => t('Mail data on gate mail init.'),
      ),
      'gate_mail_addresses' => array(
        'type' => 'list<text>',
        'label' => t('List of mail addresses.'),
      ),
    ),
  );
  $events['gate_rules_user_login'] = array(
    'label' => t('Gate is blocking "direct-login" (hook_user_login).'),
    'group' => t('Gate'),
    'variables' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('registered user'),
      ),
      'gate_login' => array(
        'type' => 'gate_login_data',
        'label' => t('Current gate login data'),
      ),
    ),
  );

  $events['gate_rules_onetimelink'] = array(
    'label' => t('Onetime-Login-Link is requested in standard form.'),
    'group' => t('Gate'),
    'variables' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('registered user'),
      ),
    ),
  );

  $events['gate_rules_pwlogin'] = array(
    'label' => t('Standard password form is used.'),
    'group' => t('Gate'),
    'variables' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('registered user'),
      ),
    ),
  );

  return $events;
}

/**
 * Implements hook_rules_action_info().
 */
function gate_rules_rules_action_info() {
  $actions = array();

  $actions['gate_rules_mail_init_send'] = array(
    'label' => t('Reinitiate drupal_mail() with bypassing gate direct-mail blocker.'),
    'group' => t('Gate'),
    'callbacks' => array(
      'execute' => '_gate_rules_mail_init_send',
    ),
    'parameter' => _gate_rules_mail_init_base_parameter(),
  );

  $actions['gate_rules_mail_lists'] = array(
    'label' => t('Get lists of user objects and non-user mail-adresses.'),
    'group' => t('Gate'),
    'callbacks' => array(
      'execute' => '_gate_rules_mail_lists',
    ),
    'parameter' => array(
      'mail_data' => array(
        'label' => t('Mail Data'),
        'type' => 'gate_mail_data',
        'description' => t('Unchanged Mail Data from gate_mail_init event.'),
        'optional' => FALSE,
        'default mode' => 'selector',
      ),
      'mail_addresses' => array(
        'type' => 'list<text>',
        'label' => t('List of all mail addresses.'),
        'description' => t('The raw list of all mail addresses of a mail message.'),
        'optional' => FALSE,
        'default mode' => 'selector',
      ),
    ),
    'provides' => array(
      'mail_users' => array(
        'type' => 'list<user>',
        'label' => t('List of mail users.'),
      ),
      'mail_addresses_other' => array(
        'type' => 'list<text>',
        'label' => t('List of mail addresses (non-user mail)'),
      ),
    ),
  );

  $actions['gate_rules_login_data_set'] = array(
    'label' => t('Modify login data and initiate before if missing.'),
    'group' => t('Gate'),
    'callbacks' => array(
      'execute' => '_gate_rules_login_data_set',
    ),
    'named parameter' => TRUE,
    'parameter' => _gate_rules_login_data_set_parameter(),
  );

  $actions['gate_rules_login_token_session'] = array(
    'label' => t('Set the gate session token variable with current gate token to grant access.'),
    'group' => t('Gate'),
    'callbacks' => array(
      'execute' => '_gate_rules_login_token_session',
    ),
    'parameter' => array(
      'uid' => array(
        'label' => t('User ID'),
        'type' => 'integer',
        'description' => t("The User ID."),
        'optional' => FALSE,
        'default mode' => 'selector',
      ),
    ),
  );

  $actions['gate_rules_onetimelink_block'] = array(
    'label' => t('Block the request of onetime login link with standard form.'),
    'group' => t('Gate'),
    'callbacks' => array(
      'execute' => '_gate_rules_onetimelink_block',
    ),
  );

  $actions['gate_rules_pwlogin_block'] = array(
    'label' => t('Block the password login with standard form.'),
    'group' => t('Gate'),
    'callbacks' => array(
      'execute' => '_gate_rules_pwlogin_block',
    ),
  );

  $actions['gate_rules_load_server_variable'] = array(
    'label' => t('Load value from $_SERVER variable.'),
    'parameter' => array(
      'data_key' => array(
        'type' => 'text',
        'label' => t('Key to identify data'),
        'default mode' => 'input',
      ),
    ),
    'group' => t('Gate'),
    'provides' => array(
      'loaded_server_variable_data' => array(
        'label' => t('Loaded server variable data.'),
        'type' => 'text',
      ),
    ),
    'callbacks' => array(
      'execute' => '_gate_rules_load_server_variable',
    ),
  );

  $actions['gate_rules_user_login_directly'] = array(
    'label' => t('Login a user directly.'),
    'parameter' => array(
      'user_account' => array(
        'type' => 'user',
        'label' => t('Registered user'),
        'description' => t("The user account which should login directly in this http session."),
        'optional' => FALSE,
        'default mode' => 'selector',
      ),
    ),
    'provides' => array(
      'user_login_report' => array(
      'label' => t('User login directly report.'),
      'description' => t('An integer to report login directly action (0: default, 1: user already logged in, 2: logged in successful.'),
      'type' => 'integer',
      ),
    ),
    'group' => t('Gate'),
    'callbacks' => array(
      'execute' => '_gate_rules_user_login_directly',
    ),
  );
  return $actions;
}

/**
 * Implements hook_rules_data_info().
 */
function gate_rules_rules_data_info() {
  return array(
    'gate_login_data' => array(
      'label' => t('Gate login data'),
      'is wrapped' => TRUE,
      'property info' => _gate_rules_gate_login_data_info(),
    ),

    'gate_mail_data' => array(
      'label' => t('Gate mail data'),
      'is wrapped' => TRUE,
      'property info' => _gate_rules_gate_mail_data_info(),
    ),
  );
}

/**
 * Defines property info for gate_rules_rules_data_info(): 'gate_login_data'.
 */
function _gate_rules_gate_login_data_info() {
  return array(
    'uid' => array(
      'label' => t('User ID'),
      'type' => 'integer',
      'description' => t("The User ID."),
    ),
    'access' => array(
      'label' => t('Access granted'),
      'type' => 'boolean',
      'description' => t("Access is granted."),
    ),
    'level' => array(
      'label' => t('Login Process Level'),
      'type' => 'integer',
      'description' => t("Integer field can be used for multi-factor login."),
    ),
    'token' => array(
      'label' => t('Token'),
      'type' => 'text',
      'description' => t('The internal token (unencrypted) for check access setting. Use string "RESET" (without quotation) for recreation.'),
    ),
    'pass' => array(
      'label' => t('Password'),
      'type' => 'text',
      'description' => t("Password field can be used for multi-factor login."),
    ),
    'created' => array(
      'label' => t('Created'),
      'type' => 'date',
      'description' => t("The creation timestamp"),
    ),
    'changed' => array(
      'label' => t('Changed'),
      'type' => 'date',
      'description' => t("The timestamp when data was changed/updated."),
    ),
  );
}

/**
 * Defines property info for gate_rules_rules_data_info(): 'gate_mail_data'.
 */
function _gate_rules_gate_mail_data_info() {
  return array(
    'id' => array(
      'label' => t('id'),
      'type' => 'text',
      'description' => t("The drupal_mail() id of the message. Look at module source code or drupal_mail() for possible id values."),
    ),
    'module' => array(
      'label' => t('module'),
      'type' => 'text',
      'description' => t("Module"),
    ),
    'key' => array(
      'label' => t('key'),
      'type' => 'text',
      'description' => t("Key"),
    ),
    'to' => array(
      'label' => t('to'),
      'type' => 'text',
      'description' => t('The address or addresses the message will be sent to. The formatting of this string will be validated with the PHP e-mail validation filter.'),
    ),
    'from' => array(
      'label' => t('from'),
      'type' => 'text',
      'description' => t('The address the message will be marked as being from, which is either a custom address or the site-wide default email address.'),
    ),
    'language' => array(
      'label' => t('subject'),
      'type' => 'text',
      'description' => t('Subject of the email to be sent. This must not contain any newline characters, or the email may not be sent properly.'),
    ),
    'params' => array(
      'label' => t('params'),
      'type' => 'list<text>',
      'description' => t('An array of optional parameters supplied by the caller of drupal_mail() that is used to build the message before hook_mail_alter() is invoked.'),
    ),
    'send' => array(
      'label' => t('send'),
      'type' => 'boolean',
      'description' => t("Send is granted."),
    ),
    'subject' => array(
      'label' => t('subject'),
      'type' => 'text',
      'description' => t('Subject of the email to be sent.'),
    ),
    'body' => array(
      'label' => t('body'),
      'type' => 'list<text>',
      'description' => t('An array of strings containing the message text. The message body is created by concatenating the individual array strings into a single text string using "\n\n" as a separator.'),
    ),
    'headers' => array(
      'label' => t('headers'),
      'type' => 'list<text>',
      'description' => t('Associative array containing mail headers, such as From, Sender, MIME-Version, Content-Type, etc.'),
    ),
  );
}

/**
 * Parameter callback for .rules action 'gate_rules_mail_init_send'.
 */
function _gate_rules_mail_init_base_parameter() {
  return array(
    'mail_data' => array(
      'label' => t('Gate mail data'),
      'type' => 'gate_mail_data',
      'description' => t('Mail Data from gate_mail_init event.'),
      'optional' => FALSE,
      'default mode' => 'selector',
    ),
    'to' => array(
      'label' => t('"to"-address (string)'),
      'type' => 'text',
      'description' => t('New "to"-address string. (should be set splitting recipients).'),
      'optional' => TRUE,
      'default mode' => 'selector',
    ),
    'from' => array(
      'label' => t('"from"-address (string)'),
      'type' => 'text',
      'description' => t('New "from"-adddress (string) if you want to change.'),
      'optional' => TRUE,
    ),
  );
}

/**
 * Parameter callback for .rules action "gate_rules_login_data_set".
 */
function _gate_rules_login_data_set_parameter() {
  $gate_login_data = _gate_rules_gate_login_data_info();
  $parameters_opt = array('access', 'level', 'token', 'pass');

  $parameters = array();
  $parameters['uid'] = $gate_login_data['uid'];
  $parameters['uid']['optional'] = FALSE;
  $parameters['uid']['default mode'] = 'selector';

  foreach ($parameters_opt as $key) {
    $parameters[$key] = $gate_login_data[$key];
    $parameters[$key]['optional'] = TRUE;
  }

  return $parameters;
}

/**
 * @}
 */
