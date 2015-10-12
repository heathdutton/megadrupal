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
 * Representation of payment confirmation received from the processing server.
 *
 * @notice
 *   Object properties are immutable.
 */
class KkbEpay_PaymentNotification {

  protected $order_id;

  protected $timestamp;

  protected $amount;

  protected $reference;

  protected $approval_code;


  public function __construct(array $params = array()) {
    if (isset($params['order_id'])) {
      $this->order_id = $params['order_id'];
    }
    if (isset($params['timestamp'])) {
      $this->timestamp = $params['timestamp'];
    }
    if (isset($params['amount'])) {
      $this->amount = (double) $params['amount'];
    }
    if (isset($params['reference'])) {
      $this->reference = $params['reference'];
    }
    if (isset($params['approval_code'])) {
      $this->approval_code = $params['approval_code'];
    }
  }

  /**
   * Returns an order ID from the payment confirmation.
   *
   * Order ID must consist of at least 6 digits, so it may contain few padding
   * zeros at the begining.
   *
   * @param boolean $remove_padding
   *   - TRUE, padding zeros are removed from the beginning;
   *   - FALSE, order ID is returned as is.
   */
  public function getOrderId($remove_padding = TRUE) {
    $id = $this->order_id;
    if ($remove_padding) {
      return ltrim($id, '0');
    }
    return $id;
  }

  public function getTimestamp() {
    return $this->timestamp;
  }

  public function getAmount() {
    return $this->amount;
  }

  public function getReference() {
    return $this->reference;
  }

  public function getApprovalCode() {
    return $this->approval_code;
  }

}
