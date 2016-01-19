<?php

/**
 * @file
 * Parameter file for the commerce_sofortbanking module.
 *
 * In this file all system parameters from SOFORT are collected and
 * get returned to the module according to the current context.
 */

/**
 * Defines the default country just in case that the system can't determine a
 * proper language.
 */
define('COMMERCE_SOFORTBANKING_DEFAULT_COUNTRY', 'de');

/**
 * Defines the default currency just in case that no other currency can be
 * determined.
 */
define('COMMERCE_SOFORTBANKING_DEFAULT_CURRENCY', 'EUR');

// =============================== Getter functions ============================

/**
 * Returns the name of the payment method according to the current UI language.
 *
 * @return string
 */
function commerce_sofortbanking_name() {
  return sofortbanking_strings(1001);
}

/**
 * Returns the name of the payment method according to the current UI language
 * and depending on settings for customer protection and recommendation.
 *
 * @return string
 */
function commerce_sofortbanking_name_checkout($settings) {
  $recommended = commerce_sofortbanking_recommended_string($settings);
  $protection = $settings['extended']['options']['customer_protection'] ? ' ' . sofortbanking_strings(1002) : '';
  return sofortbanking_strings(1001) . $protection . $recommended;
}

/**
 * Returns the fully rendered HTML for the logo of the payment method which
 * is dependant on the current UI language.
 *
 * @return string
 */
function commerce_sofortbanking_logo() {
  return _commerce_sofortbanking_image('100x38');
}

/**
 * Returns the fully rendered HTML for the banner of the payment method which
 * is dependant on the current UI language and on the settings on customer
 * protection.
 *
 * @param array $settings
 *   Includes all the settings about the payment method.
 * @return string
 */
function commerce_sofortbanking_banner($settings) {
  return $settings['extended']['options']['customer_protection'] ? _commerce_sofortbanking_image('banner_400x100_ks') : _commerce_sofortbanking_image('banner_300x100');
}

/**
 * Returns the fully rendered HTML for the logo and the description of the
 * payment method which is dependant on the current UI language and on the
 * settings on customer protection and recommendation.
 *
 * @param array $settings
 *   Includes all the settings about the payment method.
 * @return string
 */
function commerce_sofortbanking_description($settings) {
  $output = commerce_sofortbanking_name_checkout($settings);
  $output .= '<br /><a href="' . _commerce_sofortbanking_customer_info_link($settings, FALSE) . '" target="_blank">' . commerce_sofortbanking_logo() . '</a> ';
  $output .= '<ul>';
  if ($settings['extended']['options']['customer_protection']) {
    $output .= sofortbanking_strings(1005, array('!url' => _commerce_sofortbanking_customer_info_link($settings)));
  }
  $output .= sofortbanking_strings(1004);
  $output .= '</ul>';
  return $output;
}

/**
 * Returns the fully rendered HTML for the display of the payment method in
 * the checkout pane. This will either return the description or just the
 * label and the banner.
 *
 * @see also commerce_sofortbanking_description()
 *
 * @param array $settings
 *   Includes all the settings about the payment method.
 * @return string
 */
function commerce_sofortbanking_checkout_display($settings) {
  if ($settings['extended']['checkout_display_mode'] == 'description') {
    return commerce_sofortbanking_description($settings);
  }
  $output = commerce_sofortbanking_name_checkout($settings);
  $output .= '<br /><a href="'. _commerce_sofortbanking_customer_info_link($settings) . '" target="_blank">' . commerce_sofortbanking_banner($settings) . '</a>';
  return $output;
}

/**
 * Returns the language dependant string, if the payment method is recommended,
 * which depends on the settings in the shop configuration.
 *
 * @param array $settings
 *   Includes all the settings about the payment method.
 * @return string
 */
function commerce_sofortbanking_recommended_string($settings) {
  return $settings['extended']['options']['recommended'] ? sofortbanking_strings(1003) : '';
}

/**
 * Determines and returns the fully qualified URL for callbacks on success,
 * cancel or timeout of transactions. Those callbacks are protected by a
 * token which is included as a query in the URL.
 *
 * @param string $mode
 *   The values 'success', 'cancel', 'timeout' and 'notify' are supported.
 * @param object $order
 *   Includes the complete order object from commerce_order.
 * @return string
 *   The fully qualified URL for the callback.
 */
function commerce_sofortbanking_callback_url($mode, $order, $absolute = TRUE) {
  switch ($mode) {
    case 'success':
      return url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => $absolute));

    case 'timeout':
    case 'cancel':
      return url('checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key'], array('absolute' => $absolute));

    case 'notify':
      return url('commerce_sofortbanking/payment/notify', array('absolute' => $absolute, 'query' => array('cart_token' => drupal_get_token(),)));

  }
  return FALSE;
}

// =============================== Helper functions ============================

/**
 * Returns an array of all supported languages of this payment method.
 *
 * @return array
 */
function _commerce_sofortbanking_languages() {
  return array('de', 'en', 'nl', 'fr', 'it', 'es', 'pl',);
}

/**
 * Returns an array of all supported currencies of this payment method.
 *
 * @return array
 */
function _commerce_sofortbanking_currencies() {
  return array('EUR', 'GBP', 'PLN', 'CHF',);
}

/**
 * Returns the language code of the current UI language if supported by the
 * payment method or the default language code ('de') otherwise.
 *
 * @return string
 */
function _commerce_sofortbanking_language() {
  static $_commerce_sofortbanking_lng;
  if (!isset($_commerce_sofortbanking_lng)) {
    global $language;
    $_commerce_sofortbanking_lng = in_array($language->language, _commerce_sofortbanking_languages()) ? $language->language : COMMERCE_SOFORTBANKING_DEFAULT_COUNTRY;
  }
  return $_commerce_sofortbanking_lng;
}

/**
 * Returns the fully rendered HTML for an image in the subdirectory ressources
 * with the given filename and the extension png.
 *
 * The image being used is dependant on the current UI language.
 *
 * @param $filename
 * @return string
 */
function _commerce_sofortbanking_image($filename) {
  $lng   = _commerce_sofortbanking_language();
  $logo  = url(drupal_get_path('module', 'commerce_sofortbanking') . '/ressources/' . $lng . '/' . $filename . '.png', array('absolute' => TRUE));
  $alt   = commerce_sofortbanking_name();
  $title = commerce_sofortbanking_name();
  $args = array(
    'path' => $logo,
    'alt' => $alt,
    'title' => $title,
    'attributes' => array(),
  );
  return theme_image($args);
}

/**
 * Returns the URL from SOFORT AG which displays more customer information
 * about the payment method. The result depends on the current UI language.
 *
 * @return string
 */
function _commerce_sofortbanking_customer_info_link($settings, $check_for_customer_protection = TRUE) {
  $lng   = _commerce_sofortbanking_language();
  if ($check_for_customer_protection && $settings['extended']['options']['customer_protection']) {
    $lngcode2 = ($lng == 'de') ? 'ger' : 'eng';
    return 'https://www.sofort-bank.com/' . $lngcode2 . '-DE/general/kaeuferschutz/informationen-fuer-kaeufer';
  }
  return 'https://images.sofort.com/' . $lng . '/su/landing.php';
}
