<?php

/**
 * @file
 * Contains entity controller implementation for cmp_funds_transaction.
 */

/**
 * Class CommerceMarketplaceFundsTransactionController.
 * cmp_funds_transaction controller, Extends EntityAPIController.
 */
class CommerceMarketplaceFundsTransactionController extends EntityAPIController {
  private $processor = NULL;

  public function __construct($entityType) {
    $class = commerce_marketplace_funds_get_remote_processor();
    if ($class) {
      $this->processor = new $class;
    }
    parent::__construct($entityType);
  }


  /**
   * Create a model - we first set up the values that are specific
   * to our model schema but then also go through the EntityAPIController
   * function.
   *
   * @param $type
   *   The machine-readable type of the model.
   *
   * @return
   *   A model object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our Model
    $values += array(
      'transaction_id' => '',
      'is_new' => TRUE,
      'from_account' => '',
      'to_account' => '',
      'order_id' => '',
      'status' => '',
      'amount' => array(),
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
    );
    if ($this->processor) {
      $this->processor->createTransaction($values);
    }
    $method_type = parent::create($values);
    return $method_type;
  }

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    if (empty($this->created) || !empty($this->is_new) || empty($entity->transaction_id)) {
      $entity->created = REQUEST_TIME;
    }
    $entity->changed = REQUEST_TIME;
    if ($this->processor) {
      $this->processor->saveTransaction($entity);
    }
    return parent::save($entity, $transaction);
  }
}
