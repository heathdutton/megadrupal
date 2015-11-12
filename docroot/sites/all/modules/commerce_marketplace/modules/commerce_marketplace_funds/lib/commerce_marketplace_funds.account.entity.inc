<?php


class CommerceMarketplaceFundsAccount extends Entity {
  public $account_id,
    $owner_type,
    $owner_id,
    $balance,
    $created,
    $changed,
    $owner_entity = NULL,
    $owner = NULL;

  public function __construct($values = array(), $entityType = NULL) {
    parent::__construct($values, $entityType);
    $this->owner_entity = entity_load_single($this->owner_type, $this->owner_id);
    $this->owner = entity_metadata_wrapper($this->owner_type, $this->owner_entity);
  }

  public function save() {
    if (empty($this->created) && (!empty($this->is_new) || !$this->account_id)) {
      $this->created = REQUEST_TIME;
    }
    $this->changed = REQUEST_TIME;

    parent::save();
  }
}

