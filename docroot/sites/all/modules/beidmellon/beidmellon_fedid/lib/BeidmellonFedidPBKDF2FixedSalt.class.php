<?php
/**
 * @file
 * Password hashing with PBKDF2.
 * Copyright (c) 2013, Taylor Hornby
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Based on the implementation available at https://defuse.ca/php-pbkdf2.htm.
 *
 * Uses a given salt instead of a random salt to allow for quick lookup.
 */

/**
 * Class to encapsulate the PBKDF2 functions
 */
class BeidmellonFedidPBKDF2FixedSalt {

  const PBKDF2_HASH_ALGORITHM = "sha512";
  const PBKDF2_ITERATIONS = 500000;
  const PBKDF2_HASH_BYTES = 24;

  const HASH_SECTIONS = 3;
  const HASH_ALGORITHM_INDEX = 0;
  const HASH_ITERATION_INDEX = 1;
  const HASH_PBKDF2_INDEX = 2;

  /**
   * @var string
   *   the fixed, known hash to use.
   */
  protected $salt;

  /**
   * Constructor.
   *
   * @param string $salt
   *   the fixed, known hash to use.
   */
  public function __construct($salt) {
    if (!isset($salt)) {
      throw new Exception('PBKDF2 ERROR: Salt not set.');
    }
    $this->salt = $salt;
  }

  /**
   * Creates a hash for the given password.
   *
   * @param string $password
   *   The password to hash.
   *
   * @return string
   *   The hashed password in format "algorithm:iterations:salt:hash".
   */
  public function createHash($password) {
    // This uses a fixed salt.
    return BeidMellonFedidPBKDF2FixedSalt::PBKDF2_HASH_ALGORITHM . ":" . BeidMellonFedidPBKDF2FixedSalt::PBKDF2_ITERATIONS . ":" .
        base64_encode($this->hash(
            BeidMellonFedidPBKDF2FixedSalt::PBKDF2_HASH_ALGORITHM,
            $password,
            $this->salt,
            BeidMellonFedidPBKDF2FixedSalt::PBKDF2_ITERATIONS,
            BeidMellonFedidPBKDF2FixedSalt::PBKDF2_HASH_BYTES,
            TRUE
       ));
  }

  /**
   * Checks if the given password matches the given hash created by createHash.
   *
   * @param string $password
   *   The password to check.
   * @param string $good_hash
   *   The hash which should be match the password.
   *
   * @return bool
   *   TRUE if $password and $good_hash match, FALSE otherwise.
   *
   * @see createHash
   */
  public function validatePassword($password, $good_hash) {
    // This uses a fixed salt.
    $params = explode(":", $good_hash);
    if (count($params) < BeidMellonFedidPBKDF2FixedSalt::HASH_SECTIONS) {
      return FALSE;
    }
    $pbkdf2 = base64_decode($params[BeidMellonFedidPBKDF2FixedSalt::HASH_PBKDF2_INDEX]);
    return $this->slowEquals(
      $pbkdf2,
      $this->hash(
        $params[BeidMellonFedidPBKDF2FixedSalt::HASH_ALGORITHM_INDEX],
        $password,
        $this->salt,
        (int) $params[BeidMellonFedidPBKDF2FixedSalt::HASH_ITERATION_INDEX],
        strlen($pbkdf2),
        TRUE
      )
    );
  }

  /**
   * Compares two strings $a and $b in length-constant time.
   *
   * @param string $a
   *   The first string.
   * @param string $b
   *   The second string.
   *
   * @return bool
   *   TRUE if they are equal, FALSE otherwise.
   */
  public function slowEquals($a, $b) {
    $diff = strlen($a) ^ strlen($b);
    for ($i = 0; $i < strlen($a) && $i < strlen($b); $i++) {
      $diff |= ord($a[$i]) ^ ord($b[$i]);
    }
    return $diff === 0;
  }

  /**
   * PBKDF2 key derivation function.
   *
   * As defined by RSA's PKCS #5: https://www.ietf.org/rfc/rfc2898.txt
   * Test vectors can be found here: https://www.ietf.org/rfc/rfc6070.txt
   * This implementation of PBKDF2 was originally created by https://defuse.ca
   * With improvements by http://www.variations-of-shadow.com
   * Added support for the native PHP implementation by TheBlintOne
   *
   * @param string $algorithm
   *   The hash algorithm to use. Recommended: SHA256.
   * @param string $password
   *   The Password.
   * @param string $salt
   *   A salt that is unique to the password.
   * @param int $count
   *   Iteration count. Higher is better but slower. Recommended: At least 1000.
   * @param int $key_length
   *   The length of the derived key in bytes
   * @param bool $raw_output
   *   [optional] (default FALSE)
   *   If TRUE, the key is returned in raw binary format. Hex encoded otherwise.
   *
   * @return string
   *   A $key_length-byte key derived from the password and salt,
   *   depending on $raw_output this is either Hex encoded or raw binary.
   * @throws Exception
   *   If the hash algorithm are not found or if there are invalid parameters.
   */
  public function hash($algorithm, $password, $salt, $count, $key_length, $raw_output = FALSE) {
    $algorithm = strtolower($algorithm);
    if (!in_array($algorithm, hash_algos(), TRUE)) {
      throw new Exception('PBKDF2 ERROR: Invalid hash algorithm.');
    }
    if ($count <= 0 || $key_length <= 0) {
      throw new Exception('PBKDF2 ERROR: Invalid parameters.');
    }

    // Use the native implementation of the algorithm if available.
    if (function_exists("hash_pbkdf2")) {
      return hash_pbkdf2($algorithm, $password, $salt, $count, $key_length, $raw_output);
    }

    $hash_length = strlen(hash($algorithm, "", TRUE));
    $block_count = ceil($key_length / $hash_length);

    $output = "";
    for ($i = 1; $i <= $block_count; $i++) {
      // $i encoded as 4 bytes, big endian.
      $last = $salt . pack("N", $i);
      // First iteration.
      $last = $xorsum = hash_hmac($algorithm, $last, $password, TRUE);
      // Perform the other $count - 1 iterations.
      for ($j = 1; $j < $count; $j++) {
        $xorsum ^= ($last = hash_hmac($algorithm, $last, $password, TRUE));
      }
      $output .= $xorsum;
    }

    if ($raw_output) {
      return substr($output, 0, $key_length);
    }
    else {
      return bin2hex(substr($output, 0, $key_length));
    }
  }
}
