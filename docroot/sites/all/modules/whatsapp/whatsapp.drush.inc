<?php

/**
 * @file
 * drush integration for WhatsApp.
 */

require_once('includes/WhatsappController.php');

/**
 * Implements hook_drush_command().
 */
function whatsapp_drush_command() {
  $items = array();

  $items['whatsapp-init'] = array(
    'callback' => 'drush_whatsapp_init',
    'description' => "Configure your exists credentials.",
    'arguments' => array(
      'phone' => 'Telephone number including the country code without "+" or "00". For instance: 34666555444.',
      'identifier' => "This is tipically the IMEI number. If you are using an iOS device you should input your WLAN MAC address.",
    ),
    'options' => array(
      'name' => array(
        'description' => 'This is the username displayed by WhatsApp clients. Defaults site name.',
      ),
      'status' => array(
        'description' => 'Real time indication of what you are doing, so that you can keep up to date with your contacts. Defaults "Using Drupal WhatsApp!".',
      ),
    ),
    'examples' => array(
      'drush wi 34666554433 00:00:00:00:00:00 --name=Drupal --status="On-Line on Drupal WhatsApp!"' => 'Set your account settings.',
    ),
    'aliases' => array('wi'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $items['whatsapp-request-code'] = array(
    'callback' => 'drush_whatsapp_request_code',
    'description' => "Request a registration code from WhatsApp.",
    'arguments' => array(
      'phone' => 'Telephone number including the country code without "+" or "00". For instance: 34666555444.',
      'identifier' => "This is tipically the !IMEI number. If you are using an iOS device you should input your WLAN MAC address.",
    ),
    'options' => array(
      'method' => array(
        'description' => 'The method for request a registration code from WhatsApp, SMS o PHONE CALL (sms|voice). Defaults SMS',
      ),
      'country-code' => array(
        'description' => 'ISO Country Code, 2 Digit. Default "US".',
      ),
      'lang-code' => array(
        'description' => 'ISO 639-1 Language Code: two-letter codes. Default "en".',
      ),
      'name' => array(
        'description' => 'This is the username displayed by WhatsApp clients. Defaults site name.',
      ),
      'status' => array(
        'description' => 'Real time indication of what you are doing, so that you can keep up to date with your contacts. Defaults "Using Drupal WhatsApp!".',
      ),
    ),
    'examples' => array(
      'drush wc 34666554433 00:00:00:00:00:00 --method=sms --country-code=ES --lang-code=es --name=Drupal --status="On-Line on Drupal WhatsApp!"' => 'Send you a registration code from WhatsApp.',
    ),
    'aliases' => array('wc'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $items['whatsapp-register-code'] = array(
    'callback' => 'drush_whatsapp_register_code',
    'description' => "Register your account, sending the 6-digit activation code.",
    'arguments' => array(
      'code' => 'The 6-digit activation code',
    ),
    'examples' => array(
      "drush wr 000000" => 'Register your account.',
    ),
    'aliases' => array('wr'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $items['whatsapp-status'] = array(
    'callback' => 'drush_whatsapp_status',
    'description' => "Check WhatsApp credentials.",
    'examples' => array(
      "drush wst" => 'Check your WhatsApp credentials.',
    ),
    'aliases' => array('wst'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $items['whatsapp-send-message'] = array(
    'callback' => 'drush_whatsapp_send_message',
    'description' => "Send a WhatsApp message.",
    'arguments' => array(
      'destination' => 'Destination telephone number including the country code without "+" or "00". For instance: 34666555444.',
      'message' => "The text message that will be sent to the destination.",
    ),
    'examples' => array(
      'drush wsm 34666554433 "Hello Drupal WhatsApp!"' => 'Senda a "Hello Drupal WhatsApp!" message to 34666554433.',
    ),
    'aliases' => array('wsm'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $items['whatsapp-start'] = array(
    'callback' => 'drush_whatsapp_start',
    'description' => 'Start the WhatsApp connection.',
    'examples' => array(
      'drush whatsapp-start &'       => 'Start WhatsApp as a background process.',
      'nohup drush whatsapp-start &' => 'Start WhatsApp as a background process using nohup.',
    ),
    'aliases' => array('wstart'),
  );

  $items['whatsapp-status-reset'] = array(
    'callback' => 'drush_bot_status_reset',
    'description' => 'Resets the connection state of WhatsApp',
    'aliases' => array('wreset'),
  );

  return $items;
}

/**
 * Implement hook_drush_help().
 */
function whatsapp_drush_help($section) {
  switch ($section) {
    case 'drush:whatsapp-init':
      return dt('Configure your exists credentials.');
    case 'drush:whatsapp-request-code':
      return dt('Request a registration code from WhatsApp.');
    case 'drush:whatsapp-register-code':
      return dt('Register your account, sending the 6-digit activation code.');
    case 'drush:whatsapp-status':
      return dt('Check your WhatsApp credentials.');
    case 'drush:whatsapp-send-message':
      return dt('Send a WhatsApp message.');
  }
}

/**
 * COMMAND: Configure your exists credentials.
 */
function drush_whatsapp_init($phone, $identifier) {
  $wc = new WhatsappController();
  $dissectPhone = $wc->get_container('util')->dissect($phone);
  if (!$dissectPhone) {
    drush_set_error(dt('The phone number is not valid.'));
  }
  else {
    if (!$name = drush_get_option('name')) {
      $name = variable_get('site_name', 'Drupal WhatsApp');
    }
    if (!$status = drush_get_option('status')) {
      $status = variable_get('whatsapp_sitewide_status', 'On-Line on Drupal WhatsApp!');
    }
    variable_set('whatsapp_sitewide_phone', $phone);
    variable_set('whatsapp_sitewide_identifier', $identifier);
    variable_set('whatsapp_sitewide_username', $name);
    variable_set('whatsapp_sitewide_status', $status);
    drush_log(dt("Your WhatsApp account credentials has been saved.", 'success'));
  }
}

/**
 * COMMAND: Request a registration code from WhatsApp.
 */
function drush_whatsapp_request_code($phone, $identifier) {
  $wc = new WhatsappController();
  $dissectPhone = $wc->get_container('util')->dissect($phone);
  if (!$dissectPhone) {
    drush_set_error(dt('The phone number is not valid.'));
  }
  else {
    if (!$method = drush_get_option('method')) {
      $method = 'sms';
    }
    else {
      $method = drupal_strtolower($method);
      if ($method != 'sms' && $method != 'voice') {
        drush_set_error(dt('Only sms|voice methods are corrects.'));
        return;
      }
    }
    // @todo: Validate country code.
    if (!$country_code = drush_get_option('country-code')) {
      $country_code = 'US';
    }
    // @todo: Validate lang code.
    if (!$name = drush_get_option('lang-code')) {
      $lang_code = 'en';
    }
    if (!$name = drush_get_option('name')) {
      $name = variable_get('site_name', 'Drupal WhatsApp');
    }
    if (!$status = drush_get_option('status')) {
      $status = variable_get('whatsapp_sitewide_status', 'On-Line on Drupal WhatsApp!');
    }

    variable_set('whatsapp_sitewide_phone', $phone);
    variable_set('whatsapp_sitewide_identifier', $identifier);
    variable_set('whatsapp_sitewide_username', $name);
    variable_set('whatsapp_sitewide_status', $status);

    $wc->setPhoneCountryCode($phone['cc']);
    $wc->setPhoneNumber($phone['phone']);
    $wc->setIdentity($identifier);
    $wc->setUserName($name);
    $wc->setUserStatus($status);
    $wc->setCountryCode($country_code);
    $wc->setLangCode($lang_code);
    $rest = $wc->requestCode($method);

    if ($rest->status == 'sent') {
      watchdog('whatsapp', 'Request a registration code from WhatsApp');
      drush_log(dt("WhatsApp sent you an @method with a code. To complete your account registration, please enter the 6-digit activation code.", array('@method' => $form_state['values']['method'] == 'sms' ? t('SMS') : t('Call'))), 'success');
    }
    else {
      watchdog('whatsapp', 'An error occurred requesting the registration code from WhatsApp: @reason@param', array('@reason' => $rest->reason, '@param' => isset($rest->param)?': ' . $rest->param : ''), WATCHDOG_ERROR);
      drush_set_error(dt('An error occurred requesting the registration code from WhatsApp: @reason@param', array('@reason' => $rest->reason, '@param' => isset($rest->param)?': ' . $rest->param : '')));
      if (isset($rest->retry_after)) {
        drush_set_error(dt('Retry again after @seconds seconds.', array('@seconds' => $rest->retry_after)));
      }
    }
  }
}

/**
 * COMMAND: Register your account.
 */
function drush_whatsapp_register_code($code) {
  if (strlen($code) != 6) {
    drush_set_error(dt('The registration code is not 6-digit.'));
    return;
  }
  else {
    $wc = new WhatsappController();
    $rest = $wc->registerCode($code);

    if ($rest->status == 'ok') {
      watchdog('whatsapp', 'The registration code has been successfully completed');
      drush_log(dt("The registration code has been successfully completed."), 'success');
    }
    else {
      watchdog('whatsapp', 'An error occurred registering the registration code from WhatsApp: @reason@param', array('@reason' => $rest->reason, '@param' => isset($rest->param)?': ' . $rest->param : ''), WATCHDOG_ERROR);
      drush_set_error(dt('An error occurred registering the registration code from WhatsApp: @reason@param', array('@reason' => $rest->reason, '@param' => isset($rest->param)?': ' . $rest->param : '')));
    }
  }
}

/**
 * COMMAND: Check your WhatsApp credentials.
 */
function drush_whatsapp_status() {
  $wc = new WhatsappController();
  $rest = $wc->checkCredentials();
  if (!empty($rest)) {
    foreach ($rest as $key => $value) {
      if ($key == 'expiration' || $key == 'price_expiration') {
        $value = format_date($value, 'long');
      }
      drush_print(dt('Your WhatsApp @var is @value.', array('@var' => $key, '@value' => $value)));
    }
    if ($rest->status == 'ok') {
      drush_log(dt('Your credentials are good.'), 'success');
    }
    else {
      drush_set_error(dt('Your credentias are invalids.'));
    }
  }
}

/**
 * COMMAND: Send a WhatsApp message.
 */
function drush_whatsapp_send_message($destination, $message) {
  $wc = new WhatsappController();
  $dissectPhone = $wc->get_container('util')->dissect($destination);
  if (!$dissectPhone) {
    drush_set_error(dt('The phone number is not valid.'));
  }
  else if (empty($message)) {
    drush_set_error(dt('Message field can not be empty.'));
  }
  else {
    $wc = new WhatsappController();
    $wc->init();

    $success = FALSE;
    $wc->setDestination($destination);
    $success = $wc->sendMessage($message);

    if ($success) {
      $wc->WaitforReceipt();
      drush_log(dt('Your message has been sent.'), 'success');
    }
    else {
      drush_set_error(dt('Your message has not been sent.'));
    }
  }
}

/**
 * COMMAND: Connect WhatsApp.
 */
function drush_whatsapp_start() {
  if (variable_get('whatsapp_status', 'DISCONNECTED') == 'DISCONNECTED') {
    require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'whatsapp') . '/bot.smartconnection.inc';
    whatsapp_smartconnection_start();
  }

  drush_log(dt('The WhatsApp is already connected. If this is wrong, try "drush whatsapp-status-reset".'), 'error');
}

/**
 * COMMAND: Reset WhatsApp status connection.
 */
function drush_whatsapp_status_reset() {
  if (drush_confirm(dt("Are you sure you want to reset the WhatsApp's connect status?"))) {
    variable_set('whatsapp_status', 'DISCONNECTED');
    drush_log(dt("The WhatsApp's connection status has been reset."), 'ok');
  }
}
