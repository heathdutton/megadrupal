<?php

/**
 * @file
 * Contains Entity class for cmp_funds_withdraw_method.
 */

class CommerceMarketplaceFundsWithdrawMethod extends Entity {
  public $id,
    $account_id,
    $account_entity = NULL,
    $account = NULL,
    $type,
    $created,
    $changed;

  public function __construct($values = array(), $entityType = NULL) {
    parent::__construct($values, $entityType);
    if (isset($this->account_id) && $this->account_id) {
      $this->account_entity = commerce_marketplace_funds_account_load($this->account_id);
      $this->account = entity_metadata_wrapper('cmp_funds_withdraw_method', $this->account_entity);
    }
  }

  protected function defaultLabel() {
    return t('Withdraw method @id', array('@id' => $this->id));
  }

  protected function defaultUri() {
    $owner_info = entity_get_info($this->account->owner_type);
    $owner = commerce_marketplace_funds_get_account_owner($this->account);
    $entity_uri = $owner_info['uri callback']($owner);

    return array('path' => $entity_uri['path'] . '/withdraw/methods/' . $this->id);
  }


  public function uri() {
    return array('path' => 'withdraw_method/' . $this->id);
  }
}

