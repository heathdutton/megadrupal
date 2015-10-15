<?php
/**
 * @file
 * Defines inventory item data functionality.
 */

/**
 * Page callback to get inventory items data for an entity.
 */
function drpg_inventory_item_inventory_data($type, $entity_id) {
  $inventory = drpg_inventory_item_get_inventory($type, $entity_id);

  $response = new DrpgDataResponse();
  $response->success = ($inventory != NULL);
  $response->inventory = $inventory;

  // Remove values not used by the game client.
  foreach ($inventory['item_instances'] as $index => $inventory_item) {
    unset($inventory['item_instances'][$index]->drpg_item_ref);
  }

  drpg_json_output($response);
}
