<?php
/**
 * @file
 * API and hooks documentation for the Commerce OTP payment module.
 */

/**
 * Alter payment data before it is sent to OTP.
 *
 * @param array $data
 *   The data that is to be set $_REQUEST data.
 * @param object $order
 *   The commerce order object being processed.
 * @param array $settings
 *   The configuration settings.
 */
function hook_commerce_otp_data_alter(array &$data, $order, array $settings) {
  global $language;

  // For multilingual sites, attempt to use the site's active language rather
  // than the language configured through the payment method settings form.
  $language_options = array(
    'hu' => t('Hungarian'),
    'en' => t('English'),
    'de' => t('German'),
  );
  $data['languagecode'] = isset($language_options[$language->language]) ? $language_options[$language->language] : $settings['preset']['languagecode'];
}
