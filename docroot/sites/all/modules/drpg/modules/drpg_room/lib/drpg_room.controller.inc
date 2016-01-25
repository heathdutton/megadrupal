<?php

/**
 * @file
 * The controller for the room entity.
 */

class DrpgRoomEntityController extends EntityAPIController {

  /**
   * Saves a room entity.
   *
   * @param DrpgRoom $room
   *   The room to save.
   *
   * @return DrpgRoom
   *   The saved room.
   */
  public function save($room) {
    $room->updated = time();

    if (isset($room->is_new) && $room->is_new) {
      $room->created = time();
    }

    parent::save($room);

    return $room;
  }

}
