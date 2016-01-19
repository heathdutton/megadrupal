<?php

/**
 * @file
 * alias_preview.inc
 */

/**
 * Returns a pathauto alias for a given node type and title.
 *
 * @param string $type
 *   The node type machine name.
 * @param string $title
 *   The raw title value, i.e. copy and pasted from the node edit form title
 *   field element.
 * @param string $original
 *   (Optional) If passed, the function will assume this node is already saved
 *   with an alias, and prevents a node from incrementing with a "-0" suffix
 *   if the newly calculated suffix matches this own node's original alias.
 *
 * @return string $alias
 *   A pathauto alias for this node type, or all nodes if no type-specific
 *   pattern is set. Note: This is the part that comes after the base URL.
 *
 * @see alias_preview_ajax()
 */
function alias_preview_make_alias($type, $title, $original = NULL) {
  // Only continues if a pathauto pattern is available.
  $pattern = trim(variable_get("pathauto_node_{$type}_pattern", FALSE));
  if (empty($pattern)) {
    $pattern = trim(variable_get('pathauto_node_pattern', FALSE));
  }
  if (!isset($pattern) || empty($pattern)) {
    return FALSE;
  }

  // Makes a bare-minimum simple node object to pass to pathauto function.
  $node = (object) array(
    'title' => $title,
    'type' => $type,
  );
  module_load_include('inc', 'node', 'node.pages');
  node_object_prepare($node);

  // Let pathauto apply token patterns, do it's magic, and uniquify the alias.
  $op = 'return';
  $source = '';
  $data = array('node' => $node);
  $language = isset($node->language) ? $node->language : LANGUAGE_NONE;
  module_load_include('inc', 'pathauto');
  $alias = pathauto_create_alias('node', $op, $source, $data, $type, $language);
  pathauto_alias_uniquify($alias, $source, $language);

  // Detects when an original link == the newly calculated preview.
  if (!empty($original)) {
    if ($alias == "{$original}-0") {
      $alias = $original;
    }
  }

  return $alias;
}
