<?php


class CommerceMarketplaceFundsTransaction extends Entity {
  public $transaction_id,
    // Originator account ID
    $from_account,
    // Originator account standard entity
    $from_account_entity = NULL,
    // Originator account entitymetadatawrapper
    $originator_account = NULL,
    // Destination account ID
    $to_account,
    // Destination account standard entity
    $to_account_entity = NULL,
    // Destination account entitymetadatawrapper
    $destination_account = NULL,
    // Commerce Order ID
    $order_id,
    // Commerce Order standard entity
    $order_entity,
    // Commerce Order entitymetadatawrapper,
    $order = NULL,
    $amount,
    $created,
    $changed;

  public function __construct($values = array(), $entityType = NULL) {
    parent::__construct($values, $entityType);

    if ($this->from_account) {
      $this->from_account_entity = commerce_marketplace_funds_account_load($this->from_account);
      $this->originator_account = entity_metadata_wrapper('cmp_funds_account', $this->from_account_entity);
    }

    if ($this->order_id) {
      $this->order_entity = commerce_order_load($this->order_id);
    }
    if ($this->to_account) {
      $this->to_account_entity = commerce_marketplace_funds_account_load($this->to_account);
      $this->destination_account = entity_metadata_wrapper('cmp_funds_account', $this->to_account_entity);
    }
  }

  public function save() {
    if (empty($this->created) && (!empty($this->is_new) || !$this->transaction_id)) {
      $this->created = REQUEST_TIME;
    }
    $this->changed = REQUEST_TIME;

    parent::save();
  }
}

