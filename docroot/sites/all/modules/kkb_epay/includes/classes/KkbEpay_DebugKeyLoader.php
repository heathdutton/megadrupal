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
 * Loader of a private key provided by the bank for testing.
 *
 * This private key works only with the sandbox gateway.
 */
class KkbEpay_DebugKeyLoader implements KkbEpay_KeyLoaderInterface {

  protected $debug = FALSE;
  protected $filepath;


  public function getKey() {
    if (!$this->debug) {
      throw new KkbEpay_KeyException('Debug key can be loaded only when debugging mode is turned on. Call setDebug() method with TRUE parameter.');
    }
    $key = new KkbEpay_Key();

    $key->setCertificateId('00C182B189');
    $key->setMerchantName('Demo sShop');
    $key->setMerchantId('92061101');
    $key->setPassword('nissan');
    $key->setKey(trim(file_get_contents($this->getKeyFilepath())));

    return $key;
  }

  public function setDebug($flag) {
    $this->debug = (bool) $flag;
  }

  public function validateKey() {
    return TRUE;
  }

  /**
   * Returns configured path to the PEM-encoded file with the private key.
   *
   * By default, private key file is searched using relative path to this
   * file, but its location can be changed with setKeyFilepath().
   *
   * @note
   *   Private key body cannot be included directly into this file because this
   *   file is licenced under GPL and cannot include binary blobs. Encoded PEM
   *   key is such binary blob. Furthermore, its copyright status is not clear.
   */
  public function getKeyFilepath() {
    if (!isset($this->filepath)) {
      $default_path = __DIR__ . '/../../data/debug_private_key.pem';
      $this->setKeyFilepath($default_path);
    }
    return $this->filepath;
  }

  /**
   * Changes path to the PEM-encoded private key file.
   */
  public function setKeyFilepath($filepath) {
    if (!is_file($filepath) || !is_readable($filepath)) {
      throw new KkbEpay_Exception('File with debug private key cannot be read.');
    }
    $this->filepath = $filepath;
    return $this;
  }

}
