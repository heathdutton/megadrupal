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
 * Representation of a RSA private key that can be used to sign text messages.
 *
 * All setters in this class perform input validation. If any validation
 * constraint is violated, a KkbEpay_KeyException is thrown.
 */
final class KkbEpay_Key {

  private $key;

  private $password;

  private $merchant_id;

  private $merchant_name;

  private $certificate_id;


  public function __construct($key = NULL, $pwd = NULL, $c_id = NULL, $m_id = NULL, $m_name = NULL) {
    if ($key) {
      $this->setKey($key);
    }
    if ($pwd) {
      $this->setPassword($pwd);
    }
    if ($c_id) {
      $this->setCertificateId($c_id);
    }
    if ($m_id) {
      $this->setMerchantId($m_id);
    }
    if ($m_name) {
      $this->setMerchantName($m_name);
    }
  }

  public function isValid() {
    if (empty($this->key)) {
      return FALSE;
    }
    if (empty($this->merchant_id)) {
      return FALSE;
    }
    if (empty($this->merchant_name)) {
      return FALSE;
    }
    if (empty($this->certificate_id)) {
      return FALSE;
    }

    return TRUE;
  }

  public function setKey($key) {
    if (!is_string($key)) {
      throw new KkbEpay_KeyException('Key must be a string.');
    }
    if (FALSE === $start = strpos($key, '-----BEGIN RSA PRIVATE KEY-----')) {
      throw new KkbEpay_KeyException('Key is not valid, it does not contain a correct RSA key declaration.');
    }
    if (FALSE === $end = strpos($key, '-----END RSA PRIVATE KEY-----')) {
      throw new KkbEpay_KeyException('Key is not valid, it does not contain a correct RSA key declaration.');
    }
    if ($end < $start) {
      throw new KkbEpay_KeyException('Key is not valid.');
    }

    $key = substr($key, $start);
    $key = substr($key, 0, $end + 29);
    if (empty($key)) {
      throw new KkbEpay_KeyException('Key is not valid.');
    }

    $this->key = trim($key);
    return $this;
  }

  public function setPassword($pwd) {
    if (!is_string($pwd)) {
      throw new KkbEpay_KeyException('Password must be a string.');
    }
    $this->password = $pwd;
    return $this;
  }

  public function setMerchantId($id) {
    if (!is_string($id)) {
      throw new KkbEpay_KeyException('Merchant ID must be a string.');
    }
    if (!preg_match('/^[0-9]{8}$/', $id)) {
      throw new KkbEpay_KeyException('Merchant ID does not match expected format. It must consist of 8 digits.');
    }
    $this->merchant_id = $id;
    return $this;
  }

  public function setMerchantName($name) {
    if (!is_string($name)) {
      throw new KkbEpay_KeyException('Merchant name must be a string.');
    }
    if (strlen($name) > 255) {
      throw new KkbEpay_KeyException('Merchant name is too long. It cannot be longer than 255 characters.');
    }
    if (!preg_match('/^[A-Za-z0-9 _.-]{1,255}$/', $name)) {
      throw new KkbEpay_KeyException('Merchant name does not match expected format. It can consist only of English letters, digits, a dot, \'-\', \'_\' and space.');
    }
    $this->merchant_name = $name;
    return $this;
  }

  public function setCertificateId($id) {
    if (!is_string($id)) {
      throw new KkbEpay_KeyException('Certificate ID must be a string.');
    }
    if (!preg_match('/^[A-Fa-f0-9]{8,10}$/', $id)) {
      throw new KkbEpay_KeyException('Certificate ID does not match expected format. It must be 8-10 characters long and consist of hexadecimal digits.');
    }
    $this->certificate_id = strtoupper($id);
    return $this;
  }

  public function getKey() {
    return $this->key;
  }

  public function getPassword() {
    return empty($this->password) ? '' : $this->password;
  }

  public function getMerchantId() {
    return $this->merchant_id;
  }

  public function getMerchantName() {
    return $this->merchant_name;
  }

  public function getCertificateId() {
    return $this->certificate_id;
  }

}
