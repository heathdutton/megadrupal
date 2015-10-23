<?php

/**
 * @file
 * commerce_sagepay_utils.inc
 * Common utilities shared by all Integration methods.
 */

/**
 * Helper function to use Commerce to fetch the Card names.
 */
function _commerce_sagepay_all_card_names() {

  return array(
    'visa'    => 'Visa',
    'visaelectron'    => 'Visa Electron',
    'mastercard'      => 'Mastercard',
    'amex'    => 'American Express',
    'delta'   => 'Delta',
    'dc'      => 'Diners Club',
    'jcb'     => 'JCB',
    'laser'   => 'Laser',
    'maestro' => 'Maestro',
  );

}

/**
 * The card types that SagePay supports.
 */
function _commerce_sagepay_all_card_types() {
  return array_keys(_commerce_sagepay_all_card_names());
}

/**
 * Populate the card names in to a usable array.
 *
 * @param array $cards
 *    Available card codes.
 *
 * @return array
 *    Array of card codes and names.
 */
function _commerce_sagepay_populate_card_names($cards) {
  $card_array = array();
  if (empty($cards)) {
    return $card_array;
  }
  $names = _commerce_sagepay_all_card_names();
  foreach (array_values($cards) as $c) {
    if ($c != '0') {
      if (array_key_exists($c, $names)) $card_array[$c] = $names[$c];
    }
  }

  return $card_array;
}


/**
 * Correctly format the customer name.
 *
 * Some configurations have a name line where others have a
 * first name and last name.
 * This function checks both scenarios and returns a consistent name array.
 *
 * @param string $name_line
 *    The name as a single line (eg Richard Jones).
 * @param string $first_name
 *    The first name only (eg Richard).
 * @param string $last_name
 *    The last name only (eg Jones).
 *
 * @return array
 *   An array of the name compoent parts.
 */
function _commerce_sagepay_format_customer_name($name_line, $first_name, $last_name) {

  // If we have been supplied with a $name_line variable, split it by space
  // and make the assumption that the last word is the last name and all other
  // are the first name.
  $name = array();

  if (isset($name_line)) {
    $split_name = explode(' ', trim($name_line));

    $last_name = array_pop($split_name);
    foreach ($split_name as $n) {
      $first_name .= $n;
      $first_name .= ' ';
    }

    // Allow for a situation where only one name was entered on the name line.
    if ($first_name == '') {
      $first_name = 'not set';
    }
  }

  $name['first_name'] = trim($first_name);
  $name['last_name'] = trim($last_name);

  return $name;
}

/**
 * Encrypt a string ready to send to SagePay using encryption key.
 *
 * @param string $string
 *    The unencrypyted string.
 * @param string $key
 *    The encryption key.
 *
 * @return string
 *    The encoded string.
 */
function _commerce_sagepay_encrypt_and_encode($string, $key) {

  // AES encryption, CBC blocking with PKCS5 padding then HEX encoding.
  // Add PKCS5 padding to the text to be encypted.
  $string = _commerce_sagepay_addPKCS5Padding($string);

  // Perform encryption with PHP's MCRYPT module.
  $crypt = mcrypt_encrypt(MCRYPT_RIJNDAEL_128, $key, $string, MCRYPT_MODE_CBC, $key);

  // Perform hex encoding and return.
  return "@" . bin2hex($crypt);

}

/**
 * Decode a returned string from SagePay.
 *
 * @param string $str_in
 *    The encrypted String.
 * @param string $str_encryption_password
 *   The encyption password used to encrypt the string.
 *
 * @return string
 *   The unecrypted string.
 */
function _commerce_sagepay_decode_and_decrypt($str_in, $str_encryption_password) {

  // HEX decoding then AES decryption, CBC blocking with PKCS5 padding.
  // Use initialization vector (IV) set from $str_encryption_password.
  $str_iv = $str_encryption_password;

  // Remove the first char which is @ to flag this is AES encrypted.
  $str_in = substr($str_in, 1);

  // HEX decoding.
  $str_in = pack('H*', $str_in);

  // Perform decryption with PHP's MCRYPT module.
  return _commerce_sagepay_removePKCS5Padding(mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $str_encryption_password, $str_in, MCRYPT_MODE_CBC, $str_iv));

}

/**
 * Remove PKCS5 Padding from a string.
 *
 * @param string $decrypted
 *    The decrypted string.
 *
 * @return string
 *   String without the padding.
 */
function _commerce_sagepay_removePKCS5Padding($decrypted) {
  $pad_char = ord($decrypted[strlen($decrypted) - 1]);

  return substr($decrypted, 0, -$pad_char);
}


/**
 * PHP's mcrypt does not have built in PKCS5 Padding, so we use this.
 *
 * @param string $input
 *  The input string.
 *
 * @return string
 *  The string with padding.
 */
function _commerce_sagepay_addPKCS5Padding($input) {
  $blocksize = 16;
  $padding = "";

  // Pad input to an even block size boundary.
  $padlength = $blocksize - (strlen($input) % $blocksize);
  for ($i = 1; $i <= $padlength; $i++) {
    $padding .= chr($padlength);
  }

  return $input . $padding;
}

/**
 * Decode SagePay response.
 *
 * @param string $scrambled
 *    The encoded string.
 *
 * @return string
 *   The decoded string.
 */
function _commerce_sagepay_base64Decode($scrambled) {
  $corrected = str_replace(" ", "+", $scrambled);
  $output = base64_decode($corrected);

  return $output;
}

/**
 * Validate a string against allowed parameters in the SagePay Protocol.
 *
 * @param string $text
 *    The string to validate.
 * @param int $max_length
 *   The maximum permitted length of the string.
 * @param string $type
 *   The type of validation.
 * @return mixed
 *   The validated string.
 */
function _commerce_sagepay_validate_input($text, $max_length, $type = SAGEPAY_VALIDATE_ADDRESS) {

  // Trim text to max permitted length.
  if (strlen($text) > $max_length) {
    $text = substr($text, 0, $length);
  }

  // Strip unwanted characters.
  $accents = '/&([A-Za-z]{1,2})(grave|acute|circ|cedil|uml|lig);/';
  $text_encoded = htmlentities($text, ENT_NOQUOTES, 'UTF-8');
  $text = preg_replace($accents, '$1', $text_encoded);
  switch ($type) {
    case SAGEPAY_VALIDATE_NAME:
      $pat = '/[^(A-Za-z& \/\.\'\-)]*/';
      break;

    case SAGEPAY_VALIDATE_ADDRESS:
      $pat = '/[^(A-Za-z0-9&:, \/\.\'\+\-\{\})]*/';
      break;

    case SAGEPAY_VALIDATE_POSTCODE:
      $pat = '/[^(A-Za-z0-9 \-)]*/';
      break;

    case SAGEPAY_VALIDATE_EMAIL:
      // @todo could check for a valid email here, but need to know what
      // to do if it isn't valid.
      return $text;
  }

  return preg_replace($pat, '', $text);
}

/**
 * Substitute Test card data.
 *
 * For testing purposes, SagePay needs one of a set of test card numbers and
 * the following test data:
 * Billing Address1 = '88'
 * Billing Postcode = '412'
 * Expiry date = Sometime in the future
 * CV2 = 123
 * Visa test card number: 4929000000006
 * Mastercard test card number: 5404000000000001
 *
 * @param commerce_payment_transaction $transaction
 *    The transaction to be modified.
 */
function _commerce_sagepay_substitute_for_test_data(&$transaction) {
  watchdog('commerce_sagepay_direct', 'Processing transaction in test mode so overwriting entered data with test data', array(), WATCHDOG_WARNING);
  $transaction['BillingAddress1'] = '88';
  $transaction['BillingPostcode'] = '412';
  $transaction['CV2'] = '123';
  $transaction['CardNumber'] = '5404000000000001';
  $transaction['ExpiryDate'] = '1230';
  $transaction['CardType'] = 'MC';
}

/**
 * Convert a data array to a single string ready to post.
 *
 * @param array $data
 *    The data array.
 * @return string
 *   The array as a string.
 */
function _commerce_sagepay_array_to_post($data) {

  $post = '';
  foreach ($data as $name => $value) {
    $post .= urlencode($name) . '=' . urlencode($value) . '&';
  }

  // Chop off the last &.
  $post = substr($post, 0, -1);

  return $post;
}

/**
 * Remove unwanted characters from strings.
 *
 * @param string $text
 *    The string to clean.
 * @param string $type
 *   The type of string being cleaned.
 * @return string
 *   The clean string.
 */
function _commerce_sagepay_clean_input($text, $type) {

  $clean_text = check_plain($text);
  return $clean_text;

}
