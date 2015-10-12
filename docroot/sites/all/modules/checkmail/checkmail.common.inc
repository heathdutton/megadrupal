<?php
/**
 * @file
 * This file contains common functionality used in multiple files.
 */

/**
 * Encrypts a user's password.
 *
 * This function gives the option of not encrypting, by simply returning the
 * password, if neither of the Encryption (http://drupal.org/project/encrypt) or
 * AES encryption (http://drupal.org/project/aes) modules are installed.
 *
 * @param string $password
 *   The password to encrypt.
 *
 * @return string
 *   The encrypted password.
 */
function _checkmail_encrypt_password($password) {
  if (variable_get('checkmail_use_encryption', 0) == 1) {
    if (module_exists('encrypt')) {
      // Encrypt the password.
      $password = encrypt($password);
    }
    elseif (module_exists('aes')) {
      // Encrypt the password.
      $password = aes_encrypt($password);
    }
  }

  return $password;
}

/**
 * Decrypts a user's password.
 *
 * This function gives the option of not decrypting, by simply returning the
 * password, if neither of the Encryption (http://drupal.org/project/encrypt) or
 * AES encryption (http://drupal.org/project/aes) modules are installed.
 *
 * @param string $password
 *   The password to decrypt.
 *
 * @return string
 *   The decrypted password.
 */
function _checkmail_decrypt_password($password) {
  if (variable_get('checkmail_use_encryption', 0) == 1) {
    if (module_exists('encrypt')) {
      // Decrypt the password.
      $password = decrypt($password);
    }
    elseif (module_exists('aes')) {
      // Decrypt the password.
      $password = aes_decrypt($password);
    }
  }

  return $password;
}
