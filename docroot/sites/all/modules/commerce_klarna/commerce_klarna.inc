<?php

/**
 * @file
 * Includes functions related to the communication with
 * Klarna.
 */

$path = DRUPAL_ROOT . libraries_get_path('klarna', TRUE);
require_once $path . '/Klarna.php';
require_once $path . '/transport/xmlrpc-3.0.0.beta/lib/xmlrpc.inc';
require_once $path . '/transport/xmlrpc-3.0.0.beta/lib/xmlrpc_wrappers.inc';

/**
 * Helper function that initiates the Klarna object.
 *
 * @param $settings
 *  A settings array for our payment method.
 *  - estore_id: The id for our store.
 *  - secret: The shared secret for our store.
 *  - live_mode: Boolean indicating if the store is in live mode or not.
 *
 * @return
 *  A Klarna object.
 */
function commerce_klarna_init($settings) {
  // Normalize settings.
  $settings['estore_id'] = isset($settings['estore_id']) ? $settings['estore_id'] : '';
  $settings['secret'] = isset($settings['secret']) ? $settings['secret'] : '';
  $settings['live_mode'] = isset($settings['live_mode']) ? $settings['live_mode'] : 0;

  // Load Klarna.
  $klarna = new Klarna();  
  
  // Configure.
  $klarna->config(
    intval($eid = $settings['estore_id']),
    mb_convert_encoding($settings['secret'], 'ISO-8859-1'),
    KlarnaCountry::SE,
    KlarnaLanguage::SV,
    KlarnaCurrency::SEK,
    $settings['live_mode'] ? Klarna::LIVE : Klarna::BETA
  );
  
  return $klarna;
}
