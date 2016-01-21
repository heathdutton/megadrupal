<?php

/**
 * @file
 * API documentation for Node level blocks module.
 */

/**
 * Allow modules to override the blocks list.
 * @param type $blocks the list of blocks.
 * @param type $context contains the node on which the node level blocks are shown.
 */
function hook_node_level_blocks_alter(&$blocks, $context) {
  if ($context['node']->type == 'page') {
    foreach ($blocks as &$block) {
      if ($block['delta'] == 'block_delta') {
        $block['nlb_force_region'] = TRUE;
      }
      unset($block);
    }
  }
}

/**
 * Allow modules to override the regions list.
 * @param type $regions the list of regions.
 * @param type $context contains the node on which the node level blocks are shown.
 */
function hook_node_level_blocks_regions_alter(&$regions, $context) {
}
