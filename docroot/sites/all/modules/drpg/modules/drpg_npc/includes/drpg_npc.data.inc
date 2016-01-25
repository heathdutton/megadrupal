<?php
/**
 * @file
 * Defines NPC data functionality.
 */

/**
 * Page callback to get NPC data.
 */
function drpg_npc_data() {
  $response = new DrpgNPCResponse();
  $response->success = TRUE;
  $response->npc = NULL;

  drpg_json_output($response);
}
