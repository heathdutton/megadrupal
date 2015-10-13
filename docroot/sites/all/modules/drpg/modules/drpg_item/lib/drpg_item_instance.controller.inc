<?php

/**
 * @file
 * The controller for the item instance entity.
 */

class DrpgItemInstanceEntityController extends EntityAPIController {

  /**
   * Saves an item instance entity.
   *
   * @param DrpgItemInstance $item_instance
   *   The item instance to save.
   *
   * @return DrpgItemInstance
   *   The saved item instance.
   */
  public function save($item_instance) {
    $item_instance->updated = time();

    if (isset($item_instance->is_new) && $item_instance->is_new) {
      $item_instance->created = time();
    }

    parent::save($item_instance);

    return $item_instance;
  }

}
