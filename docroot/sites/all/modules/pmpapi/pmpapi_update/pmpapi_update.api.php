<?php

/**
 * @file
 * Hooks provided by the PMPAPI update module.
 */

/**
 * Act on an incoming PMP item to be deleted/updated.
 *
 * @param object $item
 *   The item to be updated/deleted (as a SimpleXML element)
 *
 * @param string $operation
 *   The operation to be performed on the item (as a PubSubHubbub topic URI)
 */
function hook_pmpapi_update_incoming_item_alter($item, $operation) {
  $guid = (string) $item->guid;
  watchdog('my_module', t('Received notification with following GUID:') . ' ' . $guid);
}