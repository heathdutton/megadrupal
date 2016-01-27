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
 * Representation of an abstract order that will be payed using KKB Epay
 * processing center. Contains an absolute minimal amount on information to
 * create a payment document for the processing center.
 */
class KkbEpay_Order {

  protected $id;

  protected $amount;

  protected $currency;

  protected $items = array();


  public function __construct($order_id = NULL) {
    if (!empty($order_id)) {
      $this->setId($order_id);
    }
  }

  /**
   * Validates, sets order ID and modifies it if neccessary.
   *
   * Order ID must consist of at least 6 digits. If this method is called with
   * an ID that does not have a valid format, it will be modified to conform
   * with all requirements.
   *
   * This ID must uniquely identify order on the site, as it will be sent
   * back by the processing center with the results of the operation.
   */
  public function setId($id) {
    $id = (string) $id;
    if (!preg_match('/^[0-9]+$/', $id)) {
      throw new KkbEpay_Exception('Order ID can consist only of digits.');
    }
    $this->id = str_pad($id, 6, '0', STR_PAD_LEFT);
    return $this;
  }

  /**
   * Sets both order amount and used currency.
   *
   * Currency must be specified with its code. Only few currencies are
   * supported.
   */
  public function setAmount($amount, $currency_code) {
    if (!$this->isSuportedCurrency($currency_code)) {
      throw new KkbEpay_Exception('Unsupported currency.');
    }
    $this->currency = $currency_code;
    $this->amount = $amount;
    return $this;
  }

  public function setTengeAmount($amount) {
    return $this->setAmount($amount, 398);
  }

  public function setItems(array $items) {
    $this->items = array();
    foreach ($items as $i) {
      $this->addItem($i);
    }
    return $this;
  }

  public function addItem(KkbEpay_OrderItem $i) {
    $this->items[] = $i;
    $i->setNumber(count($this->items));
    return $this;
  }

  public function getId() {
    return $this->id;
  }

  public function getAmount() {
    return $this->amount;
  }

  public function getCurrency() {
    return $this->currency;
  }

  public function getItems() {
    return $this->items;
  }

  protected function isSuportedCurrency($code) {
    switch ($code) {
      // Tenge.
      case 398:
        return TRUE;
    }

    return FALSE;
  }

}
