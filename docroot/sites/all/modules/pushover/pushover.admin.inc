<?php
/**
 * @file
 * Settings for Pushover.
 */

/**
 * The settings form.
 */
function pushover_config() {
  $form = array();

  $form['pushover_api_key'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Pushover API key'),
    '#description'    => t("Register your API key at !url.", array('!url' => l(t('Pushover.net'), "https://pushover.net/apps/build"))),
    '#default_value'  => variable_get('pushover_api_key', ''),
    '#required'       => TRUE,
  );

  $form['pushover_test_user'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Pushover User Key'),
    '#description'    => t("Enter a user key to send a test notification. Get your user key at !url.", array('!url' => l(t('Pushover.net'), "https://pushover.net"))),
    '#default_value'  => variable_get('pushover_test_user', ''),
  );

  $form['pushover_high_priority_retry'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Retry interval in seconds'),
    '#default_value'  => variable_get('pushover_high_priority_retry', 60),
    '#description'    => t('When using "Priority 2" a reminder will be sent to the user every X seconds.'),
  );

  $form['pushover_high_priority_expire'] = array(
    '#type'           => 'textfield',
    '#title'          => t('When to expire in seconds'),
    '#default_value'  => variable_get('pushover_high_priority_expire', 1800),
    '#description'    => t('When using "Priority 2" this is how long the notification will be retried.'),
  );

  $form['pushover_test_link'] = array(
    '#type'           => 'button',
    '#value'          => t('Send test notification'),
    '#validate'       => array('pushover_test_notification'),
  );

  $form['#validate'][] = 'pushover_config_validate';

  return system_settings_form($form);
}

/**
 * Validate that only numbers for the high priority settings.
 */
function pushover_config_validate($form, &$form_state) {
  $retry = $form_state['values']['pushover_high_priority_retry'];
  $expire = $form_state['values']['pushover_high_priority_expire'];

  if (!is_numeric($retry)) {
    form_set_error('pushover_high_priority_retry', t('"Retry" must be number.'));
  }
  else if ($retry < 30) {
    form_set_error('pushover_high_priority_retry', t('"Retry" must be atleast %sec seconds.', array('%sec' => '30')));
  }

  if (!is_numeric($expire)) {
    form_set_error('pushover_high_priority_expire', t('"Expire" must be number.'));
  }
  else if ($expire > 86400) {
    form_set_error('pushover_high_priority_expire', t('"Expire" cannot be more than %sec seconds.', array('%sec' => '86400')));
  }
}

/**
 * The receipts callback page.
 */
function pushover_receipts() {
  $receipts = variable_get('pushover_receipts', array());
  $token = variable_get('pushover_api_key', '');

  $header = array(
    'title'         => t('Title'),
    'sent'          => t('Sent'),
    'acknowledged'  => t('Acknowledged'),
    'delivered_at'  => t('Last delivered'),
    'expired'       => t('Expired'),
  );
  
  $cached_data = cache_get('pushover_receipt_cache', 'cache');

  if ($cached_data && time() < $cached_data->expire) {
    $options = $cached_data->data;
  }
  else {
    $options = array();

    if (!empty($receipts) && $token !== '') {
      foreach ($receipts as $receipt) {
        if ($receipt['added'] + (60*60*24*14) < time()) {
          unset($receipts[$receipt['id']]);
          continue;
        }
        $url = 'https://api.pushover.net/1/receipts/' . $receipt['id'] . '.json?token=' . $token;
        $data_options = array(
          'method' => 'GET'
        );
    
        $response = drupal_http_request($url, $data_options);
        $res_data = json_decode($response->data);
        $options[$receipt['id']] = array(
          'title'         => $receipt['title'],
          'sent'          => date('o.m.d H:i:s', $receipt['added']),
          'acknowledged'  => ($res_data->acknowledged == 1 ? t('Acknowledged at %time', array('%time' => date('o.m.d H:i:s', $res_data->acknowledged_at))) : 0),
          'delivered_at'  => date('o.m.d H:i:s', $res_data->last_delivered_at),
          'expired'       => ($res_data->expired == 1 ? t('Expired') : t('Expires at %time', array('%time' => date('o.m.d H:i:s', $res_data->expires_at)))),
        );
        $receipts[$receipt['id']]['acknowledged'] = $res_data->acknowledged;
      }
      variable_set('pushover_receipts', $receipts);
      cache_set('pushover_receipt_cache', $options, 'cache', time() + 300);
    }
  }
  $form = array();

  $form['explanation'] = array(
    '#type' => 'item',
    '#description' => t('Messages will be here up to 14 days.'),
  );

  $form['receips'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No content available.'),
  );

  $form['cache_explanation'] = array(
    '#type' => 'item',
    '#description' => t('The results will be cached for 5 minutes to prevent API overload.'),
  );

  return $form;
}