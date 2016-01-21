<?php

/**
 * @file
 * Defines the item entity.
 */

class DrpgItem extends Entity {

  // @codingStandardsIgnoreStart
  public $item_id = NULL;
  public $label = '';
  public $rarity = 0;
  public $equippable = 0;
  public $currency_cost = 0;
  public $currency_value = 0;
  public $ui_asset_path = '';
  public $game_asset_path = '';
  public $created = '';
  public $updated = '';
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'drpg_item');

    // Check for new item.
    if (!$this->item_id) {
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
    return array('path' => 'admin/drpg/items/manage/' . $this->internalIdentifier());
  }

}
