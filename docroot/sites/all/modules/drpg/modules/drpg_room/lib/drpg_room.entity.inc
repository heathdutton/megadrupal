<?php

/**
 * @file
 * Defines the room entity.
 */

class DrpgRoom extends Entity {

  // @codingStandardsIgnoreStart
  public $room_id = NULL;
  public $label = '';
  public $item_container_quantity = 0;
  public $created = '';
  public $updated = '';

  // Not stored. Used to provide access to referenced items.
  public $item_containers;
  public $npcs;
  public $npc_instances;
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'drpg_room');

    // Check for new room.
    if (!$this->room_id) {
      $this->created = time();
    }
  }

  /**
   * Specifies the default entity label.
   */
  protected function defaultLabel() {
    return $this->label;
  }

  /**
   * Specifies the entity URI.
   */
  protected function defaultUri() {
    return array('path' => 'admin/drpg/rooms/manage/' . $this->internalIdentifier());
  }

}
