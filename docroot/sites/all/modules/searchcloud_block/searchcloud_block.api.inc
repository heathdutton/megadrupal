<?php

/**
 * Function hook_searchcloud_block_get_result_alter.
 *
 * Give modules the power to change the query.
 * 
 * @param object $query
 *   The search query object
 */
function hook_searchcloud_block_get_result_alter(&$query) {

}

/**
 * Function hook_searchcloud_block_check_keys_execute.
 * 
 * Notification about what keys are searched at, and what the new count is.
 * Gives modules the power to store additional data alongside of the searched
 * keys. This must happen using custom storage (of course).
 * 
 * This hook is the messenger.
 * 
 * @param array $keys
 *   The search keys
 * @param int $count
 *   The count for keys
 */
function hook_searchcloud_block_check_keys_execute($keys, $count) {

}