<?php
/**
 * @file
 * Defines GlacierEntityContentUIController
 */

namespace Drupal\aws_glacier_ui\Entity;

/**
 * Class GlacierEntityContentUIController
 */
class GlacierEntityContentUIController extends \EntityContentUIController{

  /**
   * {@inheritDoc}
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    // Fix for https://drupal.org/node/2091293
    unset($items[$this->path . '/list']);

    foreach ($items as $router => $item) {
      if (strpos($router, AWS_GLACIER_ADMIN_PATH . '/vaults/manage/%entity_object') !== FALSE) {
        unset($items[$router]);
      }
      if (strpos($router, AWS_GLACIER_ADMIN_PATH . '/jobs/%entity_object/edit') !== FALSE) {
        unset($items[$router]);
      }
      if (strpos($router, AWS_GLACIER_ADMIN_PATH . '/jobs/add') !== FALSE) {
        unset($items[$router]);
      }
      unset($items[AWS_GLACIER_ADMIN_PATH . '/archives/add']);
      unset($items[AWS_GLACIER_ADMIN_PATH . '/archives/%entity_object/edit']);
    }
    return $items;
  }
}
