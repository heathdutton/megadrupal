<?php

/**
 * @file
 * Code required for KeyCAPTCHA
 */

/**
 * Let the user know that the KeyCAPTCHA Private key and JS code not set
 */
function _keycaptcha_check_vars() {
  global $_keycaptcha_calledalready;
  if (!$_keycaptcha_calledalready) {
    $_keycaptcha_calledalready = TRUE;
    $keycaptcha_js_code = variable_get('keycaptcha_js_code', '');
    $keycaptcha_private_key = variable_get('keycaptcha_private_key', '');
    if ( ( $keycaptcha_js_code = '' ) || ( $keycaptcha_private_key == '' ) ) {
      drupal_set_message(t('You do not set the parameters ("Private key" and "KeyCAPTCHA script") for the correct working of KeyCAPTCHA. Please go to <a href="https://www.keycaptcha.com" target="_blank">KeyCAPTCHA site</a> and read installation instruction for Drupal.'), 'error');
    }
  }
}

