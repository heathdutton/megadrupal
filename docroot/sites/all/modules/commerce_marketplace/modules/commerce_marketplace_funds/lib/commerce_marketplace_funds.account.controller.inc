<?php

/**
 * @file
 * Contains entity controller implementation for cmp_funds_account.
 */

/**
 * Class CommerceMarketplaceFundsAccountController.
 * cmp_funds_account controller, Extends EntityAPIControllerExportable.
 */
class CommerceMarketplaceFundsAccountController extends EntityAPIController {

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
      'account_id' => '',
      'is_new' => TRUE,
      'owner_type' => '',
      'owner_id' => '',
      'balance' => array(),
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
    );
    if ($this->processor) {
      $this->processor->createAccount($values);
    }

    $method_type = parent::create($values);
    return $method_type;
  }

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    if ($this->processor) {
      $this->processor->saveAccount($entity);
    }
    return parent::save($entity, $transaction);
  }
}
