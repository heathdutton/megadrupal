<?php
/**
 * @file
 * Functionality related to reordering of options by other modules.
 */

/**
 * Allow other modules to reorder the tree.
 *
 * This will insert the current item under/above a specified other item, or
 * on top of the list.
 *
 * @param array $options
 *   List of options, with index "link_current" or the menu link ID.
 * @param string $relative_position
 *   Where to insert the current item. Either "top", "above_{mlid}" or
 *   "below_{mlid}".
 *
 * @return array
 *   Reordered array of options.
 */
function _menu_link_weight_reorder_options(array $options, $relative_position) {
  $weight = MENU_LINK_WEIGHT_MIN_DELTA;
  $reordered_options = array();
  if (isset($options['link_current'])) {
    $link_current = $options['link_current'];
    unset($options['link_current']);
  }
  else {
    // Return original options.
    return $options;
  }
  // If the "relative position" is to be "top", insert the current link all the
  // way on top of the list.
  if ($relative_position == 'top') {
    _menu_link_weight_insert_item($reordered_options, 'link_current', $link_current, $weight);
  }
  foreach ($options as $id => $option) {
    // If the relative position is to be "above_{mlid}", insert the current link
    // just above the item with the given menu link ID.
    if ($relative_position == 'above_' . $id) {
      _menu_link_weight_insert_item($reordered_options, 'link_current', $link_current, $weight);
    }

    // Insert the regular items.
    _menu_link_weight_insert_item($reordered_options, $id, $options[$id], $weight);

    // If the relative position is to be "above_{mlid}", insert the current link
    // just below the item with the given menu link ID.
    if ($relative_position == 'below_' . $id) {
      _menu_link_weight_insert_item($reordered_options, 'link_current', $link_current, $weight);
    }
  }
  return $reordered_options;
}

/**
 * Helper function for _menu_link_weight_reorder_options().
 *
 * Adds a value, weight to the reordered options array and increments weight
 * by one.
 *
 * @param array $reordered_options
 *   Reordered options array.
 * @param string $index
 *   Either "current_link" or the menu link ID.
 * @param string $value
 *   The title of the link in the old options array.
 * @param int $weight
 *   The weight of the link within the parent menu.
 */
function _menu_link_weight_insert_item(array &$reordered_options, $index, $value, &$weight) {
  $reordered_options[$index] = $value;
  $reordered_options[$index]['weight'] = $weight;
  $weight++;
}
