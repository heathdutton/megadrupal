<?php

/**
 * @file
 * Contains Entity class for cmp_funds_withdraw_method_type.
 */

class CommerceMarketplaceFundsWithdrawMethodType extends Entity {
  public $type;
  public $label;

  public function __construct($values = array(), $entityType = NULL) {
    parent::__construct($values, $entityType);
  }
}

