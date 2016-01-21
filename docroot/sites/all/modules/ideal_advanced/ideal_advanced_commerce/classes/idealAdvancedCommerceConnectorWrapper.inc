<?php

/**
 * @file
 * Defines the classes for ideal to commerce implementation.
 */

/**
 * Set iDEALConnector namespaces.
 */

use iDEALConnector\Entities\AcquirerTransactionResponse as AcquirerStatusResponse;

/**
 * Extend the IdealAdvancedConnectorWrapper class for Commerce.
 */
class IdealAdvancedCommerceConnectorWrapper extends IdealAdvancedConnectorWrapper {

  /**
   * The commerce order object.
   *
   * @var object $order
   *   An commerce order object.
   */

  private $order;

  /**
   * Set the commerce order.
   *
   * @var object $order
   *   An commerce order object.
   */
  public function setOrder($order) {
    $this->order = $order;
  }

  /**
   * Extends startTransaction.
   *
   * @return object
   *   Available methods are:
   *    ->getTransactionID()
   *    ->getAcquirerID()
   *    ->getIssuerAuthenticationURL()
   */
  public function startTransaction() {
    $this->setTransactionValues();
    $response = parent::startTransaction();

    if (!empty($response)) {
      $this->writeMapRow($response, $this->order);
      return $response;
    }
    return NULL;
  }

  /**
   * Write mapping to link the Commerce transaction to the iDEAL transaction.
   *
   * @param AcquirerStatusResponse $response
   *   The issuers response object.
   * @param object $order
   *   The commerce order object.
   */
  protected function writeMapRow(AcquirerStatusResponse $response, $order) {
    $transaction_id = $response->getTransactionID();
    $transaction = ideal_advanced_transaction_load_by_transaction_id($transaction_id);

    db_insert('ideal_advanced_commerce')
      ->fields(array(
        'ideal_id' => $transaction->ideal_id,
        'payment_id' => $order->data['payment_id'],
        'order_id' => $order->order_id,
      )
    )->execute();
  }

  /**
   * Set the transaction values to the connector class.
   */
  private function setTransactionValues() {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $this->order);

    $return_url = url('checkout/' . $order_wrapper->order_id->value() . '/payment/return/' . $this->order->data['payment_redirect_key'], array('absolute' => TRUE));
    $description = 'order_' . $order_wrapper->order_id->value();
    $purchase_id = $order_wrapper->order_id->value();

    drupal_alter('ideal_advanced_commerce_transaction', $purchase_id, $description, $this->order);

    $this->setAmount($order_wrapper->commerce_order_total->amount->value());
    $this->setReturnUrl($return_url);
    $this->setIssuerId($this->order->data['issuer_id']);
    $this->setDescription($description);
    $this->setEntranceCode($purchase_id);
    $this->setPurchaseId($purchase_id);
  }

  /**
   * Extends updateTransactionStatus from IdealAdvancedConnectorWrapper.
   *
   * When a status call is requested, the commerce payment is also updated.
   *
   * @param int $ideal_id
   *   The ideal advanced transaction id.
   *
   * @return object $transaction
   *   The current ideal transaction entity object.
   */
  public function updateTransactionStatus($ideal_id) {
    $order = ideal_advanced_commerce_order_load_by_ideal_id($ideal_id);
    $transaction = ideal_advanced_transaction_load($ideal_id);
    $status_old = $transaction->status;

    parent::updateTransactionStatus($ideal_id);

    // The transaction is passed by reference. We don't have to load it again.
    if (isset($order) && $status_old != $transaction->status) {
      switch ($transaction->status) {
        case IDEAL_STATUS_SUCCESS:
          $status = COMMERCE_PAYMENT_STATUS_SUCCESS;
          break;

        case IDEAL_STATUS_OPEN:
          $status = COMMERCE_PAYMENT_STATUS_PENDING;
          break;

        case IDEAL_STATUS_CANCELLED:
          $status = COMMERCE_PAYMENT_STATUS_PENDING;
          break;

        default:
          $status = COMMERCE_PAYMENT_STATUS_FAILURE;
          break;
      }

      $payment = ideal_advanced_commerce_commerce_payment_load($order);
      ideal_advanced_commerce_commerce_payment_set_status($payment->transaction_id, $status);
    }
    return $transaction;
  }

}
