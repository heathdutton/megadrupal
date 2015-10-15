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
 * Main key loader that fetches private keys from Drupal's variables system
 * where it is stored by the 'kkb_epay' module.
 */
class KkbEpay_DrupalKeyLoader implements KkbEpay_KeyLoaderInterface {

  protected $debug = FALSE;


  public function getKey() {
    $data = $this->loadKeyData();
    return new KkbEpay_Key(
      $data['key'],
      $data['password'],
      $data['certificate_id'],
      $data['merchant_id'],
      $data['merchant_name']
    );
  }

  public function setDebug($flag) {
    $this->debug = (bool) $flag;
  }

  public function validateKey() {
    try {
      return $this->getKey()->isValid();
    }
    catch (KkbEpay_Exception $ex) {
      return FALSE;
    }
  }


  protected function loadKeyData() {
    $variant = 'kkb_epay_private_key_' . ($this->debug ? 'debug' : 'live');

    if (!($key_data = variable_get($variant, NULL))) {
      throw new KkbEpay_KeyException('Private key does not exist as a system variable.');
    }
    if (!($unpacked = $this->unpack($key_data))) {
      throw new KkbEpay_KeyException('Private key could not be unpacked.');
    }

    return $unpacked;
  }

  protected function unpack($data) {
    return unserialize(base64_decode(preg_replace('/\s/', '', $data)));
  }

}
