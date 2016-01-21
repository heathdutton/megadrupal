<?php
/**
 * @file
 * Contains the ElementUIController class.
 */

class ElementUIController extends EntityBundleableUIController {

  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    // The hook_menu() implementation in EntityBundleableUIController doesn't
    // pass the bundle as an access argument for create operations, which really
    // sucks because this makes it impossible to define bundle specific create
    // permissions. That is why I override.
    $items[$this->path . '/add/%']['access arguments'] = array('create', $this->entityType, $this->id_count + 1);
    foreach ($items as $item) {
      $item['access callback'] = 'element_access';
    }
    return $items;
  }

}
