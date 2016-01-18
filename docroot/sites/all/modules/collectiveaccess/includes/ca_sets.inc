<?php

/**
 * @file
 * Implements CollectiveAccess sets functionality
 */

class CollectiveAccessSets {
  protected $collectiveaccess_instance;
  protected $set_id;

  /**
   * Create a new CollectiveAccessSet
   * @param CollectiveAccessInstance $ca - object connecting to CollectiveAccess installation
   */
  public function __construct(CollectiveAccessInstance $ca, $id = NULL) {
    $this->collectiveaccess_instance = $ca;
    $this->set_id = $id;
  }

  public function getSetsForItem($item_id, $type = 'ca_objects') {
    $params = array(
      'type' => $type,
      'item_id' => $item_id,
    );
    return $this->collectiveaccess_instance->execute('ItemInfo', 'getSetsForItem', $params);
  }
  
  /**
   * Retrieve all sets
   * @return a structured array with information about all sets
   */
  public function getSets() {
    return $this->collectiveaccess_instance->execute('ItemInfo', 'getSets', array());
  }
}
