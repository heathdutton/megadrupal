<?php
/*
 * @file
 * User page callbacks for the yubikey module.
 */

// YubiKey lost reporting

/**
 * Form builder: YubiKey lost reporting page.
 */
function yubikey_lost() {
  drupal_add_js(drupal_get_path('module', 'yubikey') . '/yubikey.js', 'file');

  $form['name'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Username or e-mail address'),
    '#size'          => 60,
    '#maxlength'     => max(USERNAME_MAX_LENGTH, EMAIL_MAX_LENGTH),
    '#required'      => TRUE,
  );

  $form['yubikey_forgot_pass'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('I forgot my password'),
    '#default_value' => variable_get('yubikey_forgot_pass', FALSE)
  );

  $form['pass'] = array(
    '#type'          => 'password',
    '#title'         => t('Password'),
    '#prefix'        => '<div id="yk_pass_div">',
    '#suffix'        => '</div>'
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Report loss of YubiKey'));

  return $form;
}

function yubikey_lost_validate($form, &$form_state) {
  $forgot_pass = $form_state['values']['yubikey_forgot_pass'];
  if (!$forgot_pass && empty($form_state['values']['pass'])) {
    form_set_error('pass', t('Password field is required.'));
    return;
  }
  
  // Try to load by email and pass.
  if (!$forgot_pass) {
    $account = db_query(
      "SELECT * FROM {users} WHERE mail = :name AND pass = :pass AND status = 1", 
      array(
        ':name' => $form_state['values']['name'],
        ':pass' => $form_state['values']['pass'],
      ))->fetchObject();
  } 
  else {
    $account = db_query(
      "SELECT * FROM {users} WHERE mail = :name AND status = 1", 
      array(
        ':name' => $form_state['values']['name'],
      ))->fetchObject();
  }
  if (!$account) {
    // No success, try to load by name and pass.
    if (!$forgot_pass) {
      $account = db_query(
      "SELECT * FROM {users} WHERE name = :name AND pass = :pass AND status = 1", 
      array(
        ':name' => $form_state['values']['name'],
        ':pass' => $form_state['values']['pass'],
      ))->fetchObject();
    } 
    else {
      $account = db_query(
      "SELECT * FROM {users} WHERE name = :name AND status = 1", 
      array(
        ':name' => $form_state['values']['name'],
      ))->fetchObject();
    }
  }

  if ($account) {
    // Blocked accounts cannot report loss of YubiKey,
    // check provided username and email against access rules.
    if (drupal_is_denied('user', $account->name) || drupal_is_denied('mail', $account->mail)) {
      form_set_error('name', t('%name is not allowed to report loss of YubiKey.', array('%name' => $form_state['values']['name'])));
    }
  }

  if (isset($account->uid)) {
    form_set_value(array('#parents' => array('account')), $account, $form_state);
  }
  else {
    if (!$forgot_pass) {
      form_set_error('name', t('Sorry, unrecognized username/e-mail address or password.'));
    } 
    else {
      form_set_error('name', t('Sorry, %name is not recognized as a user name or an e-mail address.', array('%name' => $form_state['values']['name'])));
    }
  }
}

/**
 * Mail one-time YubiKey Lost confirmation link.
 */
function yubikey_lost_submit($form, &$form_state) {
  global $language;

  $account = $form_state['values']['account'];
  // Send yubikey lost confirmation mail (containing one-time confirmation URL) using current language.
  _yubikey_mail_notify('yubikey_lost', $account, $language);
  watchdog('yubikey', 'YubiKey lost confirmation link mailed to %name at %email.', array('%name' => $account->name, '%email' => $account->mail));
  drupal_set_message(t('Further instructions have been sent to your e-mail address.'));

  $form_state['redirect'] = 'user';
  return;
}

/**
 * Menu callback: process one-time YubiKey Lost confirmation link and block all YubiKeys associated with the specified user account on success.
 */
function yubikey_lost_confirm($form, &$form_state, $uid, $timestamp, $hashed_pass) {
  // Time out, in seconds, until confirmation URL expires. 24 hours = 86400 seconds.
  $timeout = 86400;
  $current = REQUEST_TIME;
  $account = db_query(
    "SELECT * FROM {users} WHERE uid = :uid AND status = 1", 
    array(
      ':uid' => $uid
    ))->fetchObject();
  if ($timestamp < $current && isset($account->uid)) {
    // Check if link has expired.
    if ($current - $timestamp > $timeout) {
      // Link has expired.
      drupal_set_message(t('You have tried to use a one-time YubiKey Lost confrimation link that has expired.'));
    } 
    elseif ($account->uid && $timestamp > $account->login && $hashed_pass == user_pass_rehash($account->pass, $timestamp, $account->login)) {
      // Check if user has associated YubiKeys.
      $count_aid = db_query("SELECT COUNT(aid) FROM {authmap} WHERE uid = :uid", array(':uid' => $account->uid))->fetchField();
      if ($count_aid != '') { 
        // User has associated YubiKeys.

        // Check if link has been already used.
        $max_created = db_query("SELECT MAX(created) FROM {yubikey_blocked} WHERE uid = :uid", array(':uid' => $account->uid))-fetchField();
        if ($max_created == '') $max_created = 0;
        if ($timestamp > $max_created) {
          db_delete('yubikey_blocked')
            ->condition('uid', $account->uid)
            ->execute();
          db_query(
            "INSERT INTO {yubikey_blocked} SELECT aid, uid, 1, :timestamp FROM {authmap} WHERE uid = :uid", 
            array(
              ':timestamp' => $timestamp, 
              ':uid' => $account->uid
            ));
          // Send yubikey lost reported notification mail using current language.
          _yubikey_mail_notify('yubikey_lost_confirmed', $account, $language);
          watchdog('yubikey', 'User %name confirmed loss of YubiKey at time %timestamp.', array('%name' => $account->name, '%timestamp' => $timestamp));

          $yk_authscheme = variable_get('yubikey_authscheme', 'un_pwd');
          if (($yk_authscheme == 'un_pwd_yk' && variable_get('yubikey_optional_if_lost', 0)) || $yk_authscheme == 'un_or_yk_pwd') {
            drupal_set_message(t('Thanks for reporting loss of your YubiKey. You can temporarily log in to this site using your Username and Password until a new YubiKey is assigned to your account.'));
          } 
          else {
            drupal_set_message(t('Thanks for reporting loss of your YubiKey. You cannot login until a new YubiKey is assigned to your account. Please contact the administrator.'));
          }
        } 
        else {
          drupal_set_message(t('You have tried to use a one-time YubiKey Lost confrimation link which has either been used or is no longer valid.'));
        }
      }
      else { 
        // User doesn't have any associated YubiKey.
        drupal_set_message(t('You don\'t have any YubiKey associated with your account.'));
      }
    }
    drupal_goto('user');
  } 
  else {
    // Deny access, no more clues.
    // Everything will be in the watchdog's URL for the administrator to check.
    drupal_access_denied();
  }
}


// YubiKey Management

/**
 * Menu callback; Manage YubiKey identities for the specified user.
 */
function yubikey_user_identities($account) {
  drupal_set_title($account->name);
  drupal_add_css(drupal_get_path('module', 'yubikey') . '/yubikey.css', 'module');

  $header = array(t('YubiKey ID'), t('Status'), array('data' => t('Operations'), 'colspan' => 2, 'align' => 'center'));
  $rows = array();

  $result = db_query("SELECT a.*, b.status FROM {authmap} a LEFT JOIN {yubikey_blocked} b ON a.aid = b.aid WHERE a.module = 'yubikey' AND a.uid = :uid", array(':uid' => $account->uid));
  
  while ($identity = $result->fetchObject()) {
    if ($identity->status == '') { // YubiKey is active.
      $rows[] = array(check_plain($identity->authname), t('Active'), l(t('Block'), 'user/' . $account->uid . '/yubikey/block/' . $identity->aid), l(t('Delete'), 'user/' . $account->uid . '/yubikey/delete/' . $identity->aid));
    } 
    else {
      $rows[] = array(check_plain($identity->authname), t('Blocked'), l(t('Activate'), 'user/' . $account->uid . '/yubikey/activate/' . $identity->aid), l(t('Delete'), 'user/' . $account->uid . '/yubikey/delete/' . $identity->aid));
    }
  }

  if (count($rows) > 0) {
    $build['yubikey_table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  } 

  $build['yubikey_user_add'] = drupal_get_form('yubikey_user_add');
  return $build;
}

/**
 * Form builder; Add a YubiKey identity.
 */
function yubikey_user_add() {
  $form['yubikey_otp'] = array(
    '#type'          => 'textfield',
    '#required'      => TRUE,
    '#size'          => 45,
    '#title'         => t('YubiKey OTP'),
    '#attributes'    => array( 'autocomplete' => 'off' )
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Add a YubiKey'));
  return $form;
}

function yubikey_user_add_validate($form, &$form_state) {
  module_load_include('inc', 'yubikey');

  $otp = $form_state['values']['yubikey_otp'];
  // Check if OTP is valid.
  if (yubikey_is_otp($otp)) {
    // Validate OTP.
    if (!yubikey_validate_otp($otp, $errstr)) {
      form_set_error('yubikey_otp', t('YubiKey OTP validation failed with message:') . ' ' . $errstr);
    } 
    else {
      // Check if YubiKey is already in use.
      $yk_identity = yubikey_get_identity($otp);
      if (db_query("SELECT authname FROM {authmap} WHERE module = 'yubikey' AND LOWER(authname) = LOWER(?)", array($yk_identity))->fetchField()) {
        form_set_error('yubikey_otp', t('That YubiKey is already in use on this site.'));
      }
    }
  } 
  else {
    form_set_error('yubikey_otp', t('YubiKey OTP is not valid.'));
  }
}

/**
 * Add a YubiKey identity.
 */
function yubikey_user_add_submit($form, &$form_state) {
  if (!form_get_errors()) {
    module_load_include('inc', 'yubikey');
    
    $otp = $form_state['values']['yubikey_otp']; 
    $yk_identity = yubikey_get_identity($otp);
    $aid = db_insert('authmap')
      ->fields(array(
        'uid' => arg(1),
        'authname' => $yk_identity,
        'module' => 'yubikey',
      ))
      ->execute();
    if ($aid) {
      $user = yubikey_user_load_info(array('uid' => arg(1), 'authname' => $yk_identity));
      $variables = array('%yk_identity' => $yk_identity, '%name' => $user->name);
      drupal_set_message(t('YubiKey %yk_identity has been added.', $variables)); 
      watchdog('yubikey', 'Added YubiKey %yk_identity for user %name.', $variables, WATCHDOG_NOTICE);
    }
  }
}

/**
 * Menu callback: activate a YubiKey identity.
 */
function yubikey_user_activate($form, $form_state, $account, $aid = 0) {
  $yk_identity = db_query('SELECT authname FROM {authmap} WHERE uid = :uid AND aid = :aid', array(':uid' => $account->uid, ':aid' => $aid))->fetchField();
  
  if ($yk_identity) {
    $id = db_delete('yubikey_blocked')
      ->condition('uid', $account->uid)
      ->condition('aid', $aid)
      ->execute();
    if ($id) {
      $variables = array('%yk_identity' => $yk_identity, '%name' => $account->name);
      drupal_set_message(t('YubiKey %yk_identity has been activated.', $variables));
      watchdog('yubikey', 'Activated YubiKey %yk_identity for user %name.', $variables, WATCHDOG_NOTICE);
    }
  }
  drupal_goto('user/' . $account->uid . '/yubikey');
}

/**
 * Menu callback: block a YubiKey identity.
 */
function yubikey_user_block($form, $form_state, $account, $aid = 0) {
  $yk_identity = db_query('SELECT authname FROM {authmap} WHERE uid = :uid AND aid = :aid', array(':uid' => $account->uid, ':aid' => $aid))->fetchField();

  if ($yk_identity) {
    $result = db_insert('yubikey_blocked')
      ->fields(array(
        'aid' => $aid,
        'uid' => $account->uid,
        'status' => 1,
        'created' => 0,
      ))
      ->execute();
    if ($result) {
      $variables = array('%yk_identity' => $yk_identity, '%name' => $account->name);
      drupal_set_message(t('YubiKey %yk_identity has been blocked.', $variables));
      watchdog('yubikey', 'Blocked YubiKey %yk_identity for user %name.', $variables, WATCHDOG_NOTICE);
    }
  }
  drupal_goto('user/' . $account->uid . '/yubikey');
}

/**
 * Menu callback: present a confirmation page to delete the specified YubiKey identity.
 */
function yubikey_user_delete_confirm($form, &$form_state, $account, $aid = 0) {
  $yk_identity = db_query('SELECT authname FROM {authmap} WHERE uid = :uid AND aid = :aid', array(':uid' => $account->uid, ':aid' => $aid))->fetchField();

  if ($yk_identity) {
    $form = array();
    $form['uid'] = array(
      '#type'        => 'value',
      '#value'       => $account->uid
    );
    $form['aid'] = array(
      '#type'        => 'value',
      '#value'       => $aid,
    );
    $form['name'] = array(
      '#type'        => 'value',
      '#value'       => $account->name,
    );
    $form['yk_identity'] = array(
      '#type'        => 'value',
      '#value'       => $yk_identity,
    );

    return confirm_form($form, t('Are you sure you want to delete the YubiKey %yk_identity for %user?', array('%yk_identity' => $yk_identity, '%user' => $account->name)), 'user/' . $account->uid . '/yubikey');
  }
  drupal_goto('user/' . $account->uid . '/yubikey');
}

/**
 * Delete a YubiKey identity.
 */
function yubikey_user_delete_confirm_submit($form, &$form_state) {
  $res1 = db_delete('yubikey_blocked')
    ->condition('uid', $form_state['values']['uid'])
    ->condition('aid', $form_state['values']['aid'])
    ->execute();
  $res2 = db_delete('authmap')
    ->condition('uid', $form_state['values']['uid'])
    ->condition('aid', $form_state['values']['aid'])
    ->execute();
  if ($res1 && $res2) {
    $variables = array('%yk_identity' => $form_state['values']['yk_identity'], '%name' => $form_state['values']['name']);
    drupal_set_message(t('YubiKey %yk_identity has been deleted.', $variables));
    watchdog('yubikey', 'Deleted YubiKey %yk_identity for user %name.', $variables, WATCHDOG_NOTICE);
  }
  $form_state['redirect'] = 'user/' . $form_state['values']['uid'] . '/yubikey';
}


// YubiKey global settings

/**
 * Form builder: configure YubiKey settings for this site.
 */
function yubikey_admin_settings() {
  drupal_add_js(drupal_get_path('module', 'yubikey') . '/yubikey_admin.js', 'file');
  
  $form = array(
    '#validate'      => array('yubikey_admin_settings_validate'),
    '#submit'        => array('yubikey_admin_settings_submit'),
  );

  $form['yubikey module desc'] = array(
    '#type'          => 'item',
    '#value'         => t('The YubiKey module provides YubiKey based strong two-factor user authentication capabilities to your site. Here, you can configure global settings for this module.'),
  );
  $form['yubikey_enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable YubiKey authentication'),
    '#default_value' => variable_get('yubikey_enabled', FALSE)
  );

  // Authentication schemes.
  $yk_authscheme_options = array(
    'pwd_yk'         => t('Password + YubiKey OTP'),
    'un_or_yk_pwd'   => t('(Username or YubiKey OTP) + Password'),
    'yk_only'        => t('YubiKey OTP only'),
    'un_pwd_yk'      => t('Username + Password + YubiKey OTP')
  );
  $form['yubikey_authscheme'] = array(
    '#type'          => 'radios',
    '#title'         => t('YubiKey authentication scheme'),
    '#default_value' => variable_get('yubikey_authscheme', 'un_pwd_yk'),
    '#options'       => $yk_authscheme_options,
    '#required'      => TRUE,
    '#prefix'        => '<div id="yk_fields_div">'
  );
  $form['yubikey_optional'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Make YubiKey OTP optional until assigned'),
    '#default_value' => variable_get('yubikey_optional', FALSE),
    '#description'   => t('If enabled, users will be able to log into your site using Username and Passowrd as long as no YubiKey has been assigned. Once a YubiKey is assigned to a particular user account, that user must provide a YubiKey OTP along with the Username and Password in order to login.'),
    '#prefix'        => '<div id="yk_optional_div">'
  );
  $form['yubikey_optional_if_lost'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Make YubiKey OTP optional in case of loss of YubiKey'),
    '#default_value' => variable_get('yubikey_optional_if_lost', FALSE),
    '#description'   => t('If enabled, the YubiKey OTP field will be automatically made optional for users who have reported and confirmed loss of their YubiKey.'),
    '#suffix'        => '</div>'
  );
  
  // Validation service settings.
  $yk_collapsed = (variable_get('yubikey_valserver_api_key', NULL) != NULL);

  $form['yubikey_valserver'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('YubiKey validation service settings'),
    '#description'   => t('You can choose Yubico online validation service or provide up to 5 URLs if you are hosting your own validation server instances.'),
    '#tree'          => TRUE
  );

  // URLs.
  $form['yubikey_valserver']['urls'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Validation service URLs'),
    '#collapsible'   => TRUE,
    '#collapsed'     => $yk_collapsed,
    '#tree'          => TRUE
  );
  $yk_valserver_urls_options = array(
    'online'         => t('Use Yubico online validation service'),
    'custom'         => t('Use your own YubiKey validation server instances')
  );
  $form['yubikey_valserver']['urls']['type'] = array(
    '#type'          => 'radios',
    '#default_value' => variable_get('yubikey_valserver_urls_type', 'online'),
    '#options'       => $yk_valserver_urls_options,
    '#required'      => TRUE
  );
  $form['yubikey_valserver']['urls']['1'] = array(
    '#type'          => 'textfield',
    '#title'         => t('URL #1'),
    '#default_value' => variable_get('yubikey_valserver_urls_1', NULL),
    '#size'          => 40,
    '#maxlength'     => 256,
    '#required'      => FALSE,
    '#prefix'        => '<div id="yk_valserver_urls_div">'
  );
  $form['yubikey_valserver']['urls']['2'] = array(
    '#type'          => 'textfield',
    '#title'         => t('URL #2'),
    '#default_value' => variable_get('yubikey_valserver_urls_2', NULL),
    '#size'          => 40,
    '#maxlength'     => 256,
    '#required'      => FALSE
  );
  $form['yubikey_valserver']['urls']['3'] = array(
    '#type'          => 'textfield',
    '#title'         => t('URL #3'),
    '#default_value' => variable_get('yubikey_valserver_urls_3', NULL),
    '#size'          => 40,
    '#maxlength'     => 256,
    '#required'      => FALSE
  );
  $form['yubikey_valserver']['urls']['4'] = array(
    '#type'          => 'textfield',
    '#title'         => t('URL #4'),
    '#default_value' => variable_get('yubikey_valserver_urls_4', NULL),
    '#size'          => 40,
    '#maxlength'     => 256,
    '#required'      => FALSE
  );
  $form['yubikey_valserver']['urls']['5'] = array(
    '#type'          => 'textfield',
    '#title'         => t('URL #5'),
    '#default_value' => variable_get('yubikey_valserver_urls_5', NULL),
    '#size'          => 40,
    '#maxlength'     => 256,
    '#required'      => FALSE,
    '#suffix'        => '</div>'
  );

  // API parameters.
  $form['yubikey_valserver']['api'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Validation service API parameters'),
    '#description'   => t('You can ' . l(t('get your API key'), 'https://upgrade.yubico.com/getapikey/') . ' and API ID from Yubico.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => $yk_collapsed,
    '#tree'          => TRUE
  );
  $form['yubikey_valserver']['api']['key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('API key'),
    '#default_value' => variable_get('yubikey_valserver_api_key', NULL),
    '#size'          => 40,
    '#maxlength'     => 128,
    '#required'      => TRUE,
  );
  $form['yubikey_valserver']['api']['id'] = array(
    '#type'          => 'textfield',
    '#title'         => t('API ID'),
    '#default_value' => variable_get('yubikey_valserver_api_id', NULL),
    '#size'          => 6,
    '#maxlength'     => 128,
    '#required'      => TRUE
  );
  $form['yubikey_valserver']['api']['https'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use HTTPs'),
    '#default_value' => variable_get('yubikey_valserver_api_https', FALSE)
  );
  $form['yubikey_valserver']['api']['timeout'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Timeout[s]'),
    '#description'   => t('How long to wait (in seconds) for an authentication response before the request times out.'),
    '#default_value' => variable_get('yubikey_valserver_api_timeout', 10),
    '#size'          => 6,
    '#maxlength'     => 128,
  );
  
  // Mail settings.
  $form['yubikey_mail'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('YubiKey e-mail settings')
  );

  // These email tokens are shared for all settings, so just define
  // the list once to help ensure they stay in sync.
  $email_token_help = t('Available variables are:') . ' !username, !site, !mailto, !date, !yubikey_lost_confirmation_url.';

  $form['yubikey_mail']['yubikey_lost'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Lost YubiKey confirmation email'),
    '#collapsible'   => TRUE,
    '#collapsed'     => $yk_collapsed,
    '#description'   => t('When a user reports loss of YubiKey, a customized e-mail message with a confirmation link is sent to the registered e-mail addressof the user.') . ' ' . $email_token_help,
  );
  $form['yubikey_mail']['yubikey_lost']['yubikey_mail_yubikey_lost_subject'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Subject'),
    '#default_value' => _yubikey_mail_text('yubikey_lost_subject'),
    '#maxlength'     => 180,
  );
  $form['yubikey_mail']['yubikey_lost']['yubikey_mail_yubikey_lost_body'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Body'),
    '#default_value' => _yubikey_mail_text('yubikey_lost_body'),
    '#rows'          => 10,
  );

  $form['yubikey_mail']['yubikey_lost_confirmed'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Loss of YubiKey reported email'),
    '#collapsible'   => TRUE,
    '#collapsed'     => $yk_collapsed,
    '#description'   => t('When a user confirms loss of YubiKey, all YubiKeys associated with the user account are blocked and a notification e-mail message is sent to the concerned people. Customize the notification message sent when a user confirms loss of YubiKey. Available variables are: !username, !site, !mailto, !date.'),
  );
  $form['yubikey_mail']['yubikey_lost_confirmed']['yubikey_mail_yubikey_lost_confirmed_recipients'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Recipients'),
    '#description'   => t('Example: \'webmaster@example.com\' or \'sales@example.com,support@example.com\'. To specify multiple recipients, separate each e-mail address with a comma.'),
    '#default_value' => _yubikey_mail_text('yubikey_lost_confirmed_recipients'),
    '#required'      => TRUE
  );
  $form['yubikey_mail']['yubikey_lost_confirmed']['yubikey_mail_yubikey_lost_confirmed_subject'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Subject'),
    '#default_value' => _yubikey_mail_text('yubikey_lost_confirmed_subject'),
    '#maxlength'     => 180,
  );
  $form['yubikey_mail']['yubikey_lost_confirmed']['yubikey_mail_yubikey_lost_confirmed_body'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Body'),
    '#default_value' => _yubikey_mail_text('yubikey_lost_confirmed_body'),
    '#rows'          => 5,
    '#suffix'        => '</div>'
  );

  return system_settings_form($form);
}

function yubikey_admin_settings_validate($form_id, &$form_state) {
  module_load_include('inc', 'yubikey');

  for ($i = 1; $i <= 5; $i++) {
    $url = trim($form_state['values']['yubikey_valserver']['urls'][$i]);
    if (!empty($url) && !yubikey_valid_url($url)) {
      form_set_error('yubikey_valserver][urls][' . $i, t('The URL #%d %url is not valid.', array('%d' => $i, '%url' => $url1)));
    }
  }

  if (empty($form_state['values']['yubikey_mail_yubikey_lost_confirmed_recipients'])) {
    form_set_error('yubikey_mail_yubikey_lost_confirmed_recipients', t('You must enter one or more recipients.'));
  } 
  else {
    $recipients = explode(',', $form_state['values']['yubikey_mail_yubikey_lost_confirmed_recipients']);
    foreach ($recipients as $recipient) {
      if (!valid_email_address(trim($recipient))) {
        form_set_error('yubikey_mail_yubikey_lost_confirmed_recipients', t('%recipient is an invalid e-mail address.', array('%recipient' => $recipient)));
      }
    }
  }
}

function yubikey_admin_settings_submit($form_id, &$form_state) {
  if ($form_state['values']['op'] == t('Reset to defaults')) {
    variable_del('yubikey_enabled');
    variable_del('yubikey_authscheme');
    variable_del('yubikey_optional');
    variable_del('yubikey_optional_if_lost');
    variable_del('yubikey_valserver_urls_type');
    variable_del('yubikey_valserver_urls_1');
    variable_del('yubikey_valserver_urls_2');
    variable_del('yubikey_valserver_urls_3');
    variable_del('yubikey_valserver_urls_4');
    variable_del('yubikey_valserver_urls_5');
    variable_del('yubikey_valserver_urls');
    variable_del('yubikey_valserver_api_key');
    variable_del('yubikey_valserver_api_id');
    variable_del('yubikey_valserver_api_https');
    variable_del('yubikey_valserver_api_timeout');
    variable_del('yubikey_mail_yubikey_lost_subject');
    variable_del('yubikey_mail_yubikey_lost_body');
    variable_del('yubikey_mail_yubikey_lost_confirmed_recipients');
    variable_del('yubikey_mail_yubikey_lost_confirmed_subject');
    variable_del('yubikey_mail_yubikey_lost_confirmed_body');
  } 
  else {
    module_load_include('inc', 'yubikey');

    $yk_enabled = $form_state['values']['yubikey_enabled'];
    $yk_authscheme = $form_state['values']['yubikey_authscheme'];
    $yk_optional = $form_state['values']['yubikey_optional'];
    $yk_optional_if_lost = $form_state['values']['yubikey_optional_if_lost'];
    $yk_valserver_urls_type = $form_state['values']['yubikey_valserver']['urls']['type'];
    $yk_valserver_urls_1 = yubikey_clean_url($form_state['values']['yubikey_valserver']['urls']['1']);
    $yk_valserver_urls_2 = yubikey_clean_url($form_state['values']['yubikey_valserver']['urls']['2']);
    $yk_valserver_urls_3 = yubikey_clean_url($form_state['values']['yubikey_valserver']['urls']['3']);
    $yk_valserver_urls_4 = yubikey_clean_url($form_state['values']['yubikey_valserver']['urls']['4']);
    $yk_valserver_urls_5 = yubikey_clean_url($form_state['values']['yubikey_valserver']['urls']['5']);
    $yk_valserver_api_key = $form_state['values']['yubikey_valserver']['api']['key'];
    $yk_valserver_api_id = $form_state['values']['yubikey_valserver']['api']['id'];
    $yk_valserver_api_https = $form_state['values']['yubikey_valserver']['api']['https'];
    $yk_valserver_api_timeout = $form_state['values']['yubikey_valserver']['api']['timeout'];

    $yk_valserver_urls  = trim($yk_valserver_urls_1);
    $yk_valserver_urls .= "," . trim($yk_valserver_urls_2);
    $yk_valserver_urls .= "," . trim($yk_valserver_urls_3);
    $yk_valserver_urls .= "," . trim($yk_valserver_urls_4);
    $yk_valserver_urls .= "," . trim($yk_valserver_urls_5);
    $yk_valserver_urls = str_replace(",,", ",", $yk_valserver_urls);

    $recipients = explode(',', $form_state['values']['yubikey_mail_yubikey_lost_confirmed_recipients']);
    foreach ($recipients as $key => $recipient) {
      $recipients[$key] = trim($recipient);
    }
    $form_state['values']['yubikey_mail_yubikey_lost_confirmed_recipients'] = implode(',', $recipients);

    variable_set('yubikey_enabled', $yk_enabled);
    variable_set('yubikey_authscheme', $yk_authscheme);
    variable_set('yubikey_optional', $yk_optional);
    variable_set('yubikey_optional_if_lost', $yk_optional_if_lost);
    variable_set('yubikey_valserver_urls_type', $yk_valserver_urls_type);
    variable_set('yubikey_valserver_urls_1', $yk_valserver_urls_1);
    variable_set('yubikey_valserver_urls_2', $yk_valserver_urls_2);
    variable_set('yubikey_valserver_urls_3', $yk_valserver_urls_3);
    variable_set('yubikey_valserver_urls_4', $yk_valserver_urls_4);
    variable_set('yubikey_valserver_urls_5', $yk_valserver_urls_5);
    variable_set('yubikey_valserver_urls', $yk_valserver_urls);
    variable_set('yubikey_valserver_api_key', $yk_valserver_api_key);
    variable_set('yubikey_valserver_api_id', $yk_valserver_api_id);
    variable_set('yubikey_valserver_api_https', $yk_valserver_api_https);
    variable_set('yubikey_valserver_api_timeout', $yk_valserver_api_timeout);
  }
}
