<?php

/**
 * Get PayWay Net token.
 *
 * Based on http://github.com/GiantRobot/uc_payway_net/
 */
function _commerce_payway_net_get_token($parameters) {
  $cert_file = $parameters['cert_path'];
  $payway_base_url = $parameters['payway_url'];
  // Find the port setting, if any.
  // @TODO: parse_url().
  $payway_url = $payway_base_url;
  $port = 443;
  $port_pos = strpos($payway_base_url, ":", 6);
  $url_end_pos = strpos($payway_base_url, "/", 8);
  if ($port_pos !== FALSE && $port_pos < $url_end_pos) {
    $port = (int)substr($payway_base_url, ((int) $port_pos) + 1, ((int) $url_end_pos));
    $payway_url = substr($payway_base_url, 0, ((int) $port_pos)) . substr($payway_base_url, ((int) $url_end_pos), strlen($payway_base_url));
  }

  $ch = curl_init($payway_url . "RequestToken");
  if ($port != 443) {
    curl_setopt($ch, CURLOPT_PORT, $port);
  }
  curl_setopt($ch, CURLOPT_FAILONERROR, TRUE);
  curl_setopt($ch, CURLOPT_FORBID_REUSE, TRUE);
  curl_setopt($ch, CURLOPT_FRESH_CONNECT, TRUE);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  // Set timeout options.
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 30);
  curl_setopt($ch, CURLOPT_TIMEOUT, 30);
  // Set references to certificate files.
  curl_setopt($ch, CURLOPT_CAINFO, $cert_file);
  // Check the existence of a common name in the SSL peer's certificate
  // and also verify that it matches the hostname provided.
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 1);
  // Verify the certificate of the SSL peer.
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, TRUE);
  // Build the parameters string to pass to PayWay.
  $parameters_string = '';
  $init = TRUE;
  foreach ($parameters as $name => $value) {
    if ($init) {
      $init = FALSE;
    }
    else {
      $parameters_string = $parameters_string . '&';
    }
    $parameters_string = $parameters_string . urlencode($name) . '=' . urlencode($value);
  }
  curl_setopt($ch, CURLOPT_POSTFIELDS, $parameters_string);
  // Make the request.
  $text = curl_exec($ch);
  // Check the response for errors.
  $errnum = curl_errno($ch);
  if ($errnum != 0) {
    watchdog('commerce_payway_net', 'cURL error getting token, Error %errnum, %errmsg', array('%errnum' => $errnum, '%errmsg' => curl_error($ch)), WATCHDOG_ERROR);
    header("HTTP/1.1 403 " . $errmsg);
    exit;
  }
  curl_close($ch);
  // Split the response into parameters.
  $rsp_parameter_array = explode("&", $text);
  $rsp_parameters = array();
  foreach ($rsp_parameter_array as $parameter) {
    list($name, $value) = explode("=", $parameter, 2);
    $rsp_parameters[$name] = $value;
  }
  if (array_key_exists('error', $rsp_parameters)) {
    watchdog('commerce_payway_net', 'Error getting token: %err', array('%err' => $rsp_parameters['error']), WATCHDOG_ERROR);
    throw new Exception(t('cURL error: %err', array('%err' => $rsp_parameters['error'])));
  }
  else {
    return $rsp_parameters['token'];
  }
}

/**
 * Decrypt data returned in $_GET params when browser is redirected from PayWay Net. This lets us
 * verify that the user viewing the return page is the one who made the transaction. Transaction
 * completion is handled via the Notify callback, however.
 */
function _commerce_payway_net_decrypt_parameters($encrytion_key, $encrypted_text, $signature) {
  $key = base64_decode($encrytion_key);
  $iv = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
  $td = mcrypt_module_open('rijndael-128', '', 'cbc', '');
  // Decrypt the parameter text
  mcrypt_generic_init($td, $key, $iv);
  $text = mdecrypt_generic($td, base64_decode($encrypted_text));
  $text = _commerce_payway_net_pkcs5_unpad($text);
  mcrypt_generic_deinit($td);
  // Decrypt the signature value
  mcrypt_generic_init($td, $key, $iv);
  $hash = mdecrypt_generic($td, base64_decode($signature));
  $hash = bin2hex(_commerce_payway_net_pkcs5_unpad($hash));
  mcrypt_generic_deinit($td);
  mcrypt_module_close($td);
  // Compute the MD5 hash of the parameters
  $text_hash = md5($text);
  // Check the provided MD5 hash against the computed one
  if ($text_hash != $hash) {
    trigger_error("Invalid parameters signature");
  }
  $params = array();
  // Loop through each parameter provided
  foreach (explode( "&", $text ) as $parameter) {
    list($name, $value) = explode("=", $parameter);
    $params[urldecode($name)] = urldecode($value);
  }
  return $params;
}

/**
 * Encrypted strings are padded, this reverses that.
 */
function _commerce_payway_net_pkcs5_unpad($text) {
  $pad = ord($text{strlen($text)-1});
  if ($pad > strlen($text)) return false;
  if (strspn($text, chr($pad), strlen($text) - $pad) != $pad) return false;
  return substr($text, 0, -1 * $pad);
}
