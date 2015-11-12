<?php

/**
 * Returns HTML for a wrapper for a menu sub-tree.
 *
 * @param $variables
 *   An associative array containing:
 *   - tree: An HTML string containing the tree's items.
 *
 * @see twbs_preprocess_menu_tree()
 * @ingroup themeable
 */
function twbs_menu_tree($variables) {
  return '<ul class="menu">' . $variables['tree'] . '</ul>';
}
