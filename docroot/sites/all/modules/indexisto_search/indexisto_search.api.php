<?php
/**
 * @file
 * Indexisto module API.
 */

/**
 * Implements hook_indexisto_search_node_data_prepare_alter().
 *
 * Get ready to send data JSON.
 */
function hook_indexisto_search_node_data_prepare_alter(&$data) {
  // Do some stuff.
  return $data;
}

/**
 * Implements hook_indexisto_search_comment_data_prepare_alter().
 *
 * Get ready to send data JSON.
 */
function hook_indexisto_search_comment_data_prepare_alter(&$data) {
  // Do some stuff.
  return $data;
}
