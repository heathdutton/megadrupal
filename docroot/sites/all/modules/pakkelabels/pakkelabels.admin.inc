<?php

/**
 * @file
 * pakkelabels module admin settings.
 */

use Pakkelabels\Pakkelabels;

/**
 * Return the pakkelabels global settings form.
 */
function pakkelabels_admin_settings() {
  $form['pakkelabels_api_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Pakkelabels.dk User'),
    '#required' => TRUE,
    '#default_value' => variable_get('pakkelabels_api_user', ''),
    '#description' => t('The user for your pakkelabels account. Get your user at pakkelabels.dk. Please read the !termslink.', array('!termslink' => l(t('pakkelabels API Terms'), 'https://www.pakkelabels.dk/integration/api/'))),
  );

  $form['pakkelabels_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Pakkelabels.dk API Key'),
    '#required' => TRUE,
    '#default_value' => variable_get('pakkelabels_api_key', ''),
    '#description' => t('The API key for your pakkelabels account. Get or generate a valid API key at pakkelabels. Please read the !termslink.', array('!termslink' => l(t('pakkelabels API Terms'), 'https://www.pakkelabels.dk/integration/api/'))),
  );

  $options = array('direct' => 'Directly', 'imported' => 'As template');
  $form['pakkelabels_create_mode'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('pakkelabels_create_mode', 'imported'),
    '#title' => t('Create label...'),
   );

  $options = array('true' => 'true', 'false' => 'false');
  $form['pakkelabels_testmode'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('pakkelabels_testmode', 'true'),
    '#title' => t('Testmode'),
   );

  $form['pakkelabels_sender_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender name'),
    '#required' => TRUE,
    '#default_value' => variable_get('pakkelabels_sender_name', ''),
    '#description' => t('Sender name which goes on the shipment label'),
  );

  $form['pakkelabels_sender_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender Address'),
    '#required' => TRUE,
    '#default_value' => variable_get('pakkelabels_sender_address', ''),
    '#description' => t('Sender address which goes on the shipment label'),
  );

  $form['pakkelabels_sender_zipcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Zip code'),
    '#required' => TRUE,
    '#default_value' => variable_get('pakkelabels_sender_zipcode', ''),
    '#description' => t('Sender zipcode which goes on the shipment label'),
  );

  $form['pakkelabels_sender_city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#required' => TRUE,
    '#default_value' => variable_get('pakkelabels_sender_city', ''),
    '#description' => t('Sender city which goes on the shipment label'),
  );

  $form['pakkelabels_sender_countrycode'] = array(
    '#type' => 'textfield',
    '#title' => t('Country code'),
    '#required' => TRUE,
    '#default_value' => variable_get('pakkelabels_sender_countrycode', ''),
    '#description' => t('Sender country code which goes on the shipment label'),
  );

  $form['pakkelabels_sender_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender email'),
    '#required' => TRUE,
    '#default_value' => variable_get('pakkelabels_sender_email', ''),
    '#description' => t('Sender email'),
  );

  // Initiate API Client.
  try {
    $api_request = pakkelabels_get_api_client();
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
    return system_settings_form($form);
  }

  return system_settings_form($form);
}
