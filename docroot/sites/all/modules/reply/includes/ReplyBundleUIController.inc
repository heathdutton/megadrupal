<?php
/**
 * @file
 * Defines the ReplyBundleUIController class.
 */

/**
 * Defines the ReplyBundleUIController.
 */
class ReplyBundleUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = t('Manage Reply bundles.');
    return $items;
  }
}
