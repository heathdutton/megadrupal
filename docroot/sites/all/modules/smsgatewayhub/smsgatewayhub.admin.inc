<?php

/**
 * @file
 * Administrative tasks in SMS gateway hub.
 */

/**
 * Send test sms form.
 */
function smsgatewayhub_send_test_sms($form, &$form_state) {
  $form = array();

  $form['numbers'] = array(
    '#type' => 'textfield',
    '#title' => t('Number'),
    '#description' => t('Enter mobile number where you want to send sms. You can provide multiple mobile numbers separated with comma. For example: %example1 OR %example2. Please see the limit of mobile numbers from !smsgatewayhub_site', array(
      '%example1' => '7722061990',
      '%example2' => '7722061990,7729111960,7707091951',
      '!smsgatewayhub_site' => l(t('smsgatewayhub'), 'http://www.smsgatewayhub.com'),
    )),
  );

  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#description' => t('Enter test message. Message length by number of sms proportion can be seen in !smsgatewayhub_site', array(
      '!smsgatewayhub_site' => l(t('smsgatewayhub'), 'http://www.smsgatewayhub.com'),
    )),
  );

  $form['flash'] = array(
    '#type' => 'checkbox',
    '#title' => t('Flash message'),
    '#description' => t('If checked the sms will appear as a flash message. For more details see !smsgatewayhub_site', array(
      '!smsgatewayhub_site' => l(t('smsgatewayhub'), 'http://www.smsgatewayhub.com'),
    )),
  );

  $form['transactional'] = array(
    '#type' => 'checkbox',
    '#title' => t('Transactional SMS'),
    '#description' => t('If checked SMS will be sent as transactional SMS, otherwise sent as promotional SMS. Note that you need to be authorized for sending transactional SMS, also check templates available for transactional SMS. See !transactional_sms and !promotional_sms here', array(
      '!transactional_sms' => l(t('transactional SMS'), 'http://www.smsgatewayhub.com/transactional-sms'),
      '!promotional_sms' => l(t('promotional SMS'), 'http://www.smsgatewayhub.com/promotional-sms'),
    )),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send test message'),
  );

  return $form;
}

/**
 * Submit callback for send test sms form.
 */
function smsgatewayhub_send_test_sms_submit($form, &$form_state) {
  smsgatewayhub_send_sms($form_state['values']['numbers'], $form_state['values']['message'], $form_state['values']['flash'], $form_state['values']['transactional']);
}

/**
 * SMS Gateway Hub settings.
 */
function smsgatewayhub_settings($form, &$form_state) {
  $form = array();

  $form['smsgatewayhub_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Enter your SMS Gateway Hub username'),
    '#default_value' => variable_get('smsgatewayhub_username', ''),
    '#required' => TRUE,
  );

  $form['smsgatewayhub_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('Enter your SMS Gateway Hub password'),
  );

  $form['smsgatewayhub_senderid'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender ID'),
    '#description' => t('Enter your SMS Gateway Hub sender ID. You can check your sender ID from !smsgatewayhub_senderid_link', array(
      '!smsgatewayhub_senderid_link' => l(t('smsgatewayhub'), 'http://api.smsgatewayhub.com/Apps/SMS/ManageSenderId.aspx#Template-menu'),
    )),
    '#default_value' => variable_get('smsgatewayhub_senderid', ''),
    '#required' => TRUE,
  );

  $form['smsgatewayhub_log'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log SMS delivery'),
    '#description' => t('Check this if you want to log SMS delivery report'),
    '#default_value' => variable_get('smsgatewayhub_log', 1),
  );

  // Since there is no default value for password, so we have to handle it
  // manually.
  // Make sure password is not lost, if left empty.
  // See https://www.drupal.org/node/486544#comment-3286740
  $form['#submit'][] = 'smsgatewayhub_settings_edit_password';

  return system_settings_form($form);
}

/**
 * Make sure password is not lost, if left empty.
 */
function smsgatewayhub_settings_edit_password($form, &$form_state) {
  if (empty($form_state['values']['smsgatewayhub_password'])) {
    // Unset password from form state so that password is not overridden.
    unset($form_state['values']['smsgatewayhub_password']);
  }
}

/**
 * SMS delivery report.
 *
 * @ingroup forms
 */
function smsgatewayhub_delivery_report($form, &$form_state) {
  $form = array();

  $form['message_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Message Id'),
    '#description' => t('Received message id after sending message'),
  );

  $form['report_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Report'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if (!empty($form_state['storage']['report'])) {
    $form['report_wrapper']['#collapsed'] = FALSE;
    $form['report_wrapper']['report'] = array(
      '#markup' => $form_state['storage']['report'],
    );
  }
  else {
    $form['report_wrapper']['report'] = array(
      '#markup' => '',
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate report'),
  );

  return $form;
}

/**
 * Generate SMS delivery report.
 */
function smsgatewayhub_delivery_report_submit($form, &$form_state) {
  $data = array(
    'user' => variable_get('smsgatewayhub_username', ''),
    'password' => variable_get('smsgatewayhub_password', ''),
    'messageId' => $form_state['values']['message_id'],
  );

  $url = url('http://api.smsgatewayhub.com/smsapi/checkdelivery.aspx', array('query' => $data));

  $response = drupal_http_request($url);

  if ($response->status_message == 'OK') {
    switch ($response->data) {
      case '#Sub-SMSC':
        $form_state['storage']['report'] = t('The message is on SMSC queue i.e. the message has been inserted into the SMSC database but the status of the message is yet to be received.');
        break;

      case '#DELIVRD':
        $form_state['storage']['report'] = t('Successfully delivered.');
        break;

      case '#FAILED':
        $form_state['storage']['report'] = t('The message is permanently failed due to CallBarred, Error in Destination Number, Error in TeleService Provider etc.');
        break;

      case '#NDNC_Failed':
        $form_state['storage']['report'] = t('Failed due to DND registration.');
        break;

      case '#Blacklist':
        $form_state['storage']['report'] = t('Black listed number. This list of numbers is provided by the Customer and includes numbers of CEO etc. A message will never go to a number in the black-list.');
        break;

      case '#Whitelist':
        $form_state['storage']['report'] = t('Opt-In account sends messages to a non-white listed number');
        break;

      case '#Invalid Series':
        $form_state['storage']['report'] = t('Number series in the correct number format is invalid.');
        break;

      case '#Prepaid Reject':
        $form_state['storage']['report'] = t('Message rejected due to insufficient credits.');
        break;

      case '#Night Expiry':
        $form_state['storage']['report'] = t('Promotional messages blocked after 9 PM');
        break;

      case '#EXP-AbsSubs':
        $form_state['storage']['report'] = t('No paging response. The IMSI record is marked detached, or the MS is subject to roaming restrictions.');
        break;

      case '#EXP-MEM-EXCD':
        $form_state['storage']['report'] = t('Message rejected because the MS does not have enough memory.');
        break;

      case '#EXP-NW-FAIL':
        $form_state['storage']['report'] = t('Message rejected due to network failure.');
        break;

      case '#EXP-NW-TMOUT':
        $form_state['storage']['report'] = t('Message rejected due to network or protocol failure.');
        break;

      case '#EXP-SMS-TMOUT':
        $form_state['storage']['report'] = t('Message rejected due to network or protocol failure.');
        break;

      case '#EXP-HDST-BUSY':
        $form_state['storage']['report'] = t('The message is rejected because of congestion encountered at the visited MSC.');
        break;

      case '#EXP-MSG-Q-EXD':
        $form_state['storage']['report'] = t('Message queue exceeded when there are too many messages for one number. SMSC can deliver only a particular number of messages to a mobile number. If there are more messages, they get this error code till the queue clears.');
        break;

      case '#Promo_blocked':
        $form_state['storage']['report'] = t('Promotional messages blocked.');
        break;

      case '#PENDG-ABS-SUB':
        $form_state['storage']['report'] = t('The message is rejected because there was no paging response, the IMSI record is marked detached, or the MS is subject to roaming restrictions at the first attempt.');
        break;

      case '#PENDG-MEM-EXCD':
        $form_state['storage']['report'] = t('Message rejected because the MS does not have enough memory when at the first attempt.');
        break;

      case '#PENDG-NW-FAILR':
        $form_state['storage']['report'] = t('Message rejected due to network failure at the first attempt.');
        break;

      case '#PENDG-NW-TMOUT':
        $form_state['storage']['report'] = t('Message rejected due to network or protocol failure at the first attempt.');
        break;

      case '#PENDG-SMS-TMOUT':
        $form_state['storage']['report'] = t('Message rejected due to network or protocol failure at the first attempt.');
        break;

      case '#PENDG-HDST-BUSY':
        $form_state['storage']['report'] = t('The message is rejected because of congestion encountered at the visited MSC at first attempt.');
        break;

      case '#REMOTE NODE NOT REACHABLE':
        $form_state['storage']['report'] = t('Remote HLR or VLR route is not defined on adjacent node.');
        break;

      case '#user Abort':
        $form_state['storage']['report'] = t('TCAP protocol abort received from one of the node on the network.');
        break;

      case '#':
        $form_state['storage']['report'] = t('Invalid message id.');
        break;
    }
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * Returns credit balance information.
 *
 * @ingroup forms
 */
function smsgatewayhub_credit_balance($form, &$form_state) {
  $form = array();
  $result = array();
  $data = array(
    'user' => variable_get('smsgatewayhub_username', ''),
    'password' => variable_get('smsgatewayhub_password', ''),
  );

  // Promotional balance.
  $data['gwid'] = 1;
  $url = url('http://api.smsgatewayhub.com/smsapi/CheckBalance.aspx', array('query' => $data));
  $response = drupal_http_request($url);

  if ($response->status_message == 'OK') {
    $return_data = explode('#', $response->data);
    $result['promotional'] = t('%count left', array('%count' => $return_data[1]));
  }
  else {
    $result['promotional'] = t('Error processing request');
  }

  // Transactional balance.
  $data['gwid'] = 2;
  $url = url('http://api.smsgatewayhub.com/smsapi/CheckBalance.aspx', array('query' => $data));
  $response = drupal_http_request($url);

  if ($response->status_message == 'OK') {
    $return_data = explode('#', $response->data);
    $result['transactional'] = t('%count left', array('%count' => $return_data[1]));
  }
  else {
    $result['transactional'] = t('Error processing request');
  }

  $form['promotional_report'] = array(
    '#type' => 'fieldset',
    '#title' => t('Promotional'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['promotional_report']['report'] = array(
    '#markup' => $result['promotional'],
  );

  $form['transactional_report'] = array(
    '#type' => 'fieldset',
    '#title' => t('Transactional'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['transactional_report']['report'] = array(
    '#markup' => $result['transactional'],
  );

  return $form;
}
