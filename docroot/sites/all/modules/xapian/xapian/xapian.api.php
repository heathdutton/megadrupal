<?php

/**
 * Overwrite the way of index a node type.
 *
 * This function must have this pattern: <node_type>_node_xapian_index().
 * Formally it is not a hook.
 *
 * @param $node
 *   A node object.
 * @return
 *   An array of terms, where each term is an array with the following
 *   indexes:
 *   - 'data': One piece of information that is going to be indexed
 *     (required).
 *   - 'weight': How important is this piece.
 *   - 'type': Either 'text' for normal data or 'term' to add a xapian
 *     term to the xapian document.
 */
function story_node_xapian_index($node) {
  // for example index only the title
  $terms = array();
  $terms[] = array(
    'weight' => 1,
    'data' => $node->title,
    'type' => 'text',
  );
  return $terms;
}
