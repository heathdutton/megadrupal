<?php

/**
 * @file
 * Defines the item instance entity.
 */

class DrpgItemInstance extends Entity {

  // @codingStandardsIgnoreStart
  public $item_instance_id = NULL;
  public $type = '';
  public $entity_id = 0;
  public $item_id = 0;
  public $created = '';
  public $updated = '';
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'drpg_item_instance');

    // Check for new item instance.
    if (!$this->item_instance_id) {
      $this->created = time();
    }
  }
}
