<?php

/**
 * @file
 * The UI controller for the item entity.
 */

class DrpgItemUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = t('Manage Items.');
    return $items;
  }
}
