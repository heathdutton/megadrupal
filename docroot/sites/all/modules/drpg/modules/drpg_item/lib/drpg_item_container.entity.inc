<?php

/**
 * @file
 * Defines the item container entity.
 */

class DrpgItemContainer extends Entity {

  // @codingStandardsIgnoreStart
  public $item_container_id = NULL;
  public $label = '';
  public $item_quantity = 0;
  public $item_rarity = 0;
  public $item_rarity_operator = '=';
  public $created = '';
  public $updated = '';

  // Not stored. Used to provide access to referenced items.
  public $items;
  public $item_instances;
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'drpg_item_container');

    // Check for new item container.
    if (!$this->item_container_id) {
      $this->created = time();
    }
  }

  /**
   * Specifies the default entity label.
   */
  protected function defaultLabel() {
    return $this->label;
  }

  /**
   * Specifies the entity URI.
   */
  protected function defaultUri() {
    return array('path' => 'admin/drpg/item_containers/manage/' . $this->internalIdentifier());
  }

}
