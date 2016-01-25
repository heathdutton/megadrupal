<?php

/**
 * @file
 * The UI controller for the attribute entity.
 */

class DrpgAttributeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = t('Manage Attributes.');
    return $items;
  }
}
