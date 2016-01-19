<?php

/**
 * Configuration options for BitPay.
 */
function bitpay_settings() {
  $form['bitpay_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('BitPay API key'),
    '#description' => t('The API key you created at <a href="https://bitpay.com/api-keys">bitpay.com</a>.'),
    '#default_value' => variable_get('bitpay_api_key', ''),
    '#required' => TRUE,
  );
  $form['bitpay_debug'] = array(
    '#type' => 'radios',
    '#title' => t('Debug logging'),
    '#description' => t('Enable to log BitPay API calls and invoice status updates.'),
    '#options' => array(t('Disabled'), t('Enabled')),
    '#default_value' => variable_get('bitpay_debug', FALSE),
    '#required' => TRUE,
  );
  $form['bitpay_notification_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification e-mail'),
    '#description' => t('E-mail address where invoice update notifications should be sent.'),
    '#default_value' => variable_get('bitpay_notification_email', variable_get('site_mail', ini_get('sendmail_from'))),
    '#required' => TRUE,
  );
  $form['bitpay_redirect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect path'),
    '#description' => t('The Drupal path where the customer should be redirected to after paying for the order; e.g. <em>donate/thanks</em>.'),
    '#default_value' => variable_get('bitpay_redirect_url', ''),
    '#required' => TRUE,
  );
  $form['bitpay_currency'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency'),
    '#description' => t('The three-letter currency code - e.g. <em>USD</em> - in which the invoice was priced.'),
    '#default_value' => variable_get('bitpay_currency', 'BTC'),
    '#required' => TRUE,
  );
  $form['bitpay_physical'] = array(
    '#type' => 'radios',
    '#title' => t('Physical'),
    '#description' => t('Indicates whether anything is to be shipped with the order (if false, the buyer will be informed that nothing is to be shipped). This setting may be overridden.'),
    '#options' => array(t('False'), t('True')),
    '#default_value' => variable_get('bitpay_physical', TRUE),
    '#required' => TRUE,
  );
  $form['bitpay_transaction_speed'] = array(
    '#type' => 'select',
    '#title' => t('Transaction speed'),
    '#description' => t('See <a href="https://bitpay.com/downloads/bitpayApi.pdf">API docs</a> for more details.'),
    '#options' => array('low' => t('Low'), 'medium' => t('Medium'), 'high' => t('High')),
    '#default_value' => variable_get('bitpay_transaction_speed', 'low'),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}
