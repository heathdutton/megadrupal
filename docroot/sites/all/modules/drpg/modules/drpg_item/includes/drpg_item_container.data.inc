<?php
/**
 * @file
 * Defines item data functionality.
 */

/**
 * Page callback to get item container data.
 */
function drpg_item_container_data($item_container_id) {
  $item_container = drpg_item_get_item_container($item_container_id);

  $response = new DrpgItemContainerResponse();
  $response->success = ($item_container != NULL);
  $response->itemContainer = $item_container;

  drpg_json_output($response);
}
