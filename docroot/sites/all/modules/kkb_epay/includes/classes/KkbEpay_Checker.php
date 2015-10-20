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
 * Signature checker.
 */
final class KkbEpay_Checker {

  private $certificate;


  public function __construct(KkbEpay_BankCertificateInterface $certificate = NULL) {
    if (empty($certificate)) {
      $certificate = new KkbEpay_DefaultBankCertificate();
    }

    $resource = openssl_get_publickey($certificate->getCertificate());
    if (empty($resource)) {
      $error = 'Provided certificate could not be loaded by openssl_get_publickey().';
      $previous = new KkbEpay_OpenSSLException(strval(openssl_error_string()));
      throw new KkbEpay_Exception($error, 0, $previous);
    }

    $this->certificate = $resource;
  }

  /**
   * Checks given message with loaded public certificate.
   *
   * @param string $message
   *   The original message whose signature must be verified.
   *   Cannot be empty.
   *
   * @param string $signature
   *   Verified signature as a binary string.
   *
   * @return boolean
   *   TRUE if signature is corrent.
   *   FALSE if signature was not verified.
   */
  public function check($message, $signature) {
    if (empty($message)) {
      throw new KkbEpay_Exception('Message cannot be empty.');
    }

    // Reasons why reverse operation is used are the same as in the
    // KkbEpay_Sign::sign method.
    $signature = $this->reverse($signature);

    return openssl_verify($message, $signature, $this->certificate);
  }

  /**
   * Decodes signature with base64 and checks with the check() method.
   */
  public function check64($message, $signature) {
    return $this->check($message, base64_decode($signature));
  }


  private function reverse($data) {
    return strrev($data);
  }

}
