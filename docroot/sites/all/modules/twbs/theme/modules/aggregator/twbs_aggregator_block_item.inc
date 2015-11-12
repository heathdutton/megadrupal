<?php

/**
 * Returns HTML for an individual feed item for display in the block.
 *
 * @param $variables
 *   An associative array containing:
 *   - item: The item to be displayed.
 *   - feed: Not used.
 *
 * @ingroup themeable
 */
function twbs_aggregator_block_item($variables) {
  // Display the external link to the item.
  return '<a href="' . check_url($variables['item']->link) . '">' . check_plain($variables['item']->title) . "</a>\n";
}
