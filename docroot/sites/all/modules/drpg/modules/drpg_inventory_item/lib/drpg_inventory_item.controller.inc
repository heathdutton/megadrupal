<?php

/**
 * @file
 * The controller for the inventory item entity.
 */

class DrpgInventoryItemEntityController extends EntityAPIController {

  /**
   * Saves an inventory item entity.
   *
   * @param DrpgInventoryItem $inventory_item
   *   The inventory item to save.
   *
   * @return DrpgInventoryItem
   *   The saved inventory item.
   */
  public function save($inventory_item) {
    $inventory_item->updated = time();

    if (isset($inventory_item->is_new) && $inventory_item->is_new) {
      $inventory_item->created = time();
    }

    parent::save($inventory_item);

    return $inventory_item;
  }

}
