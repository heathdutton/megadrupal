<?php
/**
 * @file admin.inc
 *
 * Contains configuration settings for EPS payment.
 */

/**
 * Returns the URL to the specified EPS SO (scheme operator).
 *
 * @param $server
 *   Either sandbox or live indicating which server to get the URL for.
 *
 * @return
 *   The URL to use to submit requests to the EPS SO (scheme operator).
 */
function commerce_eps_server_url($server) {
  switch ($server) {
    case 'sandbox':
      return 'https://routing.eps.or.at/appl/epsSO-test/transinit/eps/v2_5';
    case 'live':
      return 'https://routing.eps.or.at/appl/epsSO/transinit/eps/v2_5';
  }
}

/**
 * Payment method callback: settings form.
 */
function commerce_eps_settings_form($settings = array()) {
  $form = array();

  $form['eps_mode'] = array(
    '#type' => 'select',
    '#title' => t('EPS Mode'),
    '#description' => t('Enable GIROPAY compatibility if desired.'),
    '#options' => array('eps' => t('EPS only'), 'eps_gp' => t('EPS with GIROPAY compatibility')),
    '#default_value' => $settings['eps_mode'] ? $settings['eps_mode'] : 'eps',
  );

  $form['so_server'] = array(
    '#type' => 'select',
    '#title' => t('Scheme Operator Server'),
    '#description' => t('Either use the test system or process real transactions'),
    '#options' => array('sandbox' => t('Test System'), 'live' => t('Live System')),
    '#default_value' => $settings['so_server'] ? $settings['so_server'] : 'sandbox',
    '#required' => TRUE,
  );

  $form['user_id'] = array(
    '#type' => 'textfield',
    '#title' => t('User Id'),
    '#description' => t('EPS Login'),
    '#default_value' => $settings['user_id'] ? $settings['user_id'] : 'GIBAATWWXXX_00000',
    '#required' => TRUE,
  );

  $form['pin'] = array(
    '#type' => 'textfield',
    '#title' => t('PIN'),
    '#description' => t('EPS Authentification password'),
    '#default_value' => $settings['pin'] ? $settings['pin'] : '0000000000000000',
    '#required' => TRUE,
  );

  $form['ben_account_id'] = array(
    '#type' => 'textfield',
    '#title' => t('IBAN'),
    '#description' => t('Account identifier of beneficiary with IBAN (International Bank Account Number), e. g. AT611904300234573201.'),
    '#default_value' => $settings['ben_account_id'] ? $settings['ben_account_id'] : 'AT212099900000637777',
    '#maxlength' => 34,
    '#required' => TRUE,
  );

  $form['bfi_bic_id'] = array(
    '#type' => 'textfield',
    '#title' => t('BIC'),
    '#description' => t('ISO 9362 business identifier code (BIC) bank code.'),
    '#default_value' => $settings['bfi_bic_id'] ? $settings['bfi_bic_id'] : 'GIBAATWXXXX',
    '#maxlength' => 11,
    '#required' => TRUE,
  );

  $form['ben_name_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Beneficiary name and address'),
    '#description' => t('Identification (name and address) in an unstructured form. The beneficiary and account holder may vary.'),
    '#default_value' => $settings['ben_name_address'] ? $settings['ben_name_address'] : 'Max Mustermann; 1230 Wien',
    '#maxlength' => 70,
  );
  return $form;
}