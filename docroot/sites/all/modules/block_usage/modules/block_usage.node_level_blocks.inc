<?php

/**
 * @file
 * Node-Level Blocks' shadow implementation of hook_block_usage().
 */

/**
 * Implements hook_block_usage().
 */
function node_level_blocks_block_usage($module, $delta) {
  // Find block ids.
  $bids = _block_usage_bids($module, $delta);
  if (!$bids) {
    return array();
  }

  // Find NLB items.
  $items = db_query('SELECT nid, region FROM {node_level_blocks} WHERE bid IN (:bids)', array(
    ':bids' => $bids,
  ))->fetchAllKeyed(0, 1);

  // Fetch nodes and collect titles for matches.
  $nodes = node_load_multiple(array_keys($items));
  $matches = array();
  foreach ($nodes as $nid => $node) {
    $label = t('@title (# @nid)', array('@title' => $node->title, '@nid' => $nid));
    $matches[] = l($label, 'node/' . $nid) . ' (' . $items[$nid] . ')';
  }

  return $matches;
}
