<?php

/**
 * @file
 * Secure password hashing functions for user authentication.
 *
 * This file provides customised version of
 */

/**
 * The standard log2 number of iterations for password stretching. This should
 * increase by 1 every Drupal version in order to counteract increases in the
 * speed and power of computers available to crack the hashes.
 */
define('DRUPAL_HASH_COUNT', 15);

/**
 * The minimum allowed log2 number of iterations for password stretching.
 */
define('DRUPAL_MIN_HASH_COUNT', 7);

/**
 * The maximum allowed log2 number of iterations for password stretching.
 */
define('DRUPAL_MAX_HASH_COUNT', 30);

/**
 * The expected (and maximum) number of characters in a hashed password.
 */
define('DRUPAL_HASH_LENGTH', 55);

/**
 * Hash a password using a secure stretched hash.
 *
 * By using a salt and repeated hashing the password is "stretched". Its
 * security is increased because it becomes much more computationally costly
 * for an attacker to try to break the hash by brute-force computation of the
 * hashes of a large number of plain-text words or strings to find a match.
 *
 * @param string $algo
 *   The string name of a hashing algorithm usable by hash(), like 'sha256'.
 * @param string $password
 *   The plain-text password to hash.
 * @param string $setting
 *   An existing hash or the output of _password_generate_salt().  Must be
 *   at least 12 characters (the settings and salt).
 *
 * @return string
 *   A string containing the hashed password (and salt) or FALSE on failure.
 *   The return string will be truncated at DRUPAL_HASH_LENGTH characters max.
 */
function _password_crypt($algo, $password, $setting) {
  // The first 12 characters of an existing hash are its setting string.
  $setting = substr($setting, 0, 12);

  if ($setting[0] != '$' || $setting[2] != '$') {
    return FALSE;
  }
  $count_log2 = _password_get_count_log2($setting);
  // Hashes may be imported from elsewhere, so we allow != DRUPAL_HASH_COUNT
  if ($count_log2 < DRUPAL_MIN_HASH_COUNT || $count_log2 > DRUPAL_MAX_HASH_COUNT) {
    return FALSE;
  }
  $salt = substr($setting, 4, 8);
  // Hashes must have an 8 character salt.
  if (strlen($salt) != 8) {
    return FALSE;
  }

  // Convert the base 2 logarithm into an integer.
  $count = 1 << $count_log2;

  // We rely on the hash() function being available in PHP 5.2+.
  $hash = hash($algo, $salt . $password, TRUE);
  do {
    $hash = hash($algo, $hash . $password, TRUE);
  } while (--$count);

  $len = strlen($hash);
  $output = $setting . _password_base64_encode($hash, $len);
  // _password_base64_encode() of a 16 byte MD5 will always be 22 characters.
  // _password_base64_encode() of a 64 byte sha512 will always be 86 characters.
  $expected = 12 + ceil((8 * $len) / 6);
  return (strlen($output) == $expected) ? substr($output, 0, DRUPAL_HASH_LENGTH) : FALSE;
}

/**
 * Parse the log2 iteration count from a stored hash or setting string.
 */
function _password_get_count_log2($setting) {
  $itoa64 = _password_itoa64();
  return strpos($itoa64, $setting[3]);
}

/**
 * Returns a string for mapping an int to the corresponding base 64 character.
 */
function _password_itoa64() {
  return './0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';
}

/**
 * Encodes bytes into printable base 64 using the *nix standard from crypt().
 *
 * @param string $input
 *   The string containing bytes to encode.
 * @param int $count
 *   The number of characters (bytes) to encode.
 *
 * @return string
 *   Encoded string
 */
function _password_base64_encode($input, $count) {
  $output = '';
  $i = 0;
  $itoa64 = _password_itoa64();
  do {
    $value = ord($input[$i++]);
    $output .= $itoa64[$value & 0x3f];
    if ($i < $count) {
      $value |= ord($input[$i]) << 8;
    }
    $output .= $itoa64[($value >> 6) & 0x3f];
    if ($i++ >= $count) {
      break;
    }
    if ($i < $count) {
      $value |= ord($input[$i]) << 16;
    }
    $output .= $itoa64[($value >> 12) & 0x3f];
    if ($i++ >= $count) {
      break;
    }
    $output .= $itoa64[($value >> 18) & 0x3f];
  } while ($i < $count);

  return $output;
}

/**
 * Hash a password using a secure hash.
 *
 * @param string $password
 *   A plain-text password.
 * @param int $count_log2
 *   Optional integer to specify the iteration count. Generally used only during
 *   mass operations where a value less than the default is needed for speed.
 *
 * @return bool
 *   A string containing the hashed password (and a salt), or FALSE on failure.
 */
function user_hash_password($password, $count_log2 = 0) {
  if (($library = libraries_load('password_compat')) && !empty($library['loaded'])) {
    return password_hash($password, PASSWORD_BCRYPT);
  }
  else {
    return FALSE;
  }
}

/**
 * Check whether a plain text password matches a stored hashed password.
 *
 * @param text $password
 *   A plain-text password
 * @param object $account
 *   A user object with at least the fields from the {users} table.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function user_check_password($password, $account) {
  // Password Compat uses $2y$ as the prefix.
  if (substr($account->pass, 0, 4) == '$2y$') {
    if (($library = libraries_load('password_compat')) && !empty($library['loaded'])) {
      if (password_verify($password, $account->pass)) {
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    else {
      drupal_set_message(t('Currently unable to verify password'), 'error');
      watchdog('password_compat', 'Unable to load Password Compat Library while verifying password');
      return FALSE;
    }
  }

  elseif (substr($account->pass, 0, 2) == 'U$') {
    // This may be an updated password from user_update_7000(). Such hashes
    // have 'U' added as the first character and need an extra md5().
    $stored_hash = substr($account->pass, 1);
    $password = md5($password);
  }
  else {
    $stored_hash = $account->pass;
  }

  $type = substr($stored_hash, 0, 3);
  switch ($type) {
    case '$S$':
      // A normal Drupal 7 password using sha512.
      $hash = _password_crypt('sha512', $password, $stored_hash);
      break;

    case '$H$':
      // phpBB3 uses "$H$" for the same thing as "$P$".
    case '$P$':
      // A phpass password generated using md5.  This is an
      // imported password or from an earlier Drupal version.
      $hash = _password_crypt('md5', $password, $stored_hash);
      break;

    default:
      return FALSE;
  }
  return ($hash && $stored_hash == $hash);
}

/**
 * Check whether a user's hashed password needs to be replaced with a new hash.
 *
 * @param object $account
 *   A user object with at least the fields from the {users} table.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function user_needs_new_hash($account) {
  // Check whether this was an updated password.
  if (substr($account->pass, 0, 4) != '$2y$') {
    return TRUE;
  }
}
