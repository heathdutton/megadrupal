<?php

/**
 * @file
 * Administration functions for locale_cookie.module.
 */

/**
 * Identify language from a request/cookie parameter.
 *
 * @param $languages
 *   An array of valid language objects.
 *
 * @return
 *   A valid language code on success, FALSE otherwise.
 */
function locale_cookie_language_from_cookie($languages) {
  global $base_path;
  $param = variable_get('locale_cookie_language_negotiation_cookie_param', 'language');

  if (isset($_COOKIE[$param]) && isset($languages[$langcode = $_COOKIE[$param]])) {
    $langcode = $_COOKIE[$param];
  }
  else {
    global $language;
    if (is_object($language)) {
      $langcode = $language->language;
    }
    else {
      $langcode = language_default('language');
    }
  }
  $expire = REQUEST_TIME + (86400 * variable_get('locale_cookie_language_negotiation_cookie_expire', 7));
  setcookie($param, $langcode, $expire, $base_path);
  return $langcode;
}