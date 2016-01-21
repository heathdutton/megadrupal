<?php

/**
 * @file
 * Defines the inventory item entity.
 */

class DrpgInventoryItem extends Entity {

  // @codingStandardsIgnoreStart
  public $inventory_item_id = NULL;
  public $type = '';
  public $entity_id = 0;
  public $equipped = 0;
  public $created = '';
  public $updated = '';

  // Not stored. Used to provide access to referenced items.
  public $item_id = 0;
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'drpg_inventory_item');

    // Check for new inventory item.
    if (!$this->inventory_item_id) {
      $this->created = time();
    }
  }

  /**
   * Specifies the default entity label.
   */
  protected function defaultLabel() {
    return $this->inventory_item_id;
  }

  /**
   * Specifies the entity URI.
   */
  protected function defaultUri() {
    return array('path' => 'admin/drpg/inventory_items/manage/' . $this->internalIdentifier());
  }

}
