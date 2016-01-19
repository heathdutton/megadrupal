<?php

/**
 * @file
 * Helpers for Cache Bypass Path.
 */

/**
 * Generates a new hash.
 *
 * @return string
 *  A new hash that doesn't match the existing one.
 */
function cache_bypass_path_reset_hash() {
  $existing_hash = variable_get('cache_bypass_path_hash', NULL);
  $new_hash = drupal_hmac_base64(openssl_random_pseudo_bytes(10), openssl_random_pseudo_bytes(10));
  while ($new_hash == $existing_hash) {
    $new_hash = drupal_hmac_base64(openssl_random_pseudo_bytes(10), openssl_random_pseudo_bytes(10));
  }
  variable_set('cache_bypass_path_hash', $new_hash);
  return $new_hash;
}
