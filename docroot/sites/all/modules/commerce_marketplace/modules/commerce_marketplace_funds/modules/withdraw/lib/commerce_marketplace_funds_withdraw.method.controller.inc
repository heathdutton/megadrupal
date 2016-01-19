<?php

/**
 * @file
 * Contains entity controller implementation for cmp_funds_withdraw_method.
 */

/**
 * Class CommerceMarketplaceFundsWithdrawMethodController.
 * cmp_funds_withdraw_method controller, Extends EntityAPIController.
 */
class CommerceMarketplaceFundsWithdrawMethodController extends EntityAPIController {
  private $processor;


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
      'id' => '',
      'is_new' => TRUE,
      'type' => '',
      'account_id' => '',
      'created' => '',
      'changed' => '',
    );
    if ($this->processor) {
      $this->processor->createWithdrawMethod($values);
    }
    $method = parent::create($values);
    return $method;
  }

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    if ($this->processor) {
      $this->processor->saveWithdrawMethod($entity);
    }
    return parent::save($entity, $transaction);
  }
  /**
   * Overriding the buildContent function to add entity specific fields.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);
    $content['cmp_funds_withdraw_method'] =  array(
      '#markup' => theme('cmp_funds_withdraw_method', array('entity' => $entity)),
    );

    return $content;
  }
}

