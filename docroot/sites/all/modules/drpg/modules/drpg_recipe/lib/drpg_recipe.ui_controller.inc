<?php

/**
 * @file
 * The UI controller for the recipe entity.
 */

class DrpgRecipeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = t('Manage Recipes.');
    return $items;
  }
}
