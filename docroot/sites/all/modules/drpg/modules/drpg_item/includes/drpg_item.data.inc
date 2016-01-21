<?php
/**
 * @file
 * Defines item data functionality.
 */

/**
 * Page callback to get items data.
 */
function drpg_item_all_items_data() {

  $items = drpg_item_get_items();

  $response = new DrpgItemResponse();
  $response->success = TRUE;
  $response->items = $items;

  drpg_json_output($response);
}
