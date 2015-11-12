<?php

/**
 * @file
 * GitLab Account entity class.
 */

/**
 * GitLab Account entity class.
 */
class GitLabAccountEntity extends Entity {

  /**
   * Set the private token and encrypt it.
   *
   * @param string $private_token
   *   The private token wich will be encrypted and set.
   */
  public function setPrivateToken($private_token) {
    // Generate a random initialization vector for the encryption.
    $iv = openssl_random_pseudo_bytes(16);
    // AES-256-GCM is not supported by PHP right now so we use CBC instead.
    $this->private_token = $iv . openssl_encrypt($private_token, 'aes-256-cbc', $this->getPrivateTokenPass(), 0, $iv);
  }

  /**
   * Get the decrypted private token of the account.
   *
   * @returns string
   *   Returns with the decrypted private token.
   */
  public function getPrivateToken() {
    // Get the initialization vector and the encrypted private token.
    $iv = substr($this->private_token, 0, 16);
    $private_token = substr($this->private_token, 16);
    // Decrypt then encrypted private token.
    return openssl_decrypt($private_token, 'aes-256-cbc', $this->getPrivateTokenPass(), 0, $iv);
  }

  /**
   * Get the password for private token encryption/decryption.
   *
   * @returns string
   *   Returns with a substring of the $drupal_hash_salt variable.
   */
  private function getPrivateTokenPass() {
    return substr(drupal_get_hash_salt(), 0, 16);
  }

}
