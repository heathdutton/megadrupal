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
 * Representation of a single product or service in the 'appendix' field in
 * the payment details that are submited to the processing center.
 */
class KkbEpay_OrderItem {

  protected $name;

  protected $number;

  protected $amount;

  protected $quantity;


  public function __construct($name, $amount = 0, $quantity = 1) {
    $this->setName($name);
    $this->setAmount($amount);
    $this->setQuantity($quantity);
  }

  public function setName($name) {
    $this->name = (string) $name;
    return $this;
  }

  public function setAmount($amount) {
    $this->amount = $amount;
    return $this;
  }

  public function setQuantity($quantity) {
    $this->quantity = (int) $quantity;
    return $this;
  }

  public function setNumber($number) {
    $this->number = (int) $number;
    return $this;
  }

  public function getName() {
    return $this->name;
  }

  public function getAmount() {
    return $this->amount;
  }

  public function getQuantity() {
    return $this->quantity;
  }

  public function getNumber() {
    return $this->number;
  }

}
