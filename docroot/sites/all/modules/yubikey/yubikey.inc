<?php
/*
 * @file
 * Utility functions for the yubikey module.
 */

/*
 * Check if url is valid validation server url.
 */
function yubikey_clean_url($url) {
  return preg_replace("/^http[s]{0,1}:\/\//i", "", $url) ;
}

/*
 * Check if url is valid validation server url.
 */
function yubikey_valid_url($url) {
  $absolute = ((preg_match("/^http[s]{0,1}:\/\//i", $url) == 1) ? TRUE: FALSE);
  return valid_url($url, $absolute);
}

/* 
 * Get YubiKey identity from OTP.
 */
function yubikey_get_identity($otp) {
  return drupal_substr($otp, 0, drupal_strlen($otp) - 32);
}

/*
 * Check if OTP is valid YubiKey OTP.
 */
function yubikey_is_otp($otp) {
  if (!preg_match("/^[cbdefghijklnrtuvCBDEFGHIJKLNRTUV]{44}$/", $otp)) {
    return FALSE;
  }
  return TRUE;
}

/*
 * Valdiate YubiKey OTP.
 */
function yubikey_validate_otp($otp, &$errstr) {
  if (!variable_get('yubikey_enabled', FALSE)) {
    $errstr = t("YubiKey authentication is not enabled.");
    return FALSE;
  }

  if (!function_exists('curl_init')) {
    $errstr = t("A required PHP library is missing.");
    return FALSE;
  }
  
  module_load_include('php', 'yubikey', 'Yubico');

  $api_id = variable_get('yubikey_valserver_api_id', NULL);
  $api_key = variable_get('yubikey_valserver_api_key', NULL);
  $https = variable_get('yubikey_valserver_api_https', FALSE);
  $timeout = variable_get('yubikey_valserver_api_timeout', NULL);

  $yubi = new Auth_Yubico($api_id, $api_key, $https);
  if (variable_get('yubikey_valserver_urls_type', 'online') == 'custom') {
    $url = variable_get('yubikey_valserver_urls', NULL);
    $urls = explode(",", $url);
    foreach ($urls as $u) {
      if ($u != '') {
        $yubi->addURLpart($u);
      }
    }
   }

   $auth = $yubi->verify($otp, FALSE, FALSE, NULL, $timeout);
   // error_log('Yubikey OTP validation: Query details ::> ' . str_replace (" ", "\n", $yubi->getLastQuery() . " "));
   if (PEAR::isError($auth)) {
     // error_log('YubiKey OTP validation: Failed with message :: ' . $auth->getMessage());
     // error_log('YubiKey OTP validation: Debug output :: ' . $yubi->getLastResponse());

     $errstr = $auth->getMessage();
     return FALSE;
   }
   return TRUE;
}
