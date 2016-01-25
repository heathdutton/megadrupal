<?php
/**
 * @file
 * API and hooks documentation for the Commerce Europabank module.
 */

/**
 * Alter payment data before it is sent to Europabank.
 *
 * Allows modules to alter the payment data before the data is signed and sent
 * to Europabank.
 *
 * @param array $data
 *   The data that is to be sent to Europabank as an associative array.
 * @param object $order
 *   The commerce order object being processed.
 * @param array $settings
 *   The configuration settings.
 */
function hook_commerce_europabank_data(&$data, $order, $settings) {
  global $language;

  // Set the dynamic template to be used by Europabank.
  $data['TP'] = url('checkout/europabank', array('absolute' => TRUE));

  // For multilingual sites, attempt to use the site's active language rather
  // than the language configured through the payment method settings form.
  $language_mapping = array(
    'nl' => 'nl_BE',
    'fr' => 'fr_FR',
    'en' => 'en_US',
  );
  $data['LANGUAGE'] = isset($language_mapping[$language->language]) ? $language_mapping[$language->language] : $settings['language'];
}
