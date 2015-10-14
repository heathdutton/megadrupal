<?php

/**
 * Processes variables for book-node-export-html.tpl.php.
 *
 * @param $variables
 *   An associative array containing the following keys:
 *   - node
 *   - children
 *
 * @see book-node-export-html.tpl.php
 */
function twbs_preprocess_book_node_export_html(&$variables) {
  $variables['depth'] = $variables['node']->book['depth'];
  $variables['title'] = check_plain($variables['node']->title);
  $variables['content'] = $variables['node']->rendered;
}
