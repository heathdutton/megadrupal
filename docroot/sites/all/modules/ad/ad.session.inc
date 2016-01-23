<?php
/**
 * @file
 * Code for the ad cookie tracking.
 */

function ad_session_start() {
  if (variable_get('ad_session_enable', FALSE)) {
    $cookie_name = ad_session_get_cookie_name();
    if (!$_COOKIE[$cookie_name]) {
      $value = ad_uniqid('session');
      ad_session_get($value);
      $expire = variable_get('ad_session_expire', REQUEST_TIME + 60 * 60 * 24 * 30 * 12);
      setcookie($cookie_name, $value, $expire);
    }
  }
}

function ad_session_get($set_value = NULL) {
  if (variable_get('ad_session_enable', FALSE)) {
    $cookie_name = ad_session_get_cookie_name();
    if (!is_null($set_value)) {
      $_COOKIE[$cookie_name] = $set_value;
    }
    return isset($_COOKIE[$cookie_name]) ? $_COOKIE[$cookie_name] : NULL;
  }
  else {
    return NULL;
  }
}

function ad_session_get_cookie_name() {
  return variable_get('ad_session_cookie_name', 'ad');
}
