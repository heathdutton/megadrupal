<?php

/**
 * @file
 * Giftcard transaction controller class.
 */

class CommerceGCTransactionEntityController extends DrupalCommerceEntityController {
  public $amount;

  public $transaction_id;

  public $coupon_id;

  public $status;

  public $date;


  /**
   * Create a default transaction.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   * @return
   *   A product object with all default fields initialized.
   */
  public function create(array $values = array()) {
    $values += array(
      'coupon_id' => '',
      'transaction_id' => '',
      'amount' => 0,
      'date' => '',
      'status' => 'completed'
    );

    return parent::create($values);
  }

  /**
   * Saves a transaction with a created date if it is new.
   *
   * @param $commerce_coupon
   *   The full coupon object to save.
   * @param $transaction
   *   An optional transaction object.
   *
   * @return
   *   SAVED_NEW or SAVED_UPDATED depending on the operation performed.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {

    if (empty($entity->{$this->idKey}) || !empty($entity->is_new)) {
      // Set the creation timestamp if not set, for new entities.
      if (empty($entity->date)) {
        $entity->date = REQUEST_TIME;
      }
    }

    // We do not do a transaction on updates nor do we do any balance checking.
    // Only administrators should ever be updating transaction amounts, and
    // they may have reasons for taking a giftcard balance below zero.
    if (empty($entity->is_new)) {
      return parent::save($entity);
    }

    // Ensure some level of data integrity before starting the transaction.
    if (!isset($entity->amount) || empty($entity->coupon_id)) {
      throw new Exception('Unable to save giftcard transaction: required properties missing');
    }

    // Insert operations incorporate a transactional balance check.
    if (!isset($transaction)) {
      $transaction = db_transaction();
    }

    try {
      // Load the balance. Set the forUpdate arg to TRUE to lock any matching
      // rows.
      $balance = commerce_gc_giftcard_balance($entity->coupon_id, TRUE);

      // If the transaction is a debit, only save if balance is sufficient.
      if ($entity->amount > 0 || abs($entity->amount) <= $balance) {
        return parent::save($entity, $transaction);
      }

      // Otherwise cancel the transaction to free the locked rows.
      $transaction->rollback();
    }
    catch (Exception $e) {
      // Roll back the transaction if there was a problem like a lock timeout
      // or some other error.
      $transaction->rollback();
      watchdog_exception($this->entityType, $e);
      throw $e;
    }
  }
}
