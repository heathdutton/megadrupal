<?php
namespace Drupal\at_route\Hook;

use Drupal\at_route\Route\Importer;

class Menu {
  private $items;

  /**
   * @var Importer
   */
  private $importer;

  public function __construct(Importer $importer) {
    $this->importer = $importer;
  }

  /**
   * Get all menu items.
   */
  public function getMenuItems() {
    $items = array();
    foreach (at_modules('at_route') as $module) {
      $this->importer->setModule($module);
      if ($this->importer->getConfigPath()) {
        $items += $this->importer->import();
      }
    }
    return $items;
  }
}
