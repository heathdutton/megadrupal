<?php
/**
 * @file
 * Defines room data functionality.
 */

/**
 * Page callback to get room data.
 */
function drpg_room_data($room_id) {
  global $user;

  $room = drpg_room_get_room($room_id);

  if ($room != NULL) {
    $avatar = drpg_avatar_get_user_active_avatar($user->uid);
    if ($avatar !== FALSE) {
      // Delete existing entity instances for this room and avatar.
      drpg_room_delete_instances('drpg_avatar', $avatar->avatar_id);
      // Create new entity instances for this room and avatar.
      drpg_room_create_instances('drpg_avatar', $avatar->avatar_id, $room);
    }
  }

  $response = new DrpgRoomResponse();
  $response->success = ($room != NULL);
  $response->room = $room;

  // Remove properties never needed by the client.
  unset($response->room->drpg_multi_item_container_ref);
  unset($response->room->drpg_multi_npc_ref);

  drpg_json_output($response);
}
