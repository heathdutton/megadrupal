<?php
/**
 * @file Integration with the Menu Link Weight module.
 *
 * Allows for synchronization with relative weights instead of the absolute
 * numeric weights.
 */

/**
 * Looks for a relative position in the target tree to insert current link at.
 *
 * Example: "under menu link #434" (where #434 is in the target language). This
 * means we will insert the current link right under link #434 within the list
 * of children of the parent for the current link.
 *
 * The order of the search is:
 *
 * 1. See whether the link ought to be on top of the subtree. If so, abort.
 * 2. Search upwards within siblings of the source link, starting at the
 *    position of the source link itself. If we find a match, put the new item
 *    underneath the matched item.
 * 3. If no match is found: search downwards within siblings of the source link,
 *    starting at the positon of the source for current item. If we find a
 *    match, put the new item above the matched item.
 *
 * When a match is found in the source tree, we will proceed to find the target
 * link (in our current language) for the source link (in the source language),
 * and put the current link either above or below that.
 *
 * @param array $translation_source_link
 *   Menu link for the translation source of the node we are adding/editing.
 * @param string $target_language
 *   Language code of the node we are adding/editing.
 *
 * @return string|NULL
 *   Either "below_{mlid}", "above_{mlid}", or "top".
 */
function _menu_link_sync_get_relative_position(array $translation_source_link, $target_language) {
  // Get a sorted list of all the siblings of the source link.
  $tree = _menu_link_sync_get_tree($translation_source_link['menu_name'], $translation_source_link['plid']);

  // If the item is on top in the translation source tree, we can assume the
  // translated item ought to be on top as well and conclude our search here.
  $top_item = reset($tree);
  if ($top_item['link']['mlid'] == $translation_source_link['mlid']) {
    return 'top';
  }

  // Reorder the list of siblings so we go first to the item above the current
  // item, then the link above that one, etc. If we have a match, we will insert
  // the current item underneath the matched item.
  $result = _menu_link_sync_get_position_in_tree($tree, 'up', $translation_source_link, $target_language);
  if (!$result) {
    // Then go downwards and do the same, except in this case we will insert
    // the item _above_ the matched item.
    $result = _menu_link_sync_get_position_in_tree($tree, 'down', $translation_source_link, $target_language);
  }
  return $result;
}

/**
 * Helper function for _menu_link_sync_get_relative_position().
 *
 * This will search for an item to put a link underneath/above. Valid matches
 * are any links with a corresponding item in the target language. Will search
 * either upwards or downwards, starting at the source link itself.
 *
 * @param array $tree
 *   List of all siblings of the link for the translation source.
 * @param string $search_direction
 *   Either "up" or "down". Will search upwards or downwards in the list,
 *   respectively.
 *
 * @return string
 *   Either "above_{mlid}" or "below_{mlid}".
 */
function _menu_link_sync_get_position_in_tree(array $tree, $search_direction, $translation_source_link, $target_language) {
  $skip = TRUE;
  if ($search_direction == 'up') {
    // Reverse the tree so we can search upwards.
    $tree = array_reverse($tree);
  }
  foreach ($tree as $item) {
    // Ignore all items that are underneath the link (if search direction is
    // "up") / above the link (if search direction is "down").
    if ($item['link']['mlid'] == $translation_source_link['mlid']) {
      $skip = FALSE;
      // Do not process the current item.
      continue;
    }
    // Cover the edge case of a parent having several children pointing to the
    // same path.
    if ($item['link']['link_path'] == $translation_source_link['link_path']) {
      continue;
    }
    if ($skip === TRUE) {
      // Item is underneath the link (if direction = up) / above the link
      // (if direction = down). Skip.
      continue;
    }
    // See if there is an item that also exists in the tree for the target
    // language.
    $target_language_link = _menu_link_sync_get_link_for_target_language($item['link'], $target_language, TRUE);
    if (!empty($target_language_link)) {
      // We have a match! Use this link.
      if ($search_direction == 'up') {
        // If we are searching upwards (from the current item to the top) in
        // the source list, we want to place the current item underneath
        // the target of matched link in the target list.
        return 'below_' . $target_language_link['mlid'];
      }
      elseif ($search_direction == 'down') {
        // If we are searching downwards (from the current item to the bottom)
        // in the source list, we want to place the current item above the
        // target of matched link in the target list.
        return 'above_' . $target_language_link['mlid'];
      }
      else {
        throw new Exception(t('Menu Link Sync error: Invalid search direction.'));
      }
    }
  }
  return NULL;
}
