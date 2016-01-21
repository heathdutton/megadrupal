<?php

/**
 * @file
 * GovDelivery TMS admin UI.
 */

/**
 * Constructs settings form.
 *
 * @return array
 *   Form data.
 */
function govdelivery_settings($form, &$form_state) {
  $govdelivery_tms_settings = variable_get('govdelivery_tms_settings', array());
  $govdelivery_subscription_settings = variable_get('govdelivery_subscription_settings', array());

  $account_fieldset = array(
    '#type' => 'fieldset',
    '#title' => t('Mail account'),
  );
  if (isset($govdelivery_tms_settings) && isset($govdelivery_tms_settings['accounts']) && is_array($govdelivery_tms_settings['accounts'])) {
    foreach ($govdelivery_tms_settings['accounts'] as $username => $account_settings) {
      $account_fieldset[$username . '%###%fromname'] = array(
        '#type' => "textfield",
        '#title' => t('From name'),
        '#default_value' => check_plain((!empty($account_settings['fromname']) ? $account_settings['fromname'] : '')),
        '#description' => t('The name displayed in the From field of the received email. E.G. John Smith.'),
      );
      $account_fieldset[$username . '%###%username'] = array(
        '#type' => "hidden",
        '#title' => t('Username'),
        // Hardcoded username
        '#default_value' => t('gd_drupal_tms'),
      );
      $account_fieldset[$username . '%###%password'] = array(
        '#type' => "hidden",
        '#title' => t('Password'),
        '#default_value' => t('gd_drupal_tms'),
      );
    }
  }
  else {
    $account_fieldset['fromname'] = array(
      '#type' => "textfield",
      '#title' => t('From name'),
      '#description' => t('The name displayed in the From field of the received email. E.G. John Smith.'),
    );
    $account_fieldset['username'] = array(
      '#type' => "hidden",
      '#title' => t('Username'),
      '#value' => t('gd_drupal_tms'),
    );
    $account_fieldset['password'] = array(
      '#type' => "hidden",
      '#title' => t('Password'),
      '#value' => t('gd_drupal_tms'),
    );
  }

// Commenting out subscription form for now until usage is clearer.
  // $subscription_fieldset = array(
  //   '#type' => 'fieldset',
  //   '#title' => t('Subscription API'),
  //   '#tree' => TRUE,
  // );
  // $subscription_fieldset['service_url'] = array(
  //   '#type' => "textfield",
  //   '#title' => t('URL for subscription service'),
  //   '#default_value' => (!empty($govdelivery_subscription_settings['service_url']) ? check_url($govdelivery_subscription_settings['service_url']) : ''),
  // );
  // $subscription_fieldset['cron_subscriptions'] = array(
  //   '#type' => 'radios',
  //   '#default_value' => (isset($govdelivery_subscription_settings['cron_subscriptions']) ? $govdelivery_subscription_settings['cron_subscriptions'] : 1),
  //   '#title' => t('Subscriptions cron'),
  //   '#options' => array('Disabled', 'Enabled'),
  //   '#description' => t('Will automatically resend the subscriptions queue on cron runs.'),
  // );
  // $subscription_fieldset['subscriber_api_url_base'] = array(
  //   '#type' => 'textfield',
  //   '#title' => t('Base URL for Subscriber API'),
  //   '#default_value' => (!empty($govdelivery_subscription_settings['subscriber_api_url_base']) ? check_url($govdelivery_subscription_settings['subscriber_api_url_base']) : ''),
  // );
  // $subscription_fieldset['default_topic_category_id'] = array(
  //   '#type' => 'textfield',
  //   '#title' => t('Default category assigned to new topics'),
  //   '#default_value' => (!empty($govdelivery_subscription_settings['default_topic_category_id']) ? $govdelivery_subscription_settings['default_topic_category_id'] : ''),
  // );

  $form = array(
    'govdelivery_tms_settings' => array(
      '#type' => 'fieldset',
      '#title' => t('Mail server settings'),
      '#tree' => TRUE,
      'accounts' => $account_fieldset,
      // 'default_username' => array(
      //   '#type' => "textfield",
      //   '#title' => 'Default mail username',
      //   '#default_value' => (!empty($govdelivery_tms_settings['default_username']) ? check_url($govdelivery_tms_settings['default_username']) : ''),
      //   '#description' => t('The username of the default mail account to use.'),
      // ),
      'server' => array(
        '#type' => "textfield",
        '#title' => 'Server',
        '#default_value' => (!empty($govdelivery_tms_settings['server']) ? check_url($govdelivery_tms_settings['server']) : ''),
        '#description' => t('Enter the URL of your GovDelivery TMS instance. It must begin with "https." Example: https://yourserver.govdelivery.com'),
      ),
      'auth_token' => array(
        '#type' => "textfield",
        '#title' => t('Auth token'),
        '#default_value' => (!empty($govdelivery_tms_settings['auth_token']) ? $govdelivery_tms_settings['auth_token'] : ''),
        '#description' => t('Enter the authentication token needed to send emails through your GovDelivery TMS service. If you do not have an authentication token, please content your GovDelivery account representative.'),
      ),
      'enabled' => array(
        '#type' => 'radios',
        '#default_value' => (isset($govdelivery_tms_settings['enabled']) ? $govdelivery_tms_settings['enabled'] : 1),
        '#title' => t('Use TMS for Outbound Mail'),
        '#options' => array('Disabled', 'Enabled'),
        '#description' => t('If this option is enabled, emails from your Drupal site will be sent using GovDelivery\'s Transactional Message Service (TMS). If it is disabled, emails we be sent using Drupal\'s standard SMTP-based email system.'),
      ),
      'queue' => array(
        '#type' => 'radios',
        '#default_value' => (isset($govdelivery_tms_settings['queue']) ? $govdelivery_tms_settings['queue'] : 0),
        '#title' => t('Queue Mail for High Volume'),
        '#options' => array('Disabled', 'Enabled'),
        '#description' => t('If this option is enabled, messages will be placed in a queue for delivery rather than be sent immediately. (Messages that fail to send will be queued anyway for later delivery.)'),
      ),
      'cron_tms' => array(
        '#type' => 'radios',
        '#default_value' => (isset($govdelivery_tms_settings['cron_tms']) ? $govdelivery_tms_settings['cron_tms'] : 0),
        '#title' => t('Messages cron'),
        '#options' => array('Disabled', 'Enabled'),
        '#description' => t('Will automatically resend the messages queue when cron runs.'),
      ),
      'override_from' => array(
        '#type' => 'radios',
        '#default_value' => (isset($govdelivery_tms_settings['override_from']) ? $govdelivery_tms_settings['override_from'] : 0),
        '#title' => t('Override the From address on outgoing messages and failback on the predefined From address in this module'),
        '#options' => array('Disabled', 'Enabled'),
        '#description' => t('This will allow webforms and other modules to configure the From address.'),
      ),
      'max_bid' => array(
        '#type' => 'textfield',
        '#title' => t('Maximum bins used with queue asynchronous processing'),
        '#default_value' => (isset($govdelivery_tms_settings['max_bid']) ? $govdelivery_tms_settings['max_bid'] : 5),
      ),
      'external_cron_interval' => array(
        '#type' => 'textfield',
        '#title' => t('External cron interval (in seconds)'),
        '#default_value' => (isset($govdelivery_tms_settings['external_cron_interval']) ? $govdelivery_tms_settings['external_cron_interval'] : 120),
      ),
    ),
    // Commenting out the GovDelivery subscription settings for now.
    // TODO -- re-enable these and document the functionality properly.
    // 'govdelivery_subscription_settings' => $subscription_fieldset,
    'save' => array(
      '#type' => 'submit',
      '#value' => 'Save configuration',
    ),
  );
  return $form;
}

/**
 * Handles settings form submission.
 *
 * @param array $form
 *   Form data.
 * @param array $postinfo
 *   POST info.
 */
function govdelivery_settings_submit($form, $postinfo) {
  $govdelivery_tms_settings = variable_get('govdelivery_tms_settings', array());
  $govdelivery_subscription_settings = variable_get('govdelivery_subscription_settings', array());
  $account_settings = isset($govdelivery_tms_settings['accounts']) ? $govdelivery_tms_settings['accounts'] : array();
  $new_account = array();
  // Change the account storage to an associative array indexed by username.
  $accounts_posted = $postinfo['values']['govdelivery_tms_settings']['accounts'];
  foreach ($accounts_posted as $composite_key => $value) {
    // If we don't have a value we don't want to do anything.
    if (isset($value) && !empty($value)) {
      $key_elements = explode('%###%', $composite_key);
      if (count($key_elements) == 2) {
        // Hardcoded username
        $username = 'gd_drupal_tms';
        $field = $key_elements[1];
        if (!isset($account_settings[$username]) && isset($username) && !empty($username)) {
          $account_settings[$username] = array();
        }
        if ($field != 'username') {
          $account_settings[$username][$field] = $value;
        }
      }
      else {
        if (isset($key_elements[0])) {
          $new_account[$key_elements[0]] = $value;
        }
      }
    }
  }

  if (isset($new_account['username'])) {
    $account_settings[$new_account['username']] = array(
      "password" => $new_account['password'],
      "fromname" => $new_account['fromname'],
    );
  }
  $govdelivery_tms_settings['accounts'] = $account_settings;
  // Default username for mail accounts.
  $govdelivery_tms_settings['default_username'] = 'gd_drupal_tms';
  // Server.
  $server = $postinfo['values']['govdelivery_tms_settings']['server'];
  $govdelivery_tms_settings['server'] = check_url($server);

  // REST auth token.
  $govdelivery_tms_settings['auth_token'] = $postinfo['values']['govdelivery_tms_settings']['auth_token'];

  // Temporarily comment out Subscription URL handling.
  // TODO -- restore this functionality.
/*
  // Subscription URL.
  $service_url = $postinfo['values']['govdelivery_subscription_settings']['service_url'];
  $govdelivery_subscription_settings['service_url'] = check_url($service_url);

  // Subscriptions Cron.
  $govdelivery_subscription_settings['cron_subscriptions'] = $postinfo['values']['govdelivery_subscription_settings']['cron_subscriptions'];

  // Subscriber API Base URL.
  $govdelivery_subscription_settings['subscriber_api_url_base'] = $postinfo['values']['govdelivery_subscription_settings']['subscriber_api_url_base'];

  // Default Category ID when creating new topics.
  $govdelivery_subscription_settings['default_topic_category_id'] = $postinfo['values']['govdelivery_subscription_settings']['default_topic_category_id'];
// */

  // These come in as array place value so 0 or 1.
  // Queue Messages.
  $govdelivery_tms_settings['queue'] = $postinfo['values']['govdelivery_tms_settings']['queue'];

  // Messages cron.
  $govdelivery_tms_settings['cron_tms'] = $postinfo['values']['govdelivery_tms_settings']['cron_tms'];

  // Override From address.
  $govdelivery_tms_settings['override_from'] = $postinfo['values']['govdelivery_tms_settings']['override_from'];

  // Asynchronous Processing Max BID.
  $govdelivery_tms_settings['max_bid'] = $postinfo['values']['govdelivery_tms_settings']['max_bid'];

  $govdelivery_tms_settings['external_cron_interval'] = $postinfo['values']['govdelivery_tms_settings']['external_cron_interval'];

  // Mail Routing.
  $govdelivery_tms_settings['enabled'] = $postinfo['values']['govdelivery_tms_settings']['enabled'];
  if ($govdelivery_tms_settings['enabled'] == 1) {
    $mail_system = variable_get('mail_system');
    $mail_system['default-system'] = 'GovDeliveryMailSystem';
    variable_set('mail_system', $mail_system);
  }
  else {
    $mail_system = variable_get('mail_system');
    if (isset($mail_system) && !empty($mail_system['default-system'])) {
      $mail_system['default-system'] = 'DefaultMailSystem';
      variable_set('mail_system', $mail_system);
    }
  }

  // Save the validated changes.
  variable_set('govdelivery_subscription_settings', $govdelivery_subscription_settings);
  variable_set('govdelivery_tms_settings', $govdelivery_tms_settings);
}

/**
 * Send a test message to the specificed addresses.
 *
 * @return array
 *   Form data.
 */
function govdelivery_test_settings($form, &$form_state) {
  $form = array(
    'govdelivery_test_settings' => array(
      '#type' => 'fieldset',
      '#title' => t('GovDelivery test settings'),
      '#tree' => TRUE,
      'from' => array(
        '#type' => 'textfield',
        '#title' => t('From address (optional)'),
      ),
      'recipients' => array(
        '#type' => 'textfield',
        '#title' => t("Recipient's e-mail (comma separated)"),
      ),
      'test' => array(
        '#type' => 'submit',
        '#value' => t('Send test message'),
      ),
    ),
  );

  return $form;
}

/**
 * Handle test settings form submission.
 */
function govdelivery_test_settings_submit($form, $postinfo) {
  $recipients = $postinfo['values']['govdelivery_test_settings']['recipients'];
  // Send message.
  if (isset($recipients) && !empty($recipients)) {
    $from = $postinfo['values']['govdelivery_test_settings']['from'];
    // Pass on the list to be sent.
    if (!empty($from)) {
      $status = govdelivery_send_test_message($recipients, $from);
    }
    else {
      $status = govdelivery_send_test_message($recipients);
    }
  }
  drupal_set_message(t('Your test message has been sent.'));
}

/**
 * Send a test message.
 *
 * @param string $send_to
 *   Email address to send to.
 *
 * @return array
 *   Email data.
 */
function govdelivery_send_test_message($send_to, $from = NULL) {
  return drupal_mail('govdelivery', 'test_id', $send_to, language_default(), array(), $from, TRUE);
}

/**
 * Signup an email address for a specified list.
 *
 * Pass any known questions and answers along.
 *
 * @return array
 *   Test subscriptions form.
 */
function govdelivery_test_subscriptions($form, &$form_state) {
  $form = array(
    'govdelivery_test_subscriptions' => array(
      '#type' => 'fieldset',
      '#title' => t('GovDelivery settings test - subscriptions'),
      '#tree' => TRUE,
      'subscriber' => array(
        '#type' => 'textfield',
        '#title' => t("Subscriber's e-mail"),
      ),
      'list_code' => array(
        '#type' => 'textfield',
        '#title' => t('Subscription list code'),
      ),
      // Use Queue option.
      'test' => array(
        '#type' => 'submit',
        '#value' => t('Send test message'),
      ),
    ),
  );
  return $form;
}

/**
 * Validate number of subscribers.
 *
 * Need validation that there is only one subscriber or change to handle
 * multiple subscriptions.
 */
function govdelivery_test_subscriptions_submit($form, $postinfo) {
  $subscriber = $postinfo['values']['govdelivery_test_subscriptions']['subscriber'];
  $list_code = $postinfo['values']['govdelivery_test_subscriptions']['list_code'];

  // Commented code:
  // if (isset($subscriber) && !empty($subscriber) &&
  // isset($list_code) && !empty($list_code)) {
  $status = govdelivery_subscribe($list_code, $subscriber, FALSE);
  // }
  drupal_set_message(t('%response', array('%response' => $status ? 'Subscription added.' : 'Subscription failed.')));
}
