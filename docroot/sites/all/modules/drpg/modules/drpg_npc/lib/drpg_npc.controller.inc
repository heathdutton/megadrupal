<?php

/**
 * @file
 * The controller for the NPC entity.
 */

class DrpgNPCEntityController extends EntityAPIController {

  /**
   * Saves an NPC entity.
   *
   * @param DrpgNPC $npc
   *   The NPC to save.
   *
   * @return DrpgNPC
   *   The saved NPC.
   */
  public function save($npc) {
    $npc->updated = time();

    if (isset($npc->is_new) && $npc->is_new) {
      $npc->created = time();
    }

    parent::save($npc);

    return $npc;
  }

}
