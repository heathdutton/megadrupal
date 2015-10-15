<?php

/**
 * @file
 * The controller for the item entity.
 */

class DrpgItemEntityController extends EntityAPIController {

  /**
   * Saves an item entity.
   *
   * @param DrpgItem $item
   *   The item to save.
   *
   * @return DrpgItem
   *   The saved item.
   */
  public function save($item) {
    $item->updated = time();

    if (isset($item->is_new) && $item->is_new) {
      $item->created = time();
    }

    parent::save($item);

    return $item;
  }

}
