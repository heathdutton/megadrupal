<?php

/**
 * @file
 * Paymill settings forms and administration page callbacks.
 */

/**
 * Returns the site-wide Paymill settings form.
 */
function commerce_sp_paymill_settings_form($form, &$form_state) {
  // Add form elements to collect default account information.
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default account settings'),
    '#description' => t('Configure this information based on the "API Keys" section within the Paymill account settings. Unfortunately the settings have to be duplicated to the ones from Commerce Paymill.'),
    '#collapsible' => TRUE,
  );

  $form['account']['commerce_sp_paymill_test_private_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Private test key'),
    '#size' => 40,
    '#maxlength' => 32,
    '#default_value' => variable_get('commerce_sp_paymill_test_private_key', ''),
    '#required' => TRUE,
  );

  $form['account']['commerce_sp_paymill_test_public_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Public test key'),
    '#size' => 40,
    '#maxlength' => 32,
    '#default_value' => variable_get('commerce_sp_paymill_test_public_key', ''),
    '#required' => TRUE,
  );

  $form['account']['commerce_sp_paymill_live_private_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Private live key'),
    '#size' => 40,
    '#maxlength' => 32,
    '#default_value' => variable_get('commerce_sp_paymill_live_private_key', ''),
    '#required' => TRUE,
  );

  $form['account']['commerce_sp_paymill_live_public_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Public live key'),
    '#size' => 40,
    '#maxlength' => 32,
    '#default_value' => variable_get('commerce_sp_paymill_live_public_key', ''),
    '#required' => TRUE,
  );

  $form['account']['commerce_sp_paymill_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Transaction mode'),
    '#options' => array(
      'test' => t('Test'),
      'live' => t('Live'),
    ),
    '#default_value' => variable_get('commerce_sp_paymill_mode', 'test'),
    '#required' => TRUE,
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'commerce_sp_paymill_settings_form_submit';
  return $form;
}

function commerce_sp_paymill_settings_form_submit($form, &$form_state) {
  $webhookkey =  variable_get('commerce_sp_paymill_webhook_key', '');
  if (empty($webhookkey)) {
    // Initialize the paymill webhook key variable.
    // We use a "private" URL for the Paymill hook so that it cannot be easily
    // guessed from outside. Furthermore the webhook validation is based on
    // the included subscription object's id.
    // @see commerce_sp_paymill_webhook_process()
    $webhookkey = substr(drupal_hash_base64(REQUEST_TIME), 0, 10);
    variable_set('commerce_sp_paymill_webhook_key', $webhookkey);
    menu_rebuild();
  }

  $modes = array('test', 'live');
  foreach ($modes as $mode) {
    if ($form_state['values']['commerce_sp_paymill_mode'] ==  $mode) {
      $webhook_id = variable_get('commerce_sp_paymill_' . $mode . '_webhooks_id', '');
      if (empty($webhook_id)) {
        $url = $GLOBALS['base_url'] . '/commerce_sp_paymill/webhook/' . $webhookkey;
        $event_types = array_keys(commerce_sp_paymill_webhook_event_types());
        $result = commerce_sp_paymill_api_webhooks_create($url, $event_types, $mode);
        if (isset($result['id'])) {
          variable_set('commerce_sp_paymill_' . $mode . '_webhooks_id', $result['id']);
          drupal_set_message(t('Registered Paymill @mode webhook at %url', array('@mode' => $mode, '%url' => $url)));
        }
        else {
          drupal_set_message(t('Could not register Paymill @mode webhook', array('@mode' => $mode)), 'error');
        }
      }
    }
  }
}
