<?php
/**
 * @file
 * API and hooks documentation for the Commerce Payment Network module.
 */

/**
 * Alter payment data before it is sent to Payment Network.
 *
 * Allows modules to alter the payment data before the data is signed and sent
 * to Payment Network AG.
 *
 * @param $data
 *   An associative array containing data that is to be sent to
 *   Payment Network as an associative array.
 * @param $order
 *   An object containing commerce order being processed.
 * @param $settings
 *   An associative array containing configuration settings.
 */
function hook_commerce_payment_network_data(&$data, $order, $settings) {
  global $language;

  // Set the dynamic template to be used by Payment Network.
  $data['TP'] = url('checkout/payment_network', array('absolute' => TRUE));

  // For multilingual sites, attempt to use the site's active language rather
  // than the language configured through the payment method settings form.
  $language_mapping = array(
    'nl' => 'nl_BE',
    'fr' => 'fr_FR',
    'en' => 'en_US'
  );

  $data['LANGUAGE'] = isset $language_mapping[$language->language] ? $language_mapping[$language->language] : $settings['language'];
}
