<?php

/**
 * @file
 * The controller for the NPC instance entity.
 */

class DrpgNPCInstanceEntityController extends EntityAPIController {

  /**
   * Saves an NPC instance entity.
   *
   * @param DrpgNPCInstance $npc_instance
   *   The NPC instance to save.
   *
   * @return DrpgNPCInstance
   *   The saved NPC instance.
   */
  public function save($npc_instance) {
    $npc_instance->updated = time();

    if (isset($npc_instance->is_new) && $npc_instance->is_new) {
      $npc_instance->created = time();
    }

    parent::save($npc_instance);

    return $npc_instance;
  }

}
