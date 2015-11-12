<?php

/**
 * @file
 * Security hash specific functionality.
 */

/**
 * Generates the hash for a payment.
 */
function commerce_epayment_generate_hash($data, $settings) {
  $str = _commerce_epayment_expand_string($data['MERCHANT']);
  $str .= _commerce_epayment_expand_string($data['ORDER_REF']);
  $str .= _commerce_epayment_expand_string($data['ORDER_DATE']);
  $str .= _commerce_epayment_expand_array($data['ORDER_PNAME']);
  $str .= _commerce_epayment_expand_array($data['ORDER_PCODE']);
  $str .= _commerce_epayment_expand_array($data['ORDER_PINFO']);
  $str .= _commerce_epayment_expand_array($data['ORDER_PRICE']);
  $str .= _commerce_epayment_expand_array($data['ORDER_QTY']);
  $str .= _commerce_epayment_expand_array($data['ORDER_VAT']);
  $str .= _commerce_epayment_expand_string($data['ORDER_SHIPPING']);
  $str .= _commerce_epayment_expand_string($data['PRICES_CURRENCY']);

  return commerce_epayment_hmac($settings['key'], $str);
}

/**
 * Generates hmac for a request.
 */
function commerce_epayment_hmac($key, $data) {
  $b = 64; // byte length for md5
  if (strlen($key) > $b) {
    $key = pack("H*",md5($key));
  }
  $key  = str_pad($key, $b, chr(0x00));
  $ipad = str_pad('', $b, chr(0x36));
  $opad = str_pad('', $b, chr(0x5c));
  $k_ipad = $key ^ $ipad;
  $k_opad = $key ^ $opad;

  return md5($k_opad  . pack("H*", md5($k_ipad . $data)));
}

/**
 * Outputs a string for hmac format. For a string like 'a' it will return '1a'.
 */
function _commerce_epayment_expand_string($string) {
  return strlen($string) . $string;
}

/**
 * The same as _commerce_epayment_expand_string except that it receives
 * an array of strings and returns the string from all values within the array.
 */
function _commerce_epayment_expand_array($array) {
  $return = '';
  for ($i = 0; $i < count($array); $i++) {
    $return .= _commerce_epayment_expand_string($array[$i]);
  }
  return $return;
}
