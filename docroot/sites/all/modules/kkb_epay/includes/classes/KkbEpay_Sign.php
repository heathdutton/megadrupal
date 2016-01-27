<?php

/**
 * @file
 * Copyright (C) 2012, Victor Nikulshin
 *
 * This file is part of KKB Epay.
 *
 * KKB Epay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * KKB Epay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KKB Epay.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Main signer of messages.
 */
final class KkbEpay_Sign {

  private $key;


  public function __construct(KkbEpay_Key $key) {
    if (!$key->isValid()) {
      throw new KkbEpay_Exception('Provided key is not valid.');
    }

    $this->key = $key->open();
  }

  /**
   * Signs given message with loaded private key.
   *
   * @param string $message
   *   Message that must be signed, as a string. Cannot be empty.
   *
   * @return string
   *   Signature as a binary string.
   */
  public function sign($message) {
    if (empty($message)) {
      throw new KkbEpay_Exception('Message cannot be empty.');
    }

    $signature = '';
    openssl_sign($message, $signature, $this->key);

    // The reason why reverse() must be used here is mysterious.
    // But this action was performed in the original signing code that
    // was provided by the processing center. And the processing center
    // will perform reverse() operation too when it validates this
    // signature. Thus, this action is required.
    return $this->reverse($signature);
  }

  /**
   * Signs the message with sign() and encodes signature with base64.
   */
  public function sign64($message) {
    return base64_encode($this->sign($message));
  }


  private function reverse($data) {
    return strrev($data);
  }

}
