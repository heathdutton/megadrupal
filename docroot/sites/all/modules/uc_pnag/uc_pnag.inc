<?php

/**
 * @file
 * Parameter file for the uc_pnag module.
 *
 * In this file all system parameters from SOFORT are collected and
 * get returned to the module according to the current context.
 */

/**
 * Defines the default country just in case that the system can't determine a
 * proper language.
 */
define('UC_PNAG_DEFAULT_COUNTRY', 'de');

/**
 * Defines the default currency just in case that no other currency can be
 * determined.
 */
define('UC_PNAG_DEFAULT_CURRENCY', 'EUR');

// =============================== Getter functions ============================

/**
 * Returns the name of the payment method according to the current UI language.
 *
 * @return string
 */
function uc_pnag_name() {
  return sofortbanking_strings(1001);
}

/**
 * Returns the name of the payment method according to the current UI language
 * and depending on settings for customer protection and recommendation.
 *
 * @return string
 */
function uc_pnag_name_checkout() {
  $recommended = uc_pnag_recommended_string();
  $protection = variable_get('uc_pnag_customer_protection', FALSE) ? ' ' . sofortbanking_strings(1002) : '';
  return sofortbanking_strings(1001) . $protection . $recommended;
}

/**
 * Returns the fully rendered HTML for the logo of the payment method which
 * is dependant on the current UI language.
 *
 * @return string
 */
function uc_pnag_logo() {
  return _uc_pnag_image('100x38');
}

/**
 * Returns the fully rendered HTML for the banner of the payment method which
 * is dependant on the current UI language and on the settings on customer
 * protection.
 *
 * @return string
 */
function uc_pnag_banner() {
  return variable_get('uc_pnag_customer_protection', FALSE) ? _uc_pnag_image('banner_400x100_ks') : _uc_pnag_image('banner_300x100');
}

/**
 * Returns the fully rendered HTML for the logo and the description of the
 * payment method which is dependant on the current UI language and on the
 * settings on customer protection and recommendation.
 *
 * @return string
 */
function uc_pnag_description() {
  $output = uc_pnag_name_checkout();
  $output .= '<br /><a href="' . _uc_pnag_customer_info_link(FALSE) . '" target="_blank">' . uc_pnag_logo() . '</a> ';
  $output .= '<ul>';
  if (variable_get('uc_pnag_customer_protection', FALSE)) {
    $output .= sofortbanking_strings(1005, array('!url' => _uc_pnag_customer_info_link()));
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
 * @see also uc_pnag_description()
 *
 * @return string
 */
function uc_pnag_checkout_display() {
  if (variable_get('uc_pnag_checkout_display_mode', 'banner') == 'description') {
    return uc_pnag_description();
  }
  $output = uc_pnag_name_checkout();
  $output .= '<br /><a href="'. _uc_pnag_customer_info_link() . '" target="_blank">' . uc_pnag_banner() . '</a>';
  return $output;
}

/**
 * Returns the language dependant string, if the payment method is recommended,
 * which depends on the settings in the shop configuration.
 *
 * @return string
 */
function uc_pnag_recommended_string() {
  return variable_get('uc_pnag_recommended', TRUE) ? sofortbanking_strings(1003) : '';
}

/**
 * Determines and returns the fully qualified URL for callbacks on success,
 * cancel or timeout of transactions. Those callbacks are protected by a
 * token which is included as a query in the URL.
 *
 * @param string $mode
 *   The values 'success', 'cancel', 'timeout' and 'notify' are supported.
 * @return string
 *   The fully qualified URL for the callback.
 */
function uc_pnag_callback_url($mode) {
  return url('uc_pnag/payment/' . $mode, array('absolute' => TRUE, 'query' => array('cart_token' => drupal_get_token(),)));
}

// =============================== Helper functions ============================

/**
 * Returns an array of all supported languages of this payment method.
 *
 * @return array
 */
function _uc_pnag_languages() {
  return array('de', 'en', 'nl', 'fr', 'it', 'es', 'pl',);
}

/**
 * Returns an array of all supported currencies of this payment method.
 *
 * @return array
 */
function _uc_pnag_currencies() {
  return array('EUR', 'GBP', 'PLN', 'CHF',);
}

/**
 * Returns the language code of the current UI language if supported by the
 * payment method or the default language code ('de') otherwise.
 *
 * @return string
 */
function _uc_pnag_language() {
  static $_uc_pnag_lng;
  if (!isset($_uc_pnag_lng)) {
    global $language;
    $_uc_pnag_lng = in_array($language->language, _uc_pnag_languages()) ? $language->language : UC_PNAG_DEFAULT_COUNTRY;
  }
  return $_uc_pnag_lng;
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
function _uc_pnag_image($filename) {
  $lng   = _uc_pnag_language();
  $logo  = url(drupal_get_path('module', 'uc_pnag') . '/ressources/' . $lng . '/' . $filename . '.png', array('absolute' => TRUE));
  $alt   = uc_pnag_name();
  $title = uc_pnag_name();
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
function _uc_pnag_customer_info_link($check_for_customer_protection = TRUE) {
  $lng   = _uc_pnag_language();
  if ($check_for_customer_protection && variable_get('uc_pnag_customer_protection', FALSE)) {
    $lngcode2 = ($lng == 'de') ? 'ger' : 'eng';
    return 'https://www.sofort-bank.com/' . $lngcode2 . '-DE/general/kaeuferschutz/informationen-fuer-kaeufer';
  }
  return 'https://images.sofort.com/' . $lng . '/su/landing.php';
}
