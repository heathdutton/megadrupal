<?php

/**
 * @file
 * Straight forward PHP RC4 implementation.
 *
 * @see https://gist.github.com/farhadi/2185197
 */

/**
 * RC4 symmetric cipher encryption/decryption.
 *
 * @param string $key
 *   secret key for encryption/decryption
 * @param string $str
 *   string to be encrypted/decrypted
 *
 * @return string
 *   resulton string of RC4 encryption/decryption
 */
function _rc4_email_rc4($key, $str) {
  $s = array();
  for ($i = 0; $i < 256; $i++) {
    $s[$i] = $i;
  }
  $j = 0;
  for ($i = 0; $i < 256; $i++) {
    $j = ($j + $s[$i] + ord($key[$i % drupal_strlen($key)])) % 256;
    $x = $s[$i];
    $s[$i] = $s[$j];
    $s[$j] = $x;
  }
  $i = 0;
  $j = 0;
  $res = '';
  for ($y = 0; $y < drupal_strlen($str); $y++) {
    $i = ($i + 1) % 256;
    $j = ($j + $s[$i]) % 256;
    $x = $s[$i];
    $s[$i] = $s[$j];
    $s[$j] = $x;
    $res .= $str[$y] ^ chr($s[($s[$i] + $s[$j]) % 256]);
  }
  return $res;
}
