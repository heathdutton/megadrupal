<?php

/**
 * @file
 * The controller for the item container entity.
 */

class DrpgItemContainerEntityController extends EntityAPIController {

  /**
   * Saves an item container entity.
   *
   * @param DrpgItemContainer $item_container
   *   The item container to save.
   *
   * @return DrpgItemContainer
   *   The saved item container.
   */
  public function save($item_container) {
    $item_container->updated = time();

    if (isset($item_container->is_new) && $item_container->is_new) {
      $item_container->created = time();
    }

    parent::save($item_container);

    return $item_container;
  }

}
