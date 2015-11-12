<?php

/**
 * @file
 * Contains entity controller implementation for cmp_funds_withdraw_method_type.
 */

/**
 * Class CommerceMarketplaceFundsWithdrawMethodTypeController.
 * cmp_funds_withdraw_method_type controller, Extends EntityAPIControllerExportable.
 */
class CommerceMarketplaceFundsWithdrawMethodTypeController extends EntityAPIControllerExportable {
  public function __construct($entityType) {
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
      'id' => '',
      'is_new' => TRUE,
      'type' => '',
      'label' => '',
      'weight' => '',
      'data' => '',
    );

    $method_type = parent::create($values);
    return $method_type;
  }
}
